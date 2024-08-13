/*
    SPDX-FileCopyrightText: 2014-2015 Harald Sitter <sitter@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "stream.h"
#include "context_p.h"
#include "stream_p.h"
#include "volumeobject_p.h"

namespace PulseAudioQt
{
Stream::Stream(QObject *parent)
    : VolumeObject(parent)
    , d(new StreamPrivate(this))
{
    VolumeObject::d->m_volumeWritable = false;
}

Stream::~Stream()
{
}

StreamPrivate::StreamPrivate(Stream *q)
    : q(q)
{
}

StreamPrivate::~StreamPrivate()
{
}

Client *Stream::client() const
{
    const auto &clients = Context::instance()->d->m_clients.data();
    auto it = std::find_if(clients.begin(), clients.end(), [this](Client *client) {
        return client->index() == d->m_clientIndex;
    });
    if (it != clients.end()) {
        return *it;
    }
    return nullptr;
}

bool Stream::isVirtualStream() const
{
    return d->m_virtualStream;
}

quint32 Stream::deviceIndex() const
{
    return d->m_deviceIndex;
}

bool Stream::isCorked() const
{
    return d->m_corked;
}

bool Stream::hasVolume() const
{
    return d->m_hasVolume;
}

} // PulseAudioQt
