#ifndef SINK_P_H
#define SINK_P_H

#include <pulse/introspect.h>
#include "sink.h"

namespace PulseAudioQt {

class SinkPrivate {
public:
    explicit SinkPrivate(Sink* q);
    
    void update(const pa_sink_info *info);
    
    Sink* q;
};
}
#endif

