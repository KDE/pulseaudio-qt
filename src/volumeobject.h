/*
    Copyright 2014-2015 Harald Sitter <sitter@kde.org>

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
