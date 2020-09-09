/*
    SPDX-FileCopyrightText: 2018 Nicolas Fella <nicolas.fella@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
