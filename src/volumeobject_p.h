/*
    SPDX-FileCopyrightText: 2018 Nicolas Fella <nicolas.fella@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/
#ifndef VOLUMEOBJECT_P_H
#define VOLUMEOBJECT_P_H

#include <pulse/proplist.h>
#include <pulse/volume.h>

#include "indexedpulseobject_p.h"
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
    bool m_muted = true;
    bool m_volumeWritable = true;
    bool m_muteWritable = true;
    QStringList m_channels;
    QStringList m_rawChannels;

    pa_cvolume cvolume() const;

    template<typename PAInfo>
    void updateVolumeObject(PAInfo *info)
    {
        const auto readBoolProplistValue = [](const char *value, bool defaultValue) {
            const QString stringValue = QString::fromUtf8(value).trimmed().toLower();
            if (stringValue == QLatin1String("true") || stringValue == QLatin1String("1")) {
                return true;
            } else if (stringValue == QLatin1String("false") || stringValue == QLatin1String("0")) {
                return false;
            }

            return defaultValue;
        };

        q->IndexedPulseObject::d->updatePulseObject(info);
        q->PulseObject::d->updateProperties(info);

        const bool volumeWritable = readBoolProplistValue(pa_proplist_gets(info->proplist, "device.volume.write-volume-value"), true);
        if (m_volumeWritable != volumeWritable) {
            m_volumeWritable = volumeWritable;
            Q_EMIT q->isVolumeWritableChanged();
        }

        const bool muteWritable = readBoolProplistValue(pa_proplist_gets(info->proplist, "device.volume.write-mute-value"), true);
        if (m_muteWritable != muteWritable) {
            m_muteWritable = muteWritable;
            Q_EMIT q->isMuteWritableChanged();
        }

        if (m_muted != info->mute) {
            m_muted = info->mute;
            Q_EMIT q->mutedChanged();
        }
        if (!pa_cvolume_equal(&m_volume, &info->volume)) {
            m_volume = info->volume;
            Q_EMIT q->volumeChanged();
            Q_EMIT q->channelVolumesChanged();
        }
        QStringList infoChannels;
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
