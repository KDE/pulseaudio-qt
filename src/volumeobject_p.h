#ifndef VOLUMEOBJECT_P_H
#define VOLUMEOBJECT_P_H

#include "volumeobject.h"
#include "pulseobject_p.h"

namespace QPulseAudio {
 
template <typename PAInfo>
void VolumeObject::updateVolumeObject(PAInfo *info)
{
    updatePulseObject(info);
    if (m_muted != info->mute) {
        m_muted = info->mute;
        Q_EMIT mutedChanged();
    }
    if (!pa_cvolume_equal(&m_volume, &info->volume)) {
        m_volume = info->volume;
        Q_EMIT volumeChanged();
        Q_EMIT channelVolumesChanged();
    }
    QStringList infoChannels;
    infoChannels.reserve(info->channel_map.channels);
    for (int i = 0; i < info->channel_map.channels; ++i) {
        infoChannels << QString::fromUtf8(pa_channel_position_to_pretty_string(info->channel_map.map[i]));
    }
    if (m_channels != infoChannels) {
        m_channels = infoChannels;
        Q_EMIT channelsChanged();
    }
}

    
}
#endif
