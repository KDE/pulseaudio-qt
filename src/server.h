/*
    Copyright 2016 David Rosca <nowrep@gmail.com>

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

#ifndef SERVER_H
#define SERVER_H

#include "kf5pulseaudioqt_export.h"
#include <QObject>

namespace PulseAudioQt
{
class Sink;
class Source;
class Context;

class KF5PULSEAUDIOQT_EXPORT Server : public QObject
{
    Q_OBJECT

public:
    ~Server();

    Sink *defaultSink() const;
    void setDefaultSink(Sink *sink);

    Source *defaultSource() const;
    void setDefaultSource(Source *source);

Q_SIGNALS:
    void defaultSinkChanged(PulseAudioQt::Sink *sink);
    void defaultSourceChanged(PulseAudioQt::Source *source);

private:
    explicit Server(Context *context);

    void reset();
    void updateDefaultDevices();

    class ServerPrivate *const d;

    friend class ServerPrivate;
    friend class Context;
    friend class ContextPrivate;
};

} // PulseAudioQt

#endif // CONTEXT_H
