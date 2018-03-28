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
#ifndef VOLUMEOBJECT_P_H
#define VOLUMEOBJECT_P_H

#include <pulse/volume.h>

#include "volumeobject.h"
#include "pulseobject_p.h"

namespace PulseAudioQt {

class VolumeObjectPrivate {
public:
    explicit VolumeObjectPrivate(VolumeObject *q);

    VolumeObject *q;
    pa_channel_map foo;
    pa_cvolume m_volume;
    bool m_muted;
    bool m_hasVolume;
    bool m_volumeWritable;
    QStringList m_channels;

    pa_cvolume cvolume() const;
};

template <typename PAInfo>
void VolumeObject::updateVolumeObject(PAInfo *info)
{
    updatePulseObject(info);
    if (d->m_muted != info->mute) {
        d->m_muted = info->mute;
        Q_EMIT mutedChanged();
    }
    if (!pa_cvolume_equal(&d->m_volume, &info->volume)) {
        d->m_volume = info->volume;
        Q_EMIT volumeChanged();
        Q_EMIT channelVolumesChanged();
    }
    QStringList infoChannels;
    infoChannels.reserve(info->channel_map.channels);
    for (int i = 0; i < info->channel_map.channels; ++i) {
        infoChannels << QString::fromUtf8(pa_channel_position_to_pretty_string(info->channel_map.map[i]));
    }
    if (d->m_channels != infoChannels) {
        d->m_channels = infoChannels;
        Q_EMIT channelsChanged();
    }
}

}
#endif
