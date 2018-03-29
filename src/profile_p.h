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

#pragma once

#include "profile.h"

namespace PulseAudioQt
{


class ProfilePrivate
{
public:
    explicit ProfilePrivate(Profile *q);
    virtual ~ProfilePrivate();

    Profile *q;

    QString m_name;
    QString m_description;
    quint32 m_priority;
    Profile::Availability m_availability;
};

template<typename PAInfo>
void Profile::setInfo(const PAInfo *info)
{
    // Description is optional. Name not so much as we need some ID.
    Q_ASSERT(info->name);
    QString infoName = QString::fromUtf8(info->name);
    if (d->m_name != infoName) {
        d->m_name = infoName;
        Q_EMIT nameChanged();
    }
    if (info->description) {
        QString infoDescription = QString::fromUtf8(info->description);
        if (d->m_description != infoDescription) {
            d->m_description = infoDescription;
            Q_EMIT descriptionChanged();
        }
    }
    if (d->m_priority != info->priority) {
        d->m_priority = info->priority;
        Q_EMIT priorityChanged();
    }

    Availability newAvailability = info->available ? Available : Unavailable;
    if (d->m_availability != newAvailability) {
        d->m_availability = newAvailability;
        Q_EMIT availabilityChanged();
    }
}
}