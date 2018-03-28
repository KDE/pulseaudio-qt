#ifndef SINK_P_H
#define SINK_P_H

#include <pulse/introspect.h>
#include "sink.h"

namespace QPulseAudio {

class SinkPrivate {
public:
    explicit SinkPrivate(Sink* q);

    Sink* q;
};
}
#endif

