/*
    Copyright 2018 Nicolas Fella <nicolas.fella@gmx.de>

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
#ifndef STREAMRESTORE_P_H
#define STREAMRESTORE_P_H

#include "streamrestore.h"
#include <QVector>
#include <pulse/ext-stream-restore.h>

namespace PulseAudioQt
{
class StreamRestorePrivate
{
public:
    explicit StreamRestorePrivate(StreamRestore *q);
    virtual ~StreamRestorePrivate();

    void writeChanges(const pa_cvolume &volume, bool muted, const QString &device);
    void update(const pa_ext_stream_restore_info *info);

    StreamRestore *q;
    QString m_name;
    QString m_device;
    pa_cvolume m_volume;
    pa_channel_map m_channelMap;
    QVector<QString> m_channels;
    bool m_muted;

    struct {
        bool valid = false;
        pa_cvolume volume;
        bool muted;
        QString device;
    } m_cache;
};

}

#endif
