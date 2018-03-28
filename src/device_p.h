/*
    Copyright 2018 Nicolas Fella <nicolas.fella@gmx.de>

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
#ifndef DEVICE_P_H
#define DEVICE_P_H

#include <pulse/proplist.h>

#include "device.h"
#include "port.h"
#include "port_p.h"

namespace PulseAudioQt
{

class DevicePrivate
{

public:
    explicit DevicePrivate(Device *q);

    Device *q;

    QString m_name;
    QString m_description;
    QString m_formFactor;
    quint32 m_cardIndex = -1;
    QList<QObject *> m_ports;
    quint32 m_activePortIndex = -1;
    Device::State m_state = Device::UnknownState;

};

template <typename PAInfo>
void Device::updateDevice(const PAInfo *info)
{
    updateVolumeObject(info);

    if (d->m_name != info->name) {
        d->m_name = info->name;
        Q_EMIT nameChanged();
    }
    if (d->m_description != info->description) {
        d->m_description = info->description;
        Q_EMIT descriptionChanged();
    }
    const char *form_factor = pa_proplist_gets(info->proplist, PA_PROP_DEVICE_FORM_FACTOR);
    if (form_factor) {
        QString formFactor = QString::fromUtf8(form_factor);
        if (d->m_formFactor != formFactor) {
            d->m_formFactor = formFactor;
            Q_EMIT formFactorChanged();
        }
    }

    d->m_cardIndex = info->card;
    Q_EMIT cardIndexChanged();

    // TODO: this rebuilds the entire port list on every update. would be
    // nicer if it actually removed what needs removing updates what needs
    // updating and adds what needs adding. Alas, this is a tad more
    // involved.
    qDeleteAll(d->m_ports);
    d->m_ports.clear();
    for (auto **ports = info->ports; ports && *ports != nullptr; ++ports) {
        Port *port = new Port(this);
        port->setInfo(*ports);
        d->m_ports.append(port);
        if (info->active_port == *ports) {
            d->m_activePortIndex = d->m_ports.length() - 1;
        }
    }
    Q_EMIT portsChanged();
    Q_EMIT activePortIndexChanged();

    State infoState = stateFromPaState(info->state);
    if (infoState != d->m_state) {
        d->m_state = infoState;
        Q_EMIT stateChanged();
    }
}

} // namespace PulseAudioQt

#endif
