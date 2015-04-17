#ifndef PULSEOBJECT_H
#define PULSEOBJECT_H

#include <QObject>

#include <pulse/introspect.h>

class Context;

class Q_DECL_EXPORT PulseObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(quint32 index READ index CONSTANT)
public:
    template <typename PAInfo>
    void updatePulseObject(PAInfo *info)
    {
        m_index = info->index;
    }

    quint32 index() const;

protected:
    PulseObject(QObject *parent = 0);
    virtual ~PulseObject();

    Context *context() const;
    quint32 m_index;
};

#endif // PULSEOBJECT_H
