/*
    SPDX-FileCopyrightText: 2014-2015 Harald Sitter <sitter@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef PROFILE_H
#define PROFILE_H

#include "pulseaudioqt_export.h"
#include "pulseobject.h"
#include <QObject>
#include <QString>

namespace PulseAudioQt
{
/**
 * A PulseAudio profile.
 */
class PULSEAUDIOQT_EXPORT Profile : public PulseObject
{
    Q_OBJECT
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
    Q_PROPERTY(quint32 priority READ priority NOTIFY priorityChanged)
    Q_PROPERTY(Availability availability READ availability NOTIFY availabilityChanged)

public:
    enum Availability { Unknown, Available, Unavailable };
    Q_ENUM(Availability)

    ~Profile();

    /**
     * A human readable description.
     */
    QString description() const;

    /**
     * This object's priority. A higher number means higher priority.
     */
    quint32 priority() const;

    /**
     * Whether this object is available.
     */
    Availability availability() const;

Q_SIGNALS:
    /**
     * Emitted when the description changed.
     */
    void descriptionChanged();

    /**
     * Emitted when the priority changed.
     */
    void priorityChanged();

    /**
     * Emitted when the availability changed.
     */
    void availabilityChanged();

protected:
    /** @private */
    explicit Profile(QObject *parent);
    /** @private */
    class ProfilePrivate *const d;

    friend class Device;
    friend class CardPrivate;
    friend class PortPrivate;
};

} // PulseAudioQt

#endif // PROFILE_H
