#ifndef STREAM_P_H
#define STREAM_P_H

#include "stream.h"
#include "volumeobject_p.h"

namespace QPulseAudio {

template <typename PAInfo>
void Stream::updateStream(const PAInfo *info)
{
    updateVolumeObject(info);

    if (m_name != QString::fromUtf8(info->name)) {
        m_name = QString::fromUtf8(info->name);
        Q_EMIT nameChanged();
    }
    if (VolumeObject::d->m_hasVolume != info->has_volume) {
        VolumeObject::d->m_hasVolume = info->has_volume;
        Q_EMIT hasVolumeChanged();
    }
    if (VolumeObject::d->m_volumeWritable != info->volume_writable) {
        VolumeObject::d->m_volumeWritable = info->volume_writable;
        Q_EMIT isVolumeWritableChanged();
    }
    if (m_clientIndex != info->client) {
        m_clientIndex = info->client;
        Q_EMIT clientChanged();
    }
    if (m_virtualStream != (info->client == PA_INVALID_INDEX)) {
        m_virtualStream = info->client == PA_INVALID_INDEX;
        Q_EMIT virtualStreamChanged();
    }
    if (m_corked != info->corked) {
        m_corked = info->corked;
        Q_EMIT corkedChanged();
    }
}
    
}
#endif
