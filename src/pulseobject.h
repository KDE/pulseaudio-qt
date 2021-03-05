/*
    SPDX-FileCopyrightText: 2014-2015 Harald Sitter <sitter@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef PULSEOBJECT_H
#define PULSEOBJECT_H

#include <QObject>

#include "kf5pulseaudioqt_export.h"

namespace PulseAudioQt
{
class Context;

template<typename Type, typename PAInfo> class MapBase;

/**
 * Base class for most PulseAudio objects.
 */
class KF5PULSEAUDIOQT_EXPORT PulseObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(quint32 index READ index CONSTANT)
    Q_PROPERTY(QString iconName READ iconName CONSTANT)
    Q_PROPERTY(QVariantMap properties READ properties NOTIFY propertiesChanged)

public:
    ~PulseObject();

    QString name() const;

    /**
     * Index of this object.
     */
    quint32 index() const;

    /**
     * A freedesktop.org icon name that fits this object.
     */
    QString iconName() const;

    /**
     * A map of properties associated with this object.
     * The set of available properties depends on the type of object.
     */
    QVariantMap properties() const;

Q_SIGNALS:
    /**
     * Emitted when any of the \ref properties changed.
     */
    void propertiesChanged();

    void nameChanged();

protected:
    explicit PulseObject(QObject *parent);

    class PulseObjectPrivate *const d;

private:
    // Ensure that we get properly parented.
    PulseObject();
    friend class ClientPrivate;
    friend class CardPrivate;
    friend class ModulePrivate;
    friend class VolumeObjectPrivate;
};

} // PulseAudioQt

#endif // PULSEOBJECT_H
