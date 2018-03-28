#ifndef SOURCEOUTPUT_P_H
#define SOURCEOUTPUT_P_H

#include <pulse/introspect.h>
#include "sourceoutput.h"

namespace PulseAudioQt {

class SourceOutputPrivate {

public:
    explicit SourceOutputPrivate(SourceOutput* q);

    SourceOutput* q;
};

}

#endif
