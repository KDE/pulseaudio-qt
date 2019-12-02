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

#ifndef STREAM_H
#define STREAM_H

#include <QString>

#include "volumeobject.h"

// Properties need fully qualified classes even with pointers.
#include "client.h"

namespace PulseAudioQt
{
class StreamPrivate;

class KF5PULSEAUDIOQT_EXPORT Stream : public VolumeObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(PulseAudioQt::Client *client READ client NOTIFY clientChanged)
    Q_PROPERTY(bool virtualStream READ isVirtualStream NOTIFY virtualStreamChanged)
    Q_PROPERTY(quint32 deviceIndex READ deviceIndex WRITE setDeviceIndex NOTIFY deviceIndexChanged)
    Q_PROPERTY(bool corked READ isCorked NOTIFY corkedChanged)

public:
    ~Stream();

    QString name() const;
    Client *client() const;
    bool isVirtualStream() const;
    quint32 deviceIndex() const;
    bool isCorked() const;

    virtual void setDeviceIndex(quint32 deviceIndex) = 0;

Q_SIGNALS:
    void nameChanged();
    void clientChanged();
    void virtualStreamChanged();
    void deviceIndexChanged();
    void corkedChanged();

protected:
    explicit Stream(QObject *parent);

    class StreamPrivate *const d;

    friend class SinkInputPrivate;
    friend class SourceOutputPrivate;
};

} // PulseAudioQt

#endif // STREAM_H
