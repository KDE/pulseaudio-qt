#ifndef DEVICE_P_H
#define DEVICE_P_H

#include "device.h"
#include "device_p.h"
#include "volumeobject_p.h"

namespace QPulseAudio {

template <typename PAInfo>
void Device::updateDevice(const PAInfo *info)
{
    updateVolumeObject(info);

    if (m_name != info->name) {
        m_name = info->name;
        Q_EMIT nameChanged();
    }
    if (m_description != info->description) {
        m_description = info->description;
        Q_EMIT descriptionChanged();
    }
    const char *form_factor = pa_proplist_gets(info->proplist, PA_PROP_DEVICE_FORM_FACTOR);
    if (form_factor) {
        QString formFactor = QString::fromUtf8(form_factor);
        if (m_formFactor != formFactor) {
            m_formFactor = formFactor;
            Q_EMIT formFactorChanged();
        }
    }

    m_cardIndex = info->card;
    Q_EMIT cardIndexChanged();

    // TODO: this rebuilds the entire port list on every update. would be
    // nicer if it actually removed what needs removing updates what needs
    // updating and adds what needs adding. Alas, this is a tad more
    // involved.
    qDeleteAll(m_ports);
    m_ports.clear();
    for (auto **ports = info->ports; ports && *ports != nullptr; ++ports) {
        Port *port = new Port(this);
        port->setInfo(*ports);
        m_ports.append(port);
        if (info->active_port == *ports) {
            m_activePortIndex = m_ports.length() - 1;
        }
    }
    Q_EMIT portsChanged();
    Q_EMIT activePortIndexChanged();

    State infoState = stateFromPaState(info->state);
    if (infoState != m_state) {
        m_state = infoState;
        Q_EMIT stateChanged();
    }
}
}
#endif
