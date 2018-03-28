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

#ifndef CONTEXT_H
#define CONTEXT_H

#include <QMutex>
#include <QObject>
#include <QSet>
#include "maps.h"
#include "maps_dptr.h"
#include "kf5pulseaudioqt_export.h"

namespace PulseAudioQt
{

class Server;
class ContextPrivate;

class KF5PULSEAUDIOQT_EXPORT Context : public QObject
{
    Q_OBJECT


public:
    Context(QObject *parent = nullptr);
    ~Context();

    static Context *instance();

    static const qint64 NormalVolume = PA_VOLUME_NORM;
    static const qint64 MinimalVolume = 0;
    static const qint64 MaximalVolume = (PA_VOLUME_NORM / 100.0) * 150;

    void ref();
    void unref();

    bool isValid();

    Server *server() const;

    void setCardProfile(quint32 index, const QString &profile);
    void setDefaultSink(const QString &name);
    void setDefaultSource(const QString &name);

    ContextPrivate* d;
private:
    static Context* s_context;
};

} // PulseAudioQt

#endif // CONTEXT_H
