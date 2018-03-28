#ifndef STREAM_P_H
#define STREAM_P_H

#include "stream.h"
#include "volumeobject_p.h"

namespace QPulseAudio {

class StreamPrivate {
public:
    explicit StreamPrivate(Stream* q);
    virtual ~StreamPrivate();

    Stream* q;

    quint32 m_deviceIndex;
    QString m_name;
    quint32 m_clientIndex;
    bool m_virtualStream;
    bool m_corked;

};

template <typename PAInfo>
void Stream::updateStream(const PAInfo *info)
{
    updateVolumeObject(info);

    if (d->m_name != QString::fromUtf8(info->name)) {
        d->m_name = QString::fromUtf8(info->name);
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
    if (d->m_clientIndex != info->client) {
        d->m_clientIndex = info->client;
        Q_EMIT clientChanged();
    }
    if (d->m_virtualStream != (info->client == PA_INVALID_INDEX)) {
        d->m_virtualStream = info->client == PA_INVALID_INDEX;
        Q_EMIT virtualStreamChanged();
    }
    if (d->m_corked != info->corked) {
        d->m_corked = info->corked;
        Q_EMIT corkedChanged();
    }
}

}
#endif
