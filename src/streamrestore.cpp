/*
    Copyright 2016 David Rosca <nowrep@gmail.com>

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

#include "streamrestore.h"
#include "context.h"
#include "context_p.h"
#include "streamrestore_p.h"

#include "debug.h"
#include "pulseobject_p.h"

namespace PulseAudioQt
{
StreamRestore::StreamRestore(quint32 index, const QVariantMap &properties, QObject *parent)
    : PulseObject(parent)
    , d(new StreamRestorePrivate(this))
{
    memset(&d->m_volume, 0, sizeof(d->m_volume));
    memset(&d->m_channelMap, 0, sizeof(d->m_channelMap));

    d->m_muted = false;
    PulseObject::d->m_index = index;
    PulseObject::d->m_properties = properties;
}

StreamRestore::~StreamRestore()
{
}

StreamRestorePrivate::StreamRestorePrivate(StreamRestore *q)
    : q(q)
{
}

StreamRestorePrivate::~StreamRestorePrivate()
{
}

void StreamRestorePrivate::update(const pa_ext_stream_restore_info *info)
{
    m_cache.valid = false;
    const QString infoName = QString::fromUtf8(info->name);
    if (m_name != infoName) {
        m_name = infoName;
        Q_EMIT q->nameChanged();
    }
    const QString infoDevice = QString::fromUtf8(info->device);
    if (m_device != infoDevice) {
        m_device = infoDevice;
        Q_EMIT q->deviceChanged();
    }
    if (m_muted != info->mute) {
        m_muted = info->mute;
        Q_EMIT q->mutedChanged();
    }
    if (memcmp(&m_volume, &info->volume, sizeof(pa_cvolume)) != 0) {
        m_volume = info->volume;
        Q_EMIT q->volumeChanged();
        Q_EMIT q->channelVolumesChanged();
    }
    if (memcmp(&m_channelMap, &info->channel_map, sizeof(pa_channel_map)) != 0) {
        m_channels.clear();
        m_channels.reserve(info->channel_map.channels);
        for (int i = 0; i < info->channel_map.channels; ++i) {
            m_channels << QString::fromUtf8(pa_channel_position_to_pretty_string(info->channel_map.map[i]));
        }
        m_channelMap = info->channel_map;
        Q_EMIT q->channelsChanged();
    }
}

QString StreamRestore::name() const
{
    return d->m_name;
}

QString StreamRestore::device() const
{
    return d->m_device;
}

void StreamRestore::setDevice(const QString &device)
{
    if (d->m_cache.valid) {
        if (d->m_cache.device != device) {
            d->writeChanges(d->m_cache.volume, d->m_cache.muted, device);
        }
    } else {
        if (d->m_device != device) {
            d->writeChanges(d->m_volume, d->m_muted, device);
        }
    }
}

qint64 StreamRestore::volume() const
{
    return d->m_volume.values[0];
}

void StreamRestore::setVolume(qint64 volume)
{
    pa_cvolume vol = d->m_cache.valid ? d->m_cache.volume : d->m_volume;
    for (int i = 0; i < vol.channels; ++i) {
        vol.values[i] = volume;
    }

    if (d->m_cache.valid) {
        d->writeChanges(vol, d->m_cache.muted, d->m_cache.device);
    } else {
        d->writeChanges(vol, d->m_muted, d->m_device);
    }
}

bool StreamRestore::isMuted() const
{
    return d->m_muted;
}

void StreamRestore::setMuted(bool muted)
{
    if (d->m_cache.valid) {
        if (d->m_cache.muted != muted) {
            d->writeChanges(d->m_cache.volume, muted, d->m_cache.device);
        }
    } else {
        if (d->m_muted != muted) {
            d->writeChanges(d->m_volume, muted, d->m_device);
        }
    }
}

bool StreamRestore::hasVolume() const
{
    return true;
}

bool StreamRestore::isVolumeWritable() const
{
    return true;
}

QVector<QString> StreamRestore::channels() const
{
    return d->m_channels;
}

QVector<qreal> StreamRestore::channelVolumes() const
{
    QVector<qreal> ret;
    ret.reserve(d->m_volume.channels);
    for (int i = 0; i < d->m_volume.channels; ++i) {
        ret << d->m_volume.values[i];
    }
    return ret;
}

void StreamRestore::setChannelVolume(int channel, qint64 volume)
{
    Q_ASSERT(channel >= 0 && channel < d->m_volume.channels);
    pa_cvolume vol = d->m_cache.valid ? d->m_cache.volume : d->m_volume;
    vol.values[channel] = volume;

    if (d->m_cache.valid) {
        d->writeChanges(vol, d->m_cache.muted, d->m_cache.device);
    } else {
        d->writeChanges(vol, d->m_muted, d->m_device);
    }
}

quint32 StreamRestore::deviceIndex() const
{
    return PA_INVALID_INDEX;
}

void StreamRestore::setDeviceIndex(quint32 deviceIndex)
{
    Q_UNUSED(deviceIndex);
    qCWarning(PULSEAUDIOQT) << "Not implemented";
}

void StreamRestorePrivate::writeChanges(const pa_cvolume &volume, bool muted, const QString &device)
{
    const QByteArray nameData = m_name.toUtf8();
    const QByteArray deviceData = device.toUtf8();

    pa_ext_stream_restore_info info;
    info.name = nameData.constData();
    info.channel_map = m_channelMap;
    info.volume = volume;
    info.device = deviceData.isEmpty() ? nullptr : deviceData.constData();
    info.mute = muted;

    m_cache.valid = true;
    m_cache.volume = volume;
    m_cache.muted = muted;
    m_cache.device = device;

    q->context()->d->streamRestoreWrite(&info);
}

} // PulseAudioQt
