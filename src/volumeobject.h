/*
    SPDX-FileCopyrightText: 2014-2015 Harald Sitter <sitter@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef VOLUMEOBJECT_H
#define VOLUMEOBJECT_H

#include "indexedpulseobject.h"

namespace PulseAudioQt
{
/**
 * An PulseObject that has a volume. Can be a Device or a Stream.
 */
class PULSEAUDIOQT_EXPORT VolumeObject : public IndexedPulseObject
{
    Q_OBJECT
    Q_PROPERTY(qint64 volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(bool muted READ isMuted WRITE setMuted NOTIFY mutedChanged)
    Q_PROPERTY(bool volumeWritable READ isVolumeWritable NOTIFY isVolumeWritableChanged)
    Q_PROPERTY(QVector<QString> channels READ channels NOTIFY channelsChanged)
    Q_PROPERTY(QVector<qreal> channelVolumes READ channelVolumes NOTIFY channelVolumesChanged)
    Q_PROPERTY(QStringList rawChannels READ rawChannels NOTIFY rawChannelsChanged)

public:
    ~VolumeObject();

    /**
     * This device's volume
     */
    qint64 volume() const;

    /**
     * Set this device's volume
     */
    virtual void setVolume(qint64 volume) = 0;

    /**
     * Whether this device is muted.
     */
    bool isMuted() const;

    /**
     * Set whether this device is muted.
     */
    virtual void setMuted(bool muted) = 0;

    bool isVolumeWritable() const;

    QVector<QString> channels() const;
    QStringList rawChannels() const;
    QVector<qreal> channelVolumes() const;
    virtual void setChannelVolume(int channel, qint64 volume) = 0;

Q_SIGNALS:
    void volumeChanged();
    void mutedChanged();
    void isVolumeWritableChanged();
    void channelsChanged();
    void rawChannelsChanged();
    void channelVolumesChanged();

protected:
    explicit VolumeObject(QObject *parent);

    class VolumeObjectPrivate *const d;
    friend class DevicePrivate;
    friend class StreamPrivate;
};

} // PulseAudioQt

#endif // VOLUMEOBJECT_H
