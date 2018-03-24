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

#ifndef CONTEXT_H
#define CONTEXT_H

#include <QMutex>
#include <QObject>
#include <QSet>
#include "maps.h"
#include "operation.h"
#include "kf5pulseaudioqt_export.h"

namespace QPulseAudio
{

class Server;
class ContextPrivate;

class KF5PULSEAUDIOQT_EXPORT Context : public QObject
{
    Q_OBJECT


public:
    Context(QObject *parent = nullptr);
    ~Context();

    static Context *instance();

    static const qint64 NormalVolume = PA_VOLUME_NORM;
    static const qint64 MinimalVolume = 0;
    static const qint64 MaximalVolume = (PA_VOLUME_NORM / 100.0) * 150;

    void ref();
    void unref();

    bool isValid();

    const SinkMap &sinks() const;
    const SinkInputMap &sinkInputs() const;
    const SourceMap &sources() const;
    const SourceOutputMap &sourceOutputs() const;
    const ClientMap &clients() const;
    const CardMap &cards() const;
    const ModuleMap &modules() const;
    const StreamRestoreMap &streamRestores() const;
    Server *server() const;
    QString newDefaultSink() const;
    QString newDefaultSource() const;

    void subscribeCallback(pa_context *context, pa_subscription_event_type_t type, uint32_t index);
    void contextStateCallback(pa_context *context);

    void sinkCallback(const pa_sink_info *info);
    void sinkInputCallback(const pa_sink_input_info *info);
    void sourceCallback(const pa_source_info *info);
    void sourceOutputCallback(const pa_source_output_info *info);
    void clientCallback(const pa_client_info *info);
    void cardCallback(const pa_card_info *info);
    void moduleCallback(const pa_module_info *info);
    void streamRestoreCallback(const pa_ext_stream_restore_info *info);
    void serverCallback(const pa_server_info *info);

    void setCardProfile(quint32 index, const QString &profile);
    void setDefaultSink(const QString &name);
    void setDefaultSource(const QString &name);
    void streamRestoreWrite(const pa_ext_stream_restore_info *info);

    void setGenericVolume(quint32 index, int channel, qint64 newVolume, pa_cvolume cVolume, const std::function<pa_operation*(pa_context *, uint32_t, const pa_cvolume *, pa_context_success_cb_t, void *)>& );

    void setGenericMute(quint32 index, bool mute, const std::function<pa_operation*(pa_context *, uint32_t, int, pa_context_success_cb_t, void *)>& );

    void setGenericPort(quint32 index, const QString &portName, const std::function<pa_operation*(pa_context *, uint32_t, const char*, pa_context_success_cb_t, void *)>& );

    void setGenericDeviceForStream(quint32 streamIndex, quint32 deviceIndex, const std::function<pa_operation*(pa_context *, uint32_t, uint32_t, pa_context_success_cb_t, void *)>& );

private:

    void connectToDaemon();
    void reset();

    ContextPrivate* d;
    static Context* s_context;
};

} // QPulseAudio

#endif // CONTEXT_H
