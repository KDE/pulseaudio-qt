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

#include <pulse/volume.h>

#include "volumeobject.h"
#include "port.h"
#include "pulseobject.h"
#include <pulse/introspect.h>

namespace QPulseAudio
{

class KF5PULSEAUDIOQT_EXPORT Device : public VolumeObject
{
    Q_OBJECT
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
    Q_PROPERTY(QString formFactor READ formFactor NOTIFY formFactorChanged)
    Q_PROPERTY(quint32 cardIndex READ cardIndex NOTIFY cardIndexChanged)
    Q_PROPERTY(QList<QObject *> ports READ ports NOTIFY portsChanged)
    Q_PROPERTY(quint32 activePortIndex READ activePortIndex WRITE setActivePortIndex NOTIFY activePortIndexChanged)
    Q_PROPERTY(bool default READ isDefault WRITE setDefault NOTIFY defaultChanged)
public:
    enum State {
        InvalidState = 0,
        RunningState,
        IdleState,
        SuspendedState,
        UnknownState
    };
    Q_ENUMS(State);

    virtual ~Device() {}

    State state() const;
    QString name() const;
    QString description() const;
    QString formFactor() const;
    quint32 cardIndex() const;
    QList<QObject *> ports() const;
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
    Device(QObject *parent);
    template <typename PAInfo>
    void updateDevice(const PAInfo *info);

private:
    State stateFromPaState(int value) const;

    QString m_name;
    QString m_description;
    QString m_formFactor;
    quint32 m_cardIndex = -1;
    QList<QObject *> m_ports;
    quint32 m_activePortIndex = -1;
    State m_state = UnknownState;
};

} // QPulseAudio

#endif // DEVICE_H
