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

#include "card.h"
#include "card_p.h"
#include "debug.h"

#include "cardport.h"
#include "context.h"
#include "profile_p.h"
#include "pulseobject.h"
#include "pulseobject_p.h"

namespace PulseAudioQt
{
Card::Card(QObject *parent)
    : PulseObject(parent)
    , d(new CardPrivate(this))
{
}

Card::~Card()
{
    delete d;
}

CardPrivate::CardPrivate(Card *q)
    : q(q)
{
}

CardPrivate::~CardPrivate()
{
}

void CardPrivate::update(const pa_card_info *info)
{
    q->PulseObject::d->updatePulseObject(info);

    QString infoName = QString::fromUtf8(info->name);
    if (m_name != infoName) {
        m_name = infoName;
        Q_EMIT q->nameChanged();
    }

    QStringList newProfiles;
    for (auto **it = info->profiles2; it && *it != nullptr; ++it) {
        const QString name = QString::fromUtf8((*it)->name);
        newProfiles << name;
        if (!m_profiles.contains(name)) {
            m_profiles[name] = new Profile(q);
        }
        Profile *profile = m_profiles[name];
        profile->d->setInfo(*it);
        if (info->active_profile2 == *it) {
            m_activeProfileIndex = m_profiles.size() - 1;
        }
    }

    const QList<QString> profileKeys = m_profiles.keys();
    for (const QString &profileKey : profileKeys) {
        if (!newProfiles.contains(profileKey)) {
            delete m_profiles.take(profileKey);
        }
    }

    Q_EMIT q->profilesChanged();
    Q_EMIT q->activeProfileIndexChanged();

    QStringList newPorts;
    for (auto **it = info->ports; it && *it != nullptr; ++it) {
        const QString name = QString::fromUtf8((*it)->name);
        newPorts << name;
        if (!m_ports.contains(name)) {
            m_ports[name] = new CardPort(q);
        }
        CardPort *port = m_ports[name];
        port->update(*it);
    }

    const QList<QString> portKeys = m_ports.keys();
    for (const QString &portKey : profileKeys) {
        if (!newPorts.contains(portKey)) {
            delete m_ports.take(portKey);
        }
    }

    Q_EMIT q->portsChanged();
}

QString Card::name() const
{
    return d->m_name;
}

QVector<Profile *> Card::profiles() const
{
    return QVector<Profile *>::fromList(d->m_profiles.values());
}

quint32 Card::activeProfileIndex() const
{
    return d->m_activeProfileIndex;
}

void Card::setActiveProfileIndex(quint32 profileIndex)
{
    const Profile *profile = qobject_cast<Profile *>(profiles().at(profileIndex));
    context()->setCardProfile(index(), profile->name());
}

QVector<CardPort *> Card::ports() const
{
    return QVector<CardPort *>::fromList(d->m_ports.values());
}

} // PulseAudioQt
