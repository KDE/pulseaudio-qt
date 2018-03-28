#ifndef STREAMRESTORE_P_H
#define STREAMRESTORE_P_H

#include "streamrestore.h"
#include <pulse/ext-stream-restore.h>

namespace QPulseAudio {

class StreamRestorePrivate {

public:
    explicit StreamRestorePrivate(StreamRestore* q);
    virtual ~StreamRestorePrivate();

    void writeChanges(const pa_cvolume &volume, bool muted, const QString &device);

    StreamRestore* q;
    QString m_name;
    QString m_device;
    pa_cvolume m_volume;
    pa_channel_map m_channelMap;
    QStringList m_channels;
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
