/*
    SPDX-FileCopyrightText: 2018 Nicolas Fella <nicolas.fella@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/
#ifndef CARDPORT_H
#define CARDPORT_H

#include "port.h"

#include "debug.h"
#include <QObject>
#include <pulse/introspect.h>

namespace PulseAudioQt
{
class KF5PULSEAUDIOQT_EXPORT CardPort : public Port
{
    Q_OBJECT
    Q_PROPERTY(QVariantMap properties READ properties NOTIFY propertiesChanged)

public:
    ~CardPort();

    void update(const pa_card_port_info *info);

    QVariantMap properties() const;

private:
    explicit CardPort(QObject *parent);

    QVariantMap m_properties;

    friend class CardPrivate;
};

} // PulseAudioQt

#endif
