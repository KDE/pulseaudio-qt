/*
    SPDX-FileCopyrightText: 2014-2015 Harald Sitter <sitter@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
    Q_PROPERTY(quint32 priority READ priority NOTIFY priorityChanged)
    Q_PROPERTY(Availability availability READ availability NOTIFY availabilityChanged)

public:
    enum Availability { Unknown, Available, Unavailable };
    Q_ENUM(Availability)

    ~Profile();

    QString description() const;
    quint32 priority() const;
    Availability availability() const;

Q_SIGNALS:
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
