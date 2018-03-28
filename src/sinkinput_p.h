#ifndef SINKINPUT_P_H
#define SINKINPUT_P_H
#include "sinkinput.h"

namespace QPulseAudio {

class SinkInputPrivate {

public:
    explicit SinkInputPrivate(SinkInput* q);

    SinkInput* q;

};
}

#endif
