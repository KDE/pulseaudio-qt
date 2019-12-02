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

#include "device.h"
#include "device_p.h"

namespace PulseAudioQt
{
Device::State Device::state() const
{
    return d->m_state;
}

QString Device::name() const
{
    return d->m_name;
}

QString Device::description() const
{
    return d->m_description;
}

QString Device::formFactor() const
{
    return d->m_formFactor;
}

quint32 Device::cardIndex() const
{
    return d->m_cardIndex;
}

QVector<Port *> Device::ports() const
{
    return QVector<Port *>::fromList(d->m_ports.values());
}

quint32 Device::activePortIndex() const
{
    return d->m_activePortIndex;
}

Device::Device(QObject *parent)
    : VolumeObject(parent)
    , d(new DevicePrivate(this))
{
}

DevicePrivate::DevicePrivate(Device *q)
    : q(q)
{
}

Device::State DevicePrivate::stateFromPaState(int value) const
{
    switch (value) {
    case -1: // PA_X_INVALID_STATE
        return Device::InvalidState;
    case 0: // PA_X_RUNNING
        return Device::RunningState;
    case 1: // PA_X_IDLE
        return Device::IdleState;
    case 2: // PA_X_SUSPENDED
        return Device::SuspendedState;
    default:
        return Device::UnknownState;
    }
}

Device::~Device()
{
    delete d;
}

} // namespace PulseAudioQt
