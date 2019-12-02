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
#ifndef PULSEOBJECT_P_H
#define PULSEOBJECT_P_H

#include "debug.h"

#include <QVariantMap>

#include <pulse/introspect.h>

#include "context.h"

namespace PulseAudioQt
{
class PulseObjectPrivate
{
public:
    explicit PulseObjectPrivate(PulseObject *q);
    virtual ~PulseObjectPrivate();

    PulseObject *q;
    quint32 m_index;
    QVariantMap m_properties;

    template<typename PAInfo> void updatePulseObject(PAInfo *info)
    {
        m_index = info->index;

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
        Q_EMIT q->propertiesChanged();
    }
};
}
#endif
