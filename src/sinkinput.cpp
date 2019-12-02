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

#include "sinkinput.h"
#include "sinkinput_p.h"

#include "context.h"
#include "context_p.h"
#include "stream_p.h"

namespace PulseAudioQt
{
SinkInput::SinkInput(QObject *parent)
    : Stream(parent)
    , d(new SinkInputPrivate(this))
{
}

SinkInputPrivate::SinkInputPrivate(SinkInput *q)
    : q(q)
{
}

SinkInput::~SinkInput()
{
    delete d;
}

void SinkInputPrivate::update(const pa_sink_input_info *info)
{
    q->Stream::d->updateStream(info);
    if (q->Stream::d->m_deviceIndex != info->sink) {
        q->Stream::d->m_deviceIndex = info->sink;
        Q_EMIT q->deviceIndexChanged();
    }
}

void SinkInput::setDeviceIndex(quint32 deviceIndex)
{
    context()->d->setGenericDeviceForStream(index(), deviceIndex, &pa_context_move_sink_input_by_index);
}

void SinkInput::setVolume(qint64 volume)
{
    context()->d->setGenericVolume(index(), -1, volume, VolumeObject::d->cvolume(), &pa_context_set_sink_input_volume);
}

void SinkInput::setMuted(bool muted)
{
    context()->d->setGenericMute(index(), muted, &pa_context_set_sink_input_mute);
}

void SinkInput::setChannelVolume(int channel, qint64 volume)
{
    context()->d->setGenericVolume(index(), channel, volume, VolumeObject::d->cvolume(), &pa_context_set_sink_input_volume);
}

} // PulseAudioQt
