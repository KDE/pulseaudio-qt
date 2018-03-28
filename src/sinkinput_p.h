#ifndef SINKINPUT_P_H
#define SINKINPUT_P_H
#include "sinkinput.h"

namespace PulseAudioQt {

class SinkInputPrivate {

public:
    explicit SinkInputPrivate(SinkInput* q);

    SinkInput* q;

};
}

#endif
