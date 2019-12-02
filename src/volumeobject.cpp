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

#include <pulse/volume.h>

#include "volumeobject.h"
#include "volumeobject_p.h"

namespace PulseAudioQt
{
VolumeObject::VolumeObject(QObject *parent)
    : PulseObject(parent)
    , d(new VolumeObjectPrivate(this))
{
}

VolumeObjectPrivate::VolumeObjectPrivate(VolumeObject *q)
    : q(q)
    , m_muted(true)
    , m_hasVolume(true)
    , m_volumeWritable(true)
{
    pa_cvolume_init(&m_volume);
}

VolumeObject::~VolumeObject()
{
}

qint64 VolumeObject::volume() const
{
    return pa_cvolume_max(&d->m_volume);
}

bool VolumeObject::isMuted() const
{
    return d->m_muted;
}

pa_cvolume VolumeObjectPrivate::cvolume() const
{
    return m_volume;
}

bool VolumeObject::hasVolume() const
{
    return d->m_hasVolume;
}

bool VolumeObject::isVolumeWritable() const
{
    return d->m_volumeWritable;
}

QVector<QString> VolumeObject::channels() const
{
    return d->m_channels;
}

QStringList VolumeObject::rawChannels() const
{
    return d->m_rawChannels;
}

QVector<qreal> VolumeObject::channelVolumes() const
{
    QVector<qreal> ret;
    ret.reserve(d->m_volume.channels);
    for (int i = 0; i < d->m_volume.channels; ++i) {
        ret << d->m_volume.values[i];
    }
    return ret;
}

} // PulseAudioQt
