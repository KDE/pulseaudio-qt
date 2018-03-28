#ifndef CARD_P_H
#define CARD_P_H

#include "card.h"
#include <pulse/introspect.h>

namespace QPulseAudio {
class CardPrivate {

public:
    explicit CardPrivate(Card* q);
    virtual ~CardPrivate();

    Card* q;

    QString m_name;
    QList<QObject *> m_profiles;
    quint32 m_activeProfileIndex;
    QList<QObject *> m_ports;
};
}

#endif
