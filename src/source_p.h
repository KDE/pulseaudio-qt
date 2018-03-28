#ifndef SOURCE_P_H
#define SOURCE_P_H

#include <pulse/introspect.h>
#include "source.h"

namespace PulseAudioQt {

class SourcePrivate {
public:
    explicit SourcePrivate(Source* q);
    
    void update(const pa_source_info *info);
    
    Source* q;
};
}
#endif
