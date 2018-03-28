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

#ifndef SOURCE_H
#define SOURCE_H

#include "device.h"
#include <pulse/introspect.h>

namespace QPulseAudio
{

class SourcePrivate;

class KF5PULSEAUDIOQT_EXPORT Source : public Device
{
    Q_OBJECT
public:
    Source(QObject *parent);
    virtual ~Source();

    void setVolume(qint64 volume) Q_DECL_OVERRIDE;
    void setMuted(bool muted) Q_DECL_OVERRIDE;
    void setActivePortIndex(quint32 port_index) Q_DECL_OVERRIDE;
    void setChannelVolume(int channel, qint64 volume) Q_DECL_OVERRIDE;

    bool isDefault() const Q_DECL_OVERRIDE;
    void setDefault(bool enable) Q_DECL_OVERRIDE;

    SourcePrivate* d;

    // TODO move to private
    void update(const pa_source_info *info);

};

} // QPulseAudio

#endif // SOURCE_H
