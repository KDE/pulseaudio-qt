#ifndef DEVICE_P_H
#define DEVICE_P_H

#include "device.h"
#include "device_p.h"
#include "volumeobject_p.h"
#include "port.h"

namespace QPulseAudio {

class DevicePrivate {
  
public:
    explicit DevicePrivate(Device* q);
    
    Device* q;
    
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
}
#endif
