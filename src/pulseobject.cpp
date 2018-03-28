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

#include "pulseobject.h"
#include "pulseobject_p.h"

#include "context.h"

#include <QIcon>

namespace QPulseAudio
{

PulseObject::PulseObject(QObject *parent)
    : QObject(parent)
    , d(new PulseObjectPrivate(this))
{
}

PulseObjectPrivate::PulseObjectPrivate(PulseObject* q)
    : q(q)
    , m_index(0)
{
}

PulseObject::~PulseObject()
{
    delete d;
}

Context *PulseObject::context() const
{
    return Context::instance();
}

uint32_t PulseObject::index() const
{
    return d->m_index;
}

QString PulseObject::iconName() const
{
    QString name = d->m_properties.value(QStringLiteral("device.icon_name")).toString();
    if (!name.isEmpty() && QIcon::hasThemeIcon(name)) {
        return name;
    }

    name = d->m_properties.value(QStringLiteral("media.icon_name")).toString();
    if (!name.isEmpty() && QIcon::hasThemeIcon(name)) {
        return name;
    }

    name = d->m_properties.value(QStringLiteral("window.icon_name")).toString();
    if (!name.isEmpty() && QIcon::hasThemeIcon(name)) {
        return name;
    }

    name = d->m_properties.value(QStringLiteral("application.icon_name")).toString();
    if (!name.isEmpty() && QIcon::hasThemeIcon(name)) {
        return name;
    }

    name = d->m_properties.value(QStringLiteral("application.process.binary")).toString();
    if (!name.isEmpty() && QIcon::hasThemeIcon(name)) {
        return name;
    }

    name = d->m_properties.value(QStringLiteral("application.name")).toString();
    if (!name.isEmpty() && QIcon::hasThemeIcon(name)) {
        return name;
    }

    name = property("name").toString();
    if (!name.isEmpty() && QIcon::hasThemeIcon(name)) {
        return name;
    }

    return QString();
}

QVariantMap PulseObject::properties() const
{
    return d->m_properties;
}

} // QPulseAudio
