/*
    SPDX-FileCopyrightText: 2018 Nicolas Fella <nicolas.fella@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#pragma once

#include "port.h"
#include "profile_p.h"
#include <pulse/def.h>

namespace PulseAudioQt
{
class PortPrivate
{
public:
    explicit PortPrivate(Port *q);
    virtual ~PortPrivate();

    Port *q;

    template<typename PAInfo> void setInfo(const PAInfo *info)
    {
        Profile::Availability newAvailability;
        switch (info->available) {
        case PA_PORT_AVAILABLE_NO:
            newAvailability = Profile::Unavailable;
            break;
        case PA_PORT_AVAILABLE_YES:
            newAvailability = Profile::Available;
            break;
        default:
            newAvailability = Profile::Unknown;
        }
        q->Profile::d->setCommonInfo(info, newAvailability);
    }
};
}
