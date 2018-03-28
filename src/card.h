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

#ifndef CARD_H
#define CARD_H

#include <QList>

#include "profile.h"
#include "port.h"
#include "pulseobject.h"
#include <pulse/introspect.h>

namespace QPulseAudio
{
class CardPrivate;

class Card : public PulseObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QList<QObject *> profiles READ profiles  NOTIFY profilesChanged)
    Q_PROPERTY(quint32 activeProfileIndex READ activeProfileIndex WRITE setActiveProfileIndex NOTIFY activeProfileIndexChanged)
    Q_PROPERTY(QList<QObject *> ports READ ports NOTIFY portsChanged)
public:
    Card(QObject *parent);
    virtual ~Card();

    QString name() const;
    QList<QObject *> profiles() const;
    quint32 activeProfileIndex() const;
    void setActiveProfileIndex(quint32 profileIndex);
    QList<QObject *> ports() const;

    CardPrivate* d;

    //TODO move to private
    void update(const pa_card_info *info);

Q_SIGNALS:
    void nameChanged();
    void profilesChanged();
    void activeProfileIndexChanged();
    void portsChanged();
};

} // QPulseAudio

#endif // CARD_H
