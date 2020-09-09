/*
    SPDX-FileCopyrightText: 2018 Nicolas Fella <nicolas.fella@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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

    template<typename PAInfo> void setInfo(const PAInfo *info)
    {
        setCommonInfo(info, info->available ? Profile::Available : Profile::Unavailable);
    }

    template<typename PAInfo> void setCommonInfo(const PAInfo *info, Profile::Availability newAvailability)
    {
        // Description is optional. Name not so much as we need some ID.
        Q_ASSERT(info->name);
        QString infoName = QString::fromUtf8(info->name);
        if (m_name != infoName) {
            m_name = infoName;
            Q_EMIT q->nameChanged();
        }
        if (info->description) {
            QString infoDescription = QString::fromUtf8(info->description);
            if (m_description != infoDescription) {
                m_description = infoDescription;
                Q_EMIT q->descriptionChanged();
            }
        }
        if (m_priority != info->priority) {
            m_priority = info->priority;
            Q_EMIT q->priorityChanged();
        }

        if (m_availability != newAvailability) {
            m_availability = newAvailability;
            Q_EMIT q->availabilityChanged();
        }
    }
};
}
