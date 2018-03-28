#ifndef SOURCE_P_H
#define SOURCE_P_H

#include <pulse/introspect.h>
#include "source.h"

namespace QPulseAudio {

class SourcePrivate {
public:
    explicit SourcePrivate(Source* q);

    Source* q;
};
}
#endif
