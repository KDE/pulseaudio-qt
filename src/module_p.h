#ifndef MODULE_P_H
#define MODULE_P_H

#include "module.h"

namespace QPulseAudio {

class ModulePrivate {

public:
    explicit ModulePrivate(Module* q);
    virtual ~ModulePrivate();

    Module* q;

    QString m_name;
    QString m_argument;

};
}

#endif
