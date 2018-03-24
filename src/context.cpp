/*
    Copyright 2014-2015 Harald Sitter <sitter@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "context.h"
#include "server.h"

#include <QAbstractEventDispatcher>
#include "debug.h"
#include <QMutexLocker>
#include <QTimer>
#include <QDBusServiceWatcher>
#include <QDBusConnection>

#include <pulse/mainloop.h>
#include <pulse/pulseaudio.h>
#include <pulse/glib-mainloop.h>
#include <pulse/ext-stream-restore.h>

#include "card.h"
#include "client.h"
#include "sink.h"
#include "sinkinput.h"
#include "source.h"
#include "sourceoutput.h"
#include "streamrestore.h"
#include "module.h"

namespace QPulseAudio
{

Context* Context::s_context = nullptr;

class ContextPrivate {
public:
    // Don't forget to add things to reset().
    SinkMap m_sinks;
    SinkInputMap m_sinkInputs;
    SourceMap m_sources;
    SourceOutputMap m_sourceOutputs;
    ClientMap m_clients;
    CardMap m_cards;
    ModuleMap m_modules;
    StreamRestoreMap m_streamRestores;
    Server *m_server;

    pa_context *m_context;
    pa_glib_mainloop *m_mainloop;

    QString m_newDefaultSink;
    QString m_newDefaultSource;

    int m_references;
};


static bool isGoodState(int eol)
{
    if (eol < 0) {
        // Error
        return false;
    }

    if (eol > 0) {
        // End of callback chain
        return false;
    }

    return true;
}

// --------------------------

static void sink_cb(pa_context *context, const pa_sink_info *info, int eol, void *data)
{
    if (!isGoodState(eol))
        return;
    Q_ASSERT(context);
    Q_ASSERT(data);
    ((Context *)data)->sinkCallback(info);
}

static void sink_input_callback(pa_context *context, const pa_sink_input_info *info, int eol, void *data)
{
    if (!isGoodState(eol))
        return;
    // pulsesink probe is used by gst-pulse only to query sink formats (not for playback)
    if (qstrcmp(info->name, "pulsesink probe") == 0) {
        return;
    }
    if (const char *id = pa_proplist_gets(info->proplist, "module-stream-restore.id")) {
        if (qstrcmp(id, "sink-input-by-media-role:event") == 0) {
            qCDebug(PLASMAPA) << "Ignoring event role sink input.";
            return;
        }
    }
    Q_ASSERT(context);
    Q_ASSERT(data);
    ((Context *)data)->sinkInputCallback(info);
}

static void source_cb(pa_context *context, const pa_source_info *info, int eol, void *data)
{
    if (!isGoodState(eol))
        return;
    // FIXME: This forces excluding monitors
    if (info->monitor_of_sink != PA_INVALID_INDEX)
        return;
    Q_ASSERT(context);
    Q_ASSERT(data);
    ((Context *)data)->sourceCallback(info);
}

static void source_output_cb(pa_context *context, const pa_source_output_info *info, int eol, void *data)
{
    if (!isGoodState(eol))
        return;
    // FIXME: This forces excluding these apps
    if (const char *app = pa_proplist_gets(info->proplist, PA_PROP_APPLICATION_ID)) {
        if (strcmp(app, "org.PulseAudio.pavucontrol") == 0
                || strcmp(app, "org.gnome.VolumeControl") == 0
                || strcmp(app, "org.kde.kmixd") == 0)
            return;
    }
    Q_ASSERT(context);
    Q_ASSERT(data);
    ((Context *)data)->sourceOutputCallback(info);
}

static void client_cb(pa_context *context, const pa_client_info *info, int eol, void *data)
{
    if (!isGoodState(eol))
        return;
    Q_ASSERT(context);
    Q_ASSERT(data);
    ((Context *)data)->clientCallback(info);
}

static void card_cb(pa_context *context, const pa_card_info *info, int eol, void *data)
{
    if (!isGoodState(eol))
        return;
    Q_ASSERT(context);
    Q_ASSERT(data);
    ((Context *)data)->cardCallback(info);
}

static void module_info_list_cb(pa_context *context, const pa_module_info *info, int eol, void *data)
{
    if (!isGoodState(eol))
        return;
    Q_ASSERT(context);
    Q_ASSERT(data);
    ((Context *)data)->moduleCallback(info);
}

static void server_cb(pa_context *context, const pa_server_info *info, void *data)
{
    Q_ASSERT(context);
    Q_ASSERT(data);
    ((Context *)data)->serverCallback(info);
}

static void context_state_callback(pa_context *context, void *data)
{
    Q_ASSERT(data);
    ((Context *)data)->contextStateCallback(context);
}

static void subscribe_cb(pa_context *context, pa_subscription_event_type_t type, uint32_t index, void *data)
{
    Q_ASSERT(data);
    ((Context *)data)->subscribeCallback(context, type, index);
}

static void ext_stream_restore_read_cb(pa_context *context, const pa_ext_stream_restore_info *info, int eol, void *data)
{
    if (!isGoodState(eol)) {
        return;
    }
    Q_ASSERT(context);
    Q_ASSERT(data);
    ((Context *)data)->streamRestoreCallback(info);
}

static void ext_stream_restore_subscribe_cb(pa_context *context, void *data)
{
    Q_ASSERT(context);
    Q_ASSERT(data);
    if (!PAOperation(pa_ext_stream_restore_read(context, ext_stream_restore_read_cb, data))) {
        qCWarning(PLASMAPA) << "pa_ext_stream_restore_read() failed";
    }
}

static void ext_stream_restore_change_sink_cb(pa_context *context, const pa_ext_stream_restore_info *info, int eol, void *data)
{
    if (!isGoodState(eol)) {
        return;
    }
    Q_ASSERT(context);
    Q_ASSERT(data);
    if (qstrncmp(info->name, "sink-input-by", 13) == 0) {
        Context *context = static_cast<Context *>(data);
        const QByteArray deviceData = context->newDefaultSink().toUtf8();
        pa_ext_stream_restore_info newinfo;
        newinfo.name = info->name;
        newinfo.channel_map = info->channel_map;
        newinfo.volume = info->volume;
        newinfo.mute = info->mute;
        newinfo.device = deviceData.constData();
        context->streamRestoreWrite(&newinfo);
    }
}

static void ext_stream_restore_change_source_cb(pa_context *context, const pa_ext_stream_restore_info *info, int eol, void *data)
{
    if (!isGoodState(eol)) {
        return;
    }
    Q_ASSERT(context);
    Q_ASSERT(data);
    if (qstrncmp(info->name, "source-output-by", 16) == 0) {
        Context *context = static_cast<Context *>(data);
        const QByteArray deviceData = context->newDefaultSource().toUtf8();
        pa_ext_stream_restore_info newinfo;
        newinfo.name = info->name;
        newinfo.channel_map = info->channel_map;
        newinfo.volume = info->volume;
        newinfo.mute = info->mute;
        newinfo.device = deviceData.constData();
        context->streamRestoreWrite(&newinfo);
    }
}

// --------------------------

Context::Context(QObject *parent)
    : QObject(parent)
    , d(new ContextPrivate)
{
    d->m_server = new Server(this);
    d->m_context = nullptr;
    d->m_mainloop = nullptr;
    d->m_references = 0;
    QDBusServiceWatcher *watcher = new QDBusServiceWatcher(QStringLiteral("org.pulseaudio.Server"),
                                                           QDBusConnection::sessionBus(),
                                                           QDBusServiceWatcher::WatchForRegistration,
                                                           this);
    connect(watcher, &QDBusServiceWatcher::serviceRegistered, this, &Context::connectToDaemon);
    connectToDaemon();
}

Context::~Context()
{
    if (d->m_context) {
        pa_context_unref(d->m_context);
        d->m_context = nullptr;
    }

    if (d->m_mainloop) {
        pa_glib_mainloop_free(d->m_mainloop);
        d->m_mainloop = nullptr;
    }

    reset();

    delete d;
}

Context *Context::instance()
{
    if (!s_context) {
        s_context = new Context;
    }
    return s_context;
}

void Context::ref()
{
    ++d->m_references;
}

void Context::unref()
{
    if (--d->m_references == 0) {
        delete this;
        s_context = nullptr;
    }
}

void Context::subscribeCallback(pa_context *context, pa_subscription_event_type_t type, uint32_t index)
{
    Q_ASSERT(context == d->m_context);

    switch (type & PA_SUBSCRIPTION_EVENT_FACILITY_MASK) {
    case PA_SUBSCRIPTION_EVENT_SINK:
        if ((type & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE) {
            d->m_sinks.removeEntry(index);
        } else {
            if (!PAOperation(pa_context_get_sink_info_by_index(context, index, sink_cb, this))) {
                qCWarning(PLASMAPA) << "pa_context_get_sink_info_by_index() failed";
                return;
            }
        }
        break;

    case PA_SUBSCRIPTION_EVENT_SOURCE:
        if ((type & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE) {
            d->m_sources.removeEntry(index);
        } else {
            if (!PAOperation(pa_context_get_source_info_by_index(context, index, source_cb, this))) {
                qCWarning(PLASMAPA) << "pa_context_get_source_info_by_index() failed";
                return;
            }
        }
        break;

    case PA_SUBSCRIPTION_EVENT_SINK_INPUT:
        if ((type & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE) {
            d->m_sinkInputs.removeEntry(index);
        } else {
            if (!PAOperation(pa_context_get_sink_input_info(context, index, sink_input_callback, this))) {
                qCWarning(PLASMAPA) << "pa_context_get_sink_input_info() failed";
                return;
            }
        }
        break;

    case PA_SUBSCRIPTION_EVENT_SOURCE_OUTPUT:
        if ((type & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE) {
            d->m_sourceOutputs.removeEntry(index);
        } else {
            if (!PAOperation(pa_context_get_source_output_info(context, index, source_output_cb, this))) {
                qCWarning(PLASMAPA) << "pa_context_get_sink_input_info() failed";
                return;
            }
        }
        break;

    case PA_SUBSCRIPTION_EVENT_CLIENT:
        if ((type & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE) {
            d->m_clients.removeEntry(index);
        } else {
            if (!PAOperation(pa_context_get_client_info(context, index, client_cb, this))) {
                qCWarning(PLASMAPA) << "pa_context_get_client_info() failed";
                return;
            }
        }
        break;

    case PA_SUBSCRIPTION_EVENT_CARD:
        if ((type & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE) {
            d->m_cards.removeEntry(index);
        } else {
            if (!PAOperation(pa_context_get_card_info_by_index(context, index, card_cb, this))) {
                qCWarning(PLASMAPA) << "pa_context_get_card_info_by_index() failed";
                return;
            }
        }
        break;

    case PA_SUBSCRIPTION_EVENT_MODULE:
        if ((type & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE) {
            d->m_modules.removeEntry(index);
        } else {
            if (!PAOperation(pa_context_get_module_info_list(context, module_info_list_cb, this))) {
                qCWarning(PLASMAPA) << "pa_context_get_module_info_list() failed";
                return;
            }
        }
        break;


    case PA_SUBSCRIPTION_EVENT_SERVER:
        if (!PAOperation(pa_context_get_server_info(context, server_cb, this))) {
            qCWarning(PLASMAPA) << "pa_context_get_server_info() failed";
            return;
        }
        break;

    }
}

void Context::contextStateCallback(pa_context *c)
{
    qCDebug(PLASMAPA) << "state callback";
    pa_context_state_t state = pa_context_get_state(c);
    if (state == PA_CONTEXT_READY) {
        qCDebug(PLASMAPA) << "ready";

        // 1. Register for the stream changes (except during probe)
        if (d->m_context == c) {
            pa_context_set_subscribe_callback(c, subscribe_cb, this);

            if (!PAOperation(pa_context_subscribe(c, (pa_subscription_mask_t)
                                           (PA_SUBSCRIPTION_MASK_SINK|
                                            PA_SUBSCRIPTION_MASK_SOURCE|
                                            PA_SUBSCRIPTION_MASK_CLIENT|
                                            PA_SUBSCRIPTION_MASK_SINK_INPUT|
                                            PA_SUBSCRIPTION_MASK_SOURCE_OUTPUT|
                                            PA_SUBSCRIPTION_MASK_CARD|
                                            PA_SUBSCRIPTION_MASK_MODULE|
                                            PA_SUBSCRIPTION_MASK_SERVER), nullptr, nullptr))) {
                qCWarning(PLASMAPA) << "pa_context_subscribe() failed";
                return;
            }
        }

        if (!PAOperation(pa_context_get_sink_info_list(c, sink_cb, this))) {
            qCWarning(PLASMAPA) << "pa_context_get_sink_info_list() failed";
            return;
        }

        if (!PAOperation(pa_context_get_source_info_list(c, source_cb, this))) {
            qCWarning(PLASMAPA) << "pa_context_get_source_info_list() failed";
            return;
        }

        if (!PAOperation(pa_context_get_client_info_list(c, client_cb, this))) {
            qCWarning(PLASMAPA) << "pa_context_client_info_list() failed";
            return;
        }

        if (!PAOperation(pa_context_get_card_info_list(c, card_cb, this))) {
            qCWarning(PLASMAPA) << "pa_context_get_card_info_list() failed";
            return;
        }

        if (!PAOperation(pa_context_get_sink_input_info_list(c, sink_input_callback, this))) {
            qCWarning(PLASMAPA) << "pa_context_get_sink_input_info_list() failed";
            return;
        }

        if (!PAOperation(pa_context_get_source_output_info_list(c, source_output_cb, this))) {
            qCWarning(PLASMAPA) << "pa_context_get_source_output_info_list() failed";
            return;
        }

        if (!PAOperation(pa_context_get_module_info_list(c, module_info_list_cb, this))) {
            qCWarning(PLASMAPA) << "pa_context_get_module_info_list() failed";
            return;
        }

        if (!PAOperation(pa_context_get_server_info(c, server_cb, this))) {
            qCWarning(PLASMAPA) << "pa_context_get_server_info() failed";
            return;
        }

        if (PAOperation(pa_ext_stream_restore_read(c, ext_stream_restore_read_cb, this))) {
            pa_ext_stream_restore_set_subscribe_cb(c, ext_stream_restore_subscribe_cb, this);
            PAOperation(pa_ext_stream_restore_subscribe(c, 1, nullptr, this));
        } else {
            qCWarning(PLASMAPA) << "Failed to initialize stream_restore extension";
        }
    } else if (!PA_CONTEXT_IS_GOOD(state)) {
        qCWarning(PLASMAPA) << "context kaput";
        if (d->m_context) {
            pa_context_unref(d->m_context);
            d->m_context = nullptr;
        }
        reset();
        QTimer::singleShot(1000, this, &Context::connectToDaemon);
    }
}

void Context::sinkCallback(const pa_sink_info *info)
{
    // This parenting here is a bit weird
    d->m_sinks.updateEntry(info, this);
}

void Context::sinkInputCallback(const pa_sink_input_info *info)
{
    d->m_sinkInputs.updateEntry(info, this);
}

void Context::sourceCallback(const pa_source_info *info)
{
    d->m_sources.updateEntry(info, this);
}

void Context::sourceOutputCallback(const pa_source_output_info *info)
{
    d->m_sourceOutputs.updateEntry(info, this);
}

void Context::clientCallback(const pa_client_info *info)
{
    d->m_clients.updateEntry(info, this);
}

void Context::cardCallback(const pa_card_info *info)
{
    d->m_cards.updateEntry(info, this);
}

void Context::moduleCallback(const pa_module_info *info)
{
    d->m_modules.updateEntry(info, this);
}

void Context::streamRestoreCallback(const pa_ext_stream_restore_info *info)
{
    if (qstrcmp(info->name, "sink-input-by-media-role:event") != 0) {
        return;
    }

    const int eventRoleIndex = 1;
    StreamRestore *obj = qobject_cast<StreamRestore *>(d->m_streamRestores.data().value(eventRoleIndex));

    if (!obj) {
        QVariantMap props;
        props.insert(QStringLiteral("application.icon_name"),
                     QStringLiteral("preferences-desktop-notification"));
        obj = new StreamRestore(eventRoleIndex, props, this);
        d->m_streamRestores.insert(obj);
    }

    obj->update(info);
}

void Context::serverCallback(const pa_server_info *info)
{
    d->m_server->update(info);
}

void Context::setCardProfile(quint32 index, const QString &profile)
{
    if (!d->m_context) {
        return;
    }
    qCDebug(PLASMAPA) << index << profile;
    if (!PAOperation(pa_context_set_card_profile_by_index(d->m_context,
                                                          index,
                                                          profile.toUtf8().constData(),
                                                          nullptr, nullptr))) {
        qCWarning(PLASMAPA) << "pa_context_set_card_profile_by_index failed";
        return;
    }
}

void Context::setDefaultSink(const QString &name)
{
    if (!d->m_context) {
        return;
    }
    const QByteArray nameData = name.toUtf8();
    if (!PAOperation(pa_context_set_default_sink(d->m_context,
                                                 nameData.constData(),
                                                 nullptr,
                                                 nullptr))) {
        qCWarning(PLASMAPA) << "pa_context_set_default_sink failed";
    }

    // Change device for all entries in stream-restore database
    d->m_newDefaultSink = name;
    if (!PAOperation(pa_ext_stream_restore_read(d->m_context,
                                                ext_stream_restore_change_sink_cb,
                                                this))) {
        qCWarning(PLASMAPA) << "pa_ext_stream_restore_read failed";
    }
}

void Context::setDefaultSource(const QString &name)
{
    if (!d->m_context) {
        return;
    }
    const QByteArray nameData = name.toUtf8();
    if (!PAOperation(pa_context_set_default_source(d->m_context,
                                                 nameData.constData(),
                                                 nullptr,
                                                 nullptr))) {
        qCWarning(PLASMAPA) << "pa_context_set_default_source failed";
    }

    // Change device for all entries in stream-restore database
    d->m_newDefaultSource = name;
    if (!PAOperation(pa_ext_stream_restore_read(d->m_context,
                                                ext_stream_restore_change_source_cb,
                                                this))) {
        qCWarning(PLASMAPA) << "pa_ext_stream_restore_read failed";
    }
}

void Context::streamRestoreWrite(const pa_ext_stream_restore_info *info)
{
    if (!d->m_context) {
        return;
    }
    if (!PAOperation(pa_ext_stream_restore_write(d->m_context,
                                                 PA_UPDATE_REPLACE,
                                                 info,
                                                 1,
                                                 true,
                                                 nullptr,
                                                 nullptr))) {
        qCWarning(PLASMAPA) << "pa_ext_stream_restore_write failed";
    }
}

void Context::connectToDaemon()
{
    if (d->m_context) {
        return;
    }

    // We require a glib event loop
    if (!QByteArray(QAbstractEventDispatcher::instance()->metaObject()->className()).contains("EventDispatcherGlib")) {
        qCWarning(PLASMAPA) << "Disabling PulseAudio integration for lack of GLib event loop";
        return;
    }

    qCDebug(PLASMAPA) <<  "Attempting connection to PulseAudio sound daemon";
    if (!d->m_mainloop) {
        d->m_mainloop = pa_glib_mainloop_new(nullptr);
        Q_ASSERT(d->m_mainloop);
    }

    pa_mainloop_api *api = pa_glib_mainloop_get_api(d->m_mainloop);
    Q_ASSERT(api);
    d->m_context = pa_context_new(api, "QPulse");
    Q_ASSERT(d->m_context);

    if (pa_context_connect(d->m_context, NULL, PA_CONTEXT_NOFAIL, nullptr) < 0) {
        pa_context_unref(d->m_context);
        pa_glib_mainloop_free(d->m_mainloop);
        d->m_context = nullptr;
        d->m_mainloop = nullptr;
        return;
    }
    pa_context_set_state_callback(d->m_context, &context_state_callback, this);
}

void Context::reset()
{
    d->m_sinks.reset();
    d->m_sinkInputs.reset();
    d->m_sources.reset();
    d->m_sourceOutputs.reset();
    d->m_clients.reset();
    d->m_cards.reset();
    d->m_modules.reset();
    d->m_streamRestores.reset();
    d->m_server->reset();
}

const SinkMap &Context::sinks() const
{
    return d->m_sinks;
}

bool Context::isValid()
{
    return d->m_context && d->m_mainloop;
}

const SinkInputMap &Context::sinkInputs() const
{
    return d->m_sinkInputs;
}

const SourceMap &Context::sources() const
{
    return d->m_sources;
}

const SourceOutputMap &Context::sourceOutputs() const
{
    return d->m_sourceOutputs;
}

const ClientMap &Context::clients() const
{
    return d->m_clients;
}

const CardMap &Context::cards() const
{
    return d->m_cards;
}

const ModuleMap &Context::modules() const
{
    return d->m_modules;
}

const StreamRestoreMap &Context::streamRestores() const
{
    return d->m_streamRestores;
}

Server* Context::server() const
{
    return d->m_server;
}

QString Context::newDefaultSource() const
{
    return d->m_newDefaultSource;
}


QString Context::newDefaultSink() const
{
    return d->m_newDefaultSink;
}

void Context::setGenericVolume(quint32 index, int channel, qint64 newVolume, pa_cvolume cVolume, const std::function<pa_operation*(pa_context *, uint32_t, const pa_cvolume *, pa_context_success_cb_t, void *)>& pa_set_volume)
{
        if (!d->m_context) {
            return;
        }
        newVolume = qBound<qint64>(0, newVolume, PA_VOLUME_MAX);
        pa_cvolume newCVolume = cVolume;
        if (channel == -1) { // -1 all channels
            const qint64 diff = newVolume - pa_cvolume_max(&cVolume);
            for (int i = 0; i < newCVolume.channels; ++i) {
                newCVolume.values[i] = qBound<qint64>(0, newCVolume.values[i] + diff, PA_VOLUME_MAX);
            }
        } else {
            Q_ASSERT(newCVolume.channels > channel);
            newCVolume.values[channel] = newVolume;
        }
        if (!pa_set_volume(d->m_context, index, &newCVolume, nullptr, nullptr)) {
            qCWarning(PLASMAPA) <<  "pa_set_volume failed";
            return;
        }
}

void Context::setGenericMute(quint32 index, bool mute, const std::function<pa_operation*(pa_context *, uint32_t, int, pa_context_success_cb_t, void *)>& pa_set_mute)
{
    if (!d->m_context) {
        return;
    }
    if (!PAOperation(pa_set_mute(d->m_context, index, mute, nullptr, nullptr))) {
        qCWarning(PLASMAPA) <<  "pa_set_mute failed";
        return;
    }
}

void Context::setGenericPort(quint32 index, const QString &portName, const std::function<pa_operation*(pa_context *, uint32_t, const char*, pa_context_success_cb_t, void *)>& pa_set_port)
{
    if (!d->m_context) {
        return;
    }
    if (!PAOperation(pa_set_port(d->m_context,
                                    index,
                                    portName.toUtf8().constData(),
                                    nullptr,
                                    nullptr))) {
        qCWarning(PLASMAPA) << "pa_set_port failed";
        return;
    }
}

void Context::setGenericDeviceForStream(quint32 streamIndex, quint32 deviceIndex, const std::function<pa_operation*(pa_context *, uint32_t, uint32_t, pa_context_success_cb_t, void *)>& pa_move_stream_to_device)
{
    if (!d->m_context) {
        return;
    }
    if (!PAOperation(pa_move_stream_to_device(d->m_context,
                                                streamIndex,
                                                deviceIndex,
                                                nullptr,
                                                nullptr))) {
        qCWarning(PLASMAPA) << "pa_move_stream_to_device failed";
        return;
    }
}

} // QPulseAudio
