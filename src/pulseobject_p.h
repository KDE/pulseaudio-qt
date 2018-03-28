#ifndef PULSEOBJECT_P_H
#define PULSEOBJECT_P_H

#include <QVariantMap>

#include <pulse/introspect.h>

#include "context.h"

namespace QPulseAudio {

class PulseObjectPrivate {
public:
    explicit PulseObjectPrivate(PulseObject* q);

    PulseObject* q;
    quint32 m_index;
    QVariantMap m_properties;

};

template <typename PAInfo>
void PulseObject::updatePulseObject(PAInfo *info)
{
    d->m_index = info->index;

    d->m_properties.clear();
    void *it = nullptr;
    while (const char *key = pa_proplist_iterate(info->proplist, &it)) {
        Q_ASSERT(key);
        const char *value = pa_proplist_gets(info->proplist, key);
        if (!value) {
            qCDebug(PLASMAPA) << "property" << key << "not a string";
            continue;
        }
        Q_ASSERT(value);
        d->m_properties.insert(QString::fromUtf8(key), QString::fromUtf8(value));
    }
    Q_EMIT propertiesChanged();
}
}
#endif
