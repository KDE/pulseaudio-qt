/*
    SPDX-FileCopyrightText: 2014-2015 Harald Sitter <sitter@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef PA_DEVICE_H
#define PA_DEVICE_H

#include <QString>

#include "volumeobject.h"

namespace PulseAudioQt
{
class Port;
class DevicePrivate;

class KF5PULSEAUDIOQT_EXPORT Device : public VolumeObject
{
    Q_OBJECT
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
    Q_PROPERTY(QString formFactor READ formFactor NOTIFY formFactorChanged)
    Q_PROPERTY(quint32 cardIndex READ cardIndex NOTIFY cardIndexChanged)
    Q_PROPERTY(QVector<Port *> ports READ ports NOTIFY portsChanged)
    Q_PROPERTY(quint32 activePortIndex READ activePortIndex WRITE setActivePortIndex NOTIFY activePortIndexChanged)
    Q_PROPERTY(bool default READ isDefault WRITE setDefault NOTIFY defaultChanged)

public:
    enum State { InvalidState = 0, RunningState, IdleState, SuspendedState, UnknownState };
    Q_ENUM(State);

    ~Device();

    State state() const;
    QString name() const;
    QString description() const;
    QString formFactor() const;
    quint32 cardIndex() const;
    QVector<Port *> ports() const;
    quint32 activePortIndex() const;
    virtual void setActivePortIndex(quint32 port_index) = 0;
    virtual bool isDefault() const = 0;
    virtual void setDefault(bool enable) = 0;

Q_SIGNALS:
    void stateChanged();
    void nameChanged();
    void descriptionChanged();
    void formFactorChanged();
    void cardIndexChanged();
    void portsChanged();
    void activePortIndexChanged();
    void defaultChanged();

protected:
    explicit Device(QObject *parent);
    DevicePrivate *d;

private:
    friend class SinkPrivate;
    friend class SourcePrivate;
};

} // PulseAudioQt

#endif // DEVICE_H
