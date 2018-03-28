#ifndef CLIENT_P_H
#define CLIENT_P_H

#include <pulse/introspect.h>
#include "client.h"

namespace QPulseAudio
{

class ClientPrivate
{
public:
    ClientPrivate(Client* q);

    Client* q;
    QString m_name;
};

} // QPulseAudio

#endif
