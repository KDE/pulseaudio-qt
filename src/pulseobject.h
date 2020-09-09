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

class KF5PULSEAUDIOQT_EXPORT PulseObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(quint32 index READ index CONSTANT)
    Q_PROPERTY(QString iconName READ iconName CONSTANT)
    Q_PROPERTY(QVariantMap properties READ properties NOTIFY propertiesChanged)

public:
    ~PulseObject();

    quint32 index() const;
    QString iconName() const;
    QVariantMap properties() const;

    Context *context() const;

Q_SIGNALS:
    void propertiesChanged();

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
