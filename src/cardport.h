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
#ifndef CARDPORT_H
#define CARDPORT_H

#include "port.h"

#include <QObject>
#include "debug.h"
#include <pulse/introspect.h>

namespace PulseAudioQt
{

class CardPort : public Port
{
    Q_OBJECT
    Q_PROPERTY(QVariantMap properties READ properties NOTIFY propertiesChanged)
public:
    explicit CardPort(QObject *parent = nullptr);
    virtual ~CardPort() {}

//    int direction;                      /**< A #pa_direction enum, indicating the direction of this port. */
//    uint32_t n_profiles;                /**< Number of entries in profile array */
//    pa_card_profile_info** profiles;    /**< \deprecated Superseded by profiles2 */
//    int64_t latency_offset;             /**< Latency offset of the port that gets added to the sink/source latency when the port is active. \since 3.0 */
//    pa_card_profile_info2** profiles2;  /**< Array of pointers to available profiles, or NULL. Array is terminated by an entry set to NULL. \since 5.0 */

    void update(const pa_card_port_info *info);

    QVariantMap properties() const;

Q_SIGNALS:
    void propertiesChanged();

private:
    QVariantMap m_properties;
};

} // PulseAudioQt

#endif
