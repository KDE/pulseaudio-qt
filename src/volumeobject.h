/*
    SPDX-FileCopyrightText: 2014-2015 Harald Sitter <sitter@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef VOLUMEOBJECT_H
#define VOLUMEOBJECT_H

#include "pulseobject.h"

namespace PulseAudioQt
{
class KF5PULSEAUDIOQT_EXPORT VolumeObject : public PulseObject
{
    Q_OBJECT
    Q_PROPERTY(qint64 volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(bool muted READ isMuted WRITE setMuted NOTIFY mutedChanged)
    Q_PROPERTY(bool hasVolume READ hasVolume NOTIFY hasVolumeChanged)
    Q_PROPERTY(bool volumeWritable READ isVolumeWritable NOTIFY isVolumeWritableChanged)
    Q_PROPERTY(QVector<QString> channels READ channels NOTIFY channelsChanged)
    Q_PROPERTY(QVector<qreal> channelVolumes READ channelVolumes NOTIFY channelVolumesChanged)
    Q_PROPERTY(QStringList rawChannels READ rawChannels NOTIFY rawChannelsChanged)

public:
    ~VolumeObject();

    qint64 volume() const;
    virtual void setVolume(qint64 volume) = 0;

    bool isMuted() const;
    virtual void setMuted(bool muted) = 0;

    bool hasVolume() const;
    bool isVolumeWritable() const;

    QVector<QString> channels() const;
    QStringList rawChannels() const;
    QVector<qreal> channelVolumes() const;
    virtual void setChannelVolume(int channel, qint64 volume) = 0;

Q_SIGNALS:
    void volumeChanged();
    void mutedChanged();
    void hasVolumeChanged();
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
