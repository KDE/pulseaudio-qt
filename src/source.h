#ifndef SOURCE_H
#define SOURCE_H

#include "devicebase.h"

class Q_DECL_EXPORT Source : public DeviceBase
{
    Q_OBJECT
public:
    void update(const pa_source_info *info);
    void setVolume(qint64 volume) Q_DECL_OVERRIDE;
    void setMuted(bool muted) Q_DECL_OVERRIDE;
    void setActivePortIndex(quint32 port_index) Q_DECL_OVERRIDE;
};

#endif // SOURCE_H
