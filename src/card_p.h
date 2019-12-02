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
#ifndef CARD_P_H
#define CARD_P_H

#include "card.h"
#include "cardport.h"
#include "profile.h"
#include <QHash>
#include <QVector>
#include <pulse/introspect.h>

namespace PulseAudioQt
{
class CardPrivate
{
public:
    explicit CardPrivate(Card *q);
    virtual ~CardPrivate();

    Card *q;

    void update(const pa_card_info *info);

    QString m_name;
    QHash<QString, Profile *> m_profiles;
    quint32 m_activeProfileIndex;
    QHash<QString, CardPort *> m_ports;
};
}

#endif
