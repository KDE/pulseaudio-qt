/*
    SPDX-FileCopyrightText: 2018 Nicolas Fella <nicolas.fella@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/
#include "cardport.h"
#include "port_p.h"

namespace PulseAudioQt
{
CardPort::CardPort(QObject *parent)
    : Port(parent)
{
}

CardPort::~CardPort()
{
}

void CardPort::update(const pa_card_port_info *info)
{
    Port::d->setInfo(info);

    m_properties.clear();
    void *it = nullptr;

    while (const char *key = pa_proplist_iterate(info->proplist, &it)) {
        Q_ASSERT(key);
        const char *value = pa_proplist_gets(info->proplist, key);
        if (!value) {
            qCDebug(PULSEAUDIOQT) << "property" << key << "not a string";
            continue;
        }
        Q_ASSERT(value);
        m_properties.insert(QString::fromUtf8(key), QString::fromUtf8(value));
    }
    Q_EMIT propertiesChanged();
}

QVariantMap CardPort::properties() const
{
    return m_properties;
}

} // PulseAudioQt
