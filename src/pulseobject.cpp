/*
    SPDX-FileCopyrightText: 2014-2015 Harald Sitter <sitter@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "pulseobject.h"
#include "pulseobject_p.h"

#include "context.h"

#include <QIcon>
#include <QMetaProperty>

using namespace Qt::StringLiterals;

namespace PulseAudioQt
{
PulseObject::PulseObject(QObject *parent)
    : QObject(parent)
    , d(new PulseObjectPrivate(this))
{
}

PulseObject::~PulseObject()
{
}

PulseObjectPrivate::PulseObjectPrivate(PulseObject *q)
    : q(q)
{
}

PulseObjectPrivate::~PulseObjectPrivate()
{
}

QString PulseObject::name() const
{
    return d->m_name;
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

    name = d->m_properties.value(QStringLiteral("pipewire.access.portal.app_id")).toString();
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

} // PulseAudioQt

QDebug operator<<(QDebug dbg, PulseAudioQt::PulseObjectPrivate const *d)
{
    QDebugStateSaver saver(dbg);

    auto mo = d->q->metaObject();

    QStringList superClasses;
    auto superClass = mo;
    while ((superClass = superClass->superClass())) {
        superClasses.append(QString::fromLatin1(superClass->className()));
    }

    dbg.nospace().noquote() << '\n' << mo->className() << '{' << superClasses.join(u", "_s) << '}';
    dbg.nospace() << "(\n";

    auto indent = QLatin1String("    ");

    for (int i = 0; i < mo->propertyCount(); i++) {
        auto property = mo->property(i);
        auto propertyValue = property.read(d->q);
        if (propertyValue.canConvert<QVariantMap>()) {
            dbg.nospace() << indent << property.name() << ":\n";
            const auto properties = propertyValue.value<QVariantMap>();
            for (auto it = properties.cbegin(); it != properties.cend(); ++it) {
                auto &key = it.key();
                auto &value = it.value();
                dbg.nospace() << indent << indent << key << ": " << value << '\n';
            }
        } else {
            dbg.nospace() << indent << property.name() << ": " << propertyValue << '\n';
        }
    }

    dbg.nospace() << ')';

    return dbg;
}
