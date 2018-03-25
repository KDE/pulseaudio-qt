#ifndef VOLUMEOBJECT_P_H
#define VOLUMEOBJECT_P_H

#include "volumeobject.h"
#include "pulseobject_p.h"

namespace QPulseAudio {

class VolumeObjectPrivate {
public:
    explicit VolumeObjectPrivate(VolumeObject* q);

    VolumeObject* q;
    pa_cvolume m_volume;
    bool m_muted;
    bool m_hasVolume;
    bool m_volumeWritable;
    QStringList m_channels;

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
