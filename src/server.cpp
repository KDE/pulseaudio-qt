/*
    Copyright 2016 David Rosca <nowrep@gmail.com>

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

#include "server.h"
#include "server_p.h"

#include "context.h"
#include "context_p.h"
#include "debug.h"
#include "sink.h"
#include "source.h"

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

void ServerPrivate::update(const pa_server_info *info)
{
    m_defaultSinkName = QString::fromUtf8(info->default_sink_name);
    m_defaultSourceName = QString::fromUtf8(info->default_source_name);

    q->updateDefaultDevices();
}

template<typename Type, typename Vector> static Type *findByName(const Vector &vector, const QString &name)
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

} // PulseAudioQt
