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

#ifndef PA_DEVICE_H
#define PA_DEVICE_H

#include <QString>

#include "volumeobject.h"

namespace PulseAudioQt
{
class Port;
class DevicePrivate;

class KF5PULSEAUDIOQT_EXPORT Device : public VolumeObject
{
    Q_OBJECT
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
    Q_PROPERTY(QString formFactor READ formFactor NOTIFY formFactorChanged)
    Q_PROPERTY(quint32 cardIndex READ cardIndex NOTIFY cardIndexChanged)
    Q_PROPERTY(QVector<Port *> ports READ ports NOTIFY portsChanged)
    Q_PROPERTY(quint32 activePortIndex READ activePortIndex WRITE setActivePortIndex NOTIFY activePortIndexChanged)
    Q_PROPERTY(bool default READ isDefault WRITE setDefault NOTIFY defaultChanged)

public:
    enum State { InvalidState = 0, RunningState, IdleState, SuspendedState, UnknownState };
    Q_ENUM(State);

    ~Device();

    State state() const;
    QString name() const;
    QString description() const;
    QString formFactor() const;
    quint32 cardIndex() const;
    QVector<Port *> ports() const;
    quint32 activePortIndex() const;
    virtual void setActivePortIndex(quint32 port_index) = 0;
    virtual bool isDefault() const = 0;
    virtual void setDefault(bool enable) = 0;

Q_SIGNALS:
    void stateChanged();
    void nameChanged();
    void descriptionChanged();
    void formFactorChanged();
    void cardIndexChanged();
    void portsChanged();
    void activePortIndexChanged();
    void defaultChanged();

protected:
    explicit Device(QObject *parent);
    DevicePrivate *d;

private:
    friend class SinkPrivate;
    friend class SourcePrivate;
};

} // PulseAudioQt

#endif // DEVICE_H
