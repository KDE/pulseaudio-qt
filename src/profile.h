/*
    Copyright 2014-2015 Harald Sitter <sitter@kde.org>

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

#ifndef PROFILE_H
#define PROFILE_H

#include "kf5pulseaudioqt_export.h"
#include "pulseobject.h"
#include <QObject>
#include <QString>

namespace PulseAudioQt
{
class KF5PULSEAUDIOQT_EXPORT Profile : public PulseObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
    Q_PROPERTY(quint32 priority READ priority NOTIFY priorityChanged)
    Q_PROPERTY(Availability availability READ availability NOTIFY availabilityChanged)

public:
    enum Availability { Unknown, Available, Unavailable };
    Q_ENUM(Availability)

    ~Profile();

    QString name() const;
    QString description() const;
    quint32 priority() const;
    Availability availability() const;

Q_SIGNALS:
    void nameChanged();
    void descriptionChanged();
    void priorityChanged();
    void availabilityChanged();

protected:
    explicit Profile(QObject *parent);
    class ProfilePrivate *const d;

    friend class Device;
    friend class CardPrivate;
    friend class PortPrivate;
};

} // PulseAudioQt

#endif // PROFILE_H
