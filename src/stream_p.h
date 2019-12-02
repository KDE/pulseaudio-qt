/*
    Copyright 2018 Nicolas Fella <nicolas.fella@gmx.de>

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
#ifndef STREAM_P_H
#define STREAM_P_H

#include "stream.h"
#include "volumeobject_p.h"

namespace PulseAudioQt
{
class StreamPrivate
{
public:
    explicit StreamPrivate(Stream *q);
    virtual ~StreamPrivate();

    Stream *q;

    quint32 m_deviceIndex;
    QString m_name;
    quint32 m_clientIndex;
    bool m_virtualStream;
    bool m_corked;

    template<typename PAInfo> void updateStream(const PAInfo *info)
    {
        q->VolumeObject::d->updateVolumeObject(info);

        if (m_name != QString::fromUtf8(info->name)) {
            m_name = QString::fromUtf8(info->name);
            Q_EMIT q->nameChanged();
        }
        if (q->VolumeObject::d->m_hasVolume != info->has_volume) {
            q->VolumeObject::d->m_hasVolume = info->has_volume;
            Q_EMIT q->hasVolumeChanged();
        }
        if (q->VolumeObject::d->m_volumeWritable != info->volume_writable) {
            q->VolumeObject::d->m_volumeWritable = info->volume_writable;
            Q_EMIT q->isVolumeWritableChanged();
        }
        if (m_clientIndex != info->client) {
            m_clientIndex = info->client;
            Q_EMIT q->clientChanged();
        }
        if (m_virtualStream != (info->client == PA_INVALID_INDEX)) {
            m_virtualStream = info->client == PA_INVALID_INDEX;
            Q_EMIT q->virtualStreamChanged();
        }
        if (m_corked != info->corked) {
            m_corked = info->corked;
            Q_EMIT q->corkedChanged();
        }
    }
};
}
#endif
