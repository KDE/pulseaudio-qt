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

#include "pulseobject.h"

struct pa_card_info;

namespace PulseAudioQt
{
class CardPort;
class Profile;

class KF5PULSEAUDIOQT_EXPORT Card : public PulseObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QVector<Profile *> profiles READ profiles NOTIFY profilesChanged)
    Q_PROPERTY(quint32 activeProfileIndex READ activeProfileIndex WRITE setActiveProfileIndex NOTIFY activeProfileIndexChanged)
    Q_PROPERTY(QVector<CardPort *> ports READ ports NOTIFY portsChanged)

public:
    ~Card();

    QString name() const;
    QVector<Profile *> profiles() const;
    quint32 activeProfileIndex() const;
    void setActiveProfileIndex(quint32 profileIndex);
    QVector<CardPort *> ports() const;

Q_SIGNALS:
    void nameChanged();
    void profilesChanged();
    void activeProfileIndexChanged();
    void portsChanged();

private:
    explicit Card(QObject *parent);

    class CardPrivate *const d;
    friend class MapBase<Card, pa_card_info>;
};

} // PulseAudioQt

#endif // CARD_H
