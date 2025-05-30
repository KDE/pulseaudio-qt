/*
    SPDX-FileCopyrightText: 2016 David Rosca <nowrep@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "server.h"
#include "server_p.h"

#include "context.h"
#include "context_p.h"
#include "debug.h"
#include "sink.h"
#include "source.h"

using namespace std::chrono_literals;

namespace PulseAudioQt
{
Server::Server(Context *context)
    : QObject(context)
    , d(new ServerPrivate(this))
{
    Q_ASSERT(context);

    connect(&context->d->m_sinks, &MapBaseQObject::added, this, &Server::updateDefaultDevices);
    connect(&context->d->m_sinks, &MapBaseQObject::removed, this, &Server::updateDefaultDevices);
    connect(&context->d->m_sources, &MapBaseQObject::added, this, &Server::updateDefaultDevices);
    connect(&context->d->m_sources, &MapBaseQObject::removed, this, &Server::updateDefaultDevices);

    // WirePlumber detection works based on connected clients.
    // Since we act on individual client changes let's compress them otherwise we may be switching state multiple times
    // for no reason.
    d->m_wirePlumberFindTimer.setInterval(250ms); // arbitrary compression time
    d->m_wirePlumberFindTimer.setSingleShot(true);
    connect(&d->m_wirePlumberFindTimer, &QTimer::timeout, this, [this] {
        d->findWirePlumber();
    });
    connect(&context->d->m_clients, &MapBaseQObject::added, &d->m_wirePlumberFindTimer, qOverload<>(&QTimer::start));
    connect(&context->d->m_clients, &MapBaseQObject::removed, &d->m_wirePlumberFindTimer, qOverload<>(&QTimer::start));
}

Server::~Server()
{
}

ServerPrivate::ServerPrivate(Server *q)
    : q(q)
    , m_defaultSink(nullptr)
    , m_defaultSource(nullptr)
{
}

ServerPrivate::~ServerPrivate()
{
}

Sink *Server::defaultSink() const
{
    return d->m_defaultSink;
}

void Server::setDefaultSink(Sink *sink)
{
    Q_ASSERT(sink);
    Context::instance()->setDefaultSink(sink->name());
}

Source *Server::defaultSource() const
{
    return d->m_defaultSource;
}

void Server::setDefaultSource(Source *source)
{
    Q_ASSERT(source);
    Context::instance()->setDefaultSource(source->name());
}

void Server::reset()
{
    if (d->m_defaultSink) {
        d->m_defaultSink = nullptr;
        Q_EMIT defaultSinkChanged(d->m_defaultSink);
    }

    if (d->m_defaultSource) {
        d->m_defaultSource = nullptr;
        Q_EMIT defaultSourceChanged(d->m_defaultSource);
    }
}

void Server::disconnectSignals()
{
    disconnect(this, &Server::defaultSinkChanged, nullptr, nullptr);
    disconnect(this, &Server::defaultSourceChanged, nullptr, nullptr);
    disconnect(this, &Server::isPipeWireChanged, nullptr, nullptr);
    disconnect(this, &Server::updated, nullptr, nullptr);
    disconnect(this, &Server::hasWirePlumberChanged, nullptr, nullptr);
}

void ServerPrivate::update(const pa_server_info *info)
{
    m_defaultSinkName = QString::fromUtf8(info->default_sink_name);
    m_defaultSourceName = QString::fromUtf8(info->default_source_name);

    const bool isPw = QString::fromUtf8(info->server_name).contains(QLatin1String("PipeWire"));

    if (isPw != m_isPipeWire) {
        m_isPipeWire = isPw;
        Q_EMIT q->isPipeWireChanged();
    }

    // Do not use the pulseaudio server version for pipewire
    if (!isPw) {
        const QString pulseVersion = QString::fromUtf8(info->server_version);
        if (pulseVersion != m_version) {
            m_version = pulseVersion;
            Q_EMIT q->versionChanged();
        }
    }

    q->updateDefaultDevices();

    Q_EMIT q->updated();
}

/** @private */
template<typename Type, typename Vector>
static Type *findByName(const Vector &vector, const QString &name)
{
    Type *out = nullptr;
    if (name.isEmpty()) {
        return out;
    }
    for (Type *t : vector) {
        out = t;
        if (out->name() == name) {
            return out;
        }
    }
    qCWarning(PULSEAUDIOQT) << "No object for name" << name;
    return out;
}

void Server::updateDefaultDevices()
{
    Sink *sink = findByName<Sink>(Context::instance()->d->m_sinks.data(), d->m_defaultSinkName);
    Source *source = findByName<Source>(Context::instance()->d->m_sources.data(), d->m_defaultSourceName);

    if (d->m_defaultSink != sink) {
        qCDebug(PULSEAUDIOQT) << "Default sink changed" << sink;
        d->m_defaultSink = sink;
        Q_EMIT defaultSinkChanged(d->m_defaultSink);
    }

    if (d->m_defaultSource != source) {
        qCDebug(PULSEAUDIOQT) << "Default source changed" << source;
        d->m_defaultSource = source;
        Q_EMIT defaultSourceChanged(d->m_defaultSource);
    }
}

bool Server::isPipeWire() const
{
    return d->m_isPipeWire;
}

QString Server::version() const
{
    return d->m_version;
}

void ServerPrivate::findWirePlumber()
{
    if (!m_isPipeWire) {
        return;
    }

    const auto clients = Context::instance()->clients();

    // Get the actual PipeWire version by reading the `core.version` property
    // from one of the clients in the list that exposes it
    for (const auto &client : clients) {
        if (client->properties().contains(QStringLiteral("core.version"))) {
            const QString pwVersion = client->properties().value(QStringLiteral("core.version")).toString();
            if (pwVersion != m_version) {
                m_version = pwVersion;
                Q_EMIT q->versionChanged();
                break;
            }
        }
    }

    for (const auto &client : clients) {
        if (client->properties().value(QStringLiteral("wireplumber.daemon")) == QLatin1String("true")) {
            m_wirePlumberVersion = client->properties().value(QStringLiteral("application.version")).toString();
            Q_EMIT q->hasWirePlumberChanged();
            return;
        }
    }

    // Found no plumber, mark as not availabe
    m_wirePlumberVersion = std::nullopt;
    Q_EMIT q->hasWirePlumberChanged();
}

bool Server::hasWirePlumber() const
{
    return d->m_wirePlumberVersion.has_value();
}

QString Server::wirePlumberVersion() const
{
    return d->m_wirePlumberVersion.value_or(QString());
}

} // PulseAudioQt
