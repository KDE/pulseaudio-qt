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

#ifndef SINKINPUT_H
#define SINKINPUT_H

#include "stream.h"

struct pa_sink_input_info;

namespace PulseAudioQt
{

class SinkInput : public Stream
{
    Q_OBJECT

public:
    explicit SinkInput(QObject *parent);
    virtual ~SinkInput();

    void setSinkIndex(quint32 sinkIndex);

    void setVolume(qint64 volume) Q_DECL_OVERRIDE;
    void setMuted(bool muted) Q_DECL_OVERRIDE;
    void setChannelVolume(int channel, qint64 volume) Q_DECL_OVERRIDE;
    void setDeviceIndex(quint32 deviceIndex) Q_DECL_OVERRIDE;

private:
    class SinkInputPrivate *const d;
    friend class MapBase<SinkInput, pa_sink_input_info>;
};

} // PulseAudioQt

#endif // SINKINPUT_H
