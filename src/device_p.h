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

#include <QHash>
#include <QVector>

#include "device.h"
#include "port.h"
#include "port_p.h"
#include "volumeobject_p.h"

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
    QHash<QString, Port *> m_ports;
    quint32 m_activePortIndex = -1;
    Device::State m_state = Device::UnknownState;

    Device::State stateFromPaState(int value) const;

    template<typename PAInfo> void updateDevice(const PAInfo *info)
    {
        q->VolumeObject::d->updateVolumeObject(info);

        if (m_name != info->name) {
            m_name = info->name;
            Q_EMIT q->nameChanged();
        }
        if (m_description != info->description) {
            m_description = info->description;
            Q_EMIT q->descriptionChanged();
        }
        const char *form_factor = pa_proplist_gets(info->proplist, PA_PROP_DEVICE_FORM_FACTOR);
        if (form_factor) {
            QString formFactor = QString::fromUtf8(form_factor);
            if (m_formFactor != formFactor) {
                m_formFactor = formFactor;
                Q_EMIT q->formFactorChanged();
            }
        }

        m_cardIndex = info->card;
        Q_EMIT q->cardIndexChanged();

        QStringList newPorts;
        for (auto **it = info->ports; it && *it != nullptr; ++it) {
            const QString name = QString::fromUtf8((*it)->name);
            newPorts << name;
            if (!m_ports.contains(name)) {
                m_ports[name] = new Port(q);
            }
            Port *port = m_ports[name];
            port->d->setInfo(*it);
            if (info->active_port == *it) {
                m_activePortIndex = m_ports.size() - 1;
            }
        }

        const QList<QString> keys = m_ports.keys();
        for (const QString &portKey : keys) {
            if (!newPorts.contains(portKey)) {
                delete m_ports.take(portKey);
            }
        }

        Q_EMIT q->portsChanged();
        Q_EMIT q->activePortIndexChanged();

        Device::State infoState = stateFromPaState(info->state);
        if (infoState != m_state) {
            m_state = infoState;
            Q_EMIT q->stateChanged();
        }
    }
};

} // namespace PulseAudioQt

#endif
