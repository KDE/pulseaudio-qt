/*
    SPDX-FileCopyrightText: 2014-2015 Harald Sitter <sitter@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "card.h"
#include "card_p.h"
#include "debug.h"

#include "cardport.h"
#include "context.h"
#include "profile_p.h"
#include "indexedpulseobject_p.h"
#include "port_p.h"

namespace PulseAudioQt
{
Card::Card(QObject *parent)
    : IndexedPulseObject(parent)
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
    q->IndexedPulseObject::d->updatePulseObject(info);
    q->PulseObject::d->updateProperties(info);

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
        port->d->setInfo(*it);
    }

    const QList<QString> portKeys = m_ports.keys();
    for (const QString &portKey : profileKeys) {
        if (!newPorts.contains(portKey)) {
            delete m_ports.take(portKey);
        }
    }

    Q_EMIT q->portsChanged();
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
    Context::instance()->setCardProfile(index(), profile->name());
}

QVector<CardPort *> Card::ports() const
{
    return QVector<CardPort *>::fromList(d->m_ports.values());
}

} // PulseAudioQt
