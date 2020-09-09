/*
    SPDX-FileCopyrightText: 2014-2015 Harald Sitter <sitter@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "client.h"
#include "client_p.h"

#include "debug.h"
#include "pulseobject_p.h"

namespace PulseAudioQt
{
Client::Client(QObject *parent)
    : PulseObject(parent)
    , d(new ClientPrivate(this))
{
}

ClientPrivate::ClientPrivate(Client *q)
    : q(q)
{
}

Client::~Client()
{
    delete d;
}

void ClientPrivate::update(const pa_client_info *info)
{
    q->PulseObject::d->updatePulseObject(info);

    QString infoName = QString::fromUtf8(info->name);
    if (m_name != infoName) {
        m_name = infoName;
        Q_EMIT q->nameChanged();
    }
}

QString Client::name() const
{
    return d->m_name;
}

} // PulseAudioQt
