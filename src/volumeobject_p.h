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

#include "pulseobject_p.h"
#include "volumeobject.h"

namespace PulseAudioQt
{
class VolumeObjectPrivate
{
public:
    explicit VolumeObjectPrivate(VolumeObject *q);

    VolumeObject *q;
    pa_channel_map foo;
    pa_cvolume m_volume;
    bool m_muted;
    bool m_hasVolume;
    bool m_volumeWritable;
    QVector<QString> m_channels;
    QStringList m_rawChannels;

    pa_cvolume cvolume() const;

    template<typename PAInfo> void updateVolumeObject(PAInfo *info)
    {
        q->PulseObject::d->updatePulseObject(info);
        if (m_muted != info->mute) {
            m_muted = info->mute;
            Q_EMIT q->mutedChanged();
        }
        if (!pa_cvolume_equal(&m_volume, &info->volume)) {
            m_volume = info->volume;
            Q_EMIT q->volumeChanged();
            Q_EMIT q->channelVolumesChanged();
        }
        QVector<QString> infoChannels;
        infoChannels.reserve(info->channel_map.channels);
        for (int i = 0; i < info->channel_map.channels; ++i) {
            infoChannels << QString::fromUtf8(pa_channel_position_to_pretty_string(info->channel_map.map[i]));
        }
        if (m_channels != infoChannels) {
            m_channels = infoChannels;
            Q_EMIT q->channelsChanged();
        }

        QStringList infoRawChannels;
        infoRawChannels.reserve(info->channel_map.channels);
        for (int i = 0; i < info->channel_map.channels; ++i) {
            infoRawChannels << QString::fromUtf8(pa_channel_position_to_string(info->channel_map.map[i]));
        }
        if (m_rawChannels != infoRawChannels) {
            m_rawChannels = infoRawChannels;
            Q_EMIT q->rawChannelsChanged();
        }
    }
};
}
#endif
