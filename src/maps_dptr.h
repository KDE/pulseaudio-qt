/*
    Copyright 2014-2015 Harald Sitter <sitter@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "debug.h"
#include <QMap>
#include <QObject>

#include <pulse/pulseaudio.h>
#include <pulse/ext-stream-restore.h>
#include "kf5pulseaudioqt_export.h"

#include "sink_p.h"
#include "source_p.h"

namespace PulseAudioQt
{

// Used for typedefs.
class Sink;
class Source;

/**
 * @see MapBase
 * This class is nothing more than the QObject base since moc cannot handle
 * templates.
 */
class KF5PULSEAUDIOQT_EXPORT MapBaseQObject_Dptr : public QObject
{
    Q_OBJECT

public:
    virtual int count() const = 0;
    virtual QObject *objectAt(int index) const = 0;
    virtual int indexOfObject(QObject *object) const = 0;

Q_SIGNALS:
    void aboutToBeAdded(int index);
    void added(int index);
    void aboutToBeRemoved(int index);
    void removed(int index);
};

/**
 * Maps a specific index to a specific object pointer.
 * This is used to give the unique arbitrary PulseAudio index of a PulseObject a
 * serialized list index. Namely it enables us to translate a discrete list
 * index to a pulse index to an object, and any permutation thereof.
 */
template<typename Type, typename PAInfo>
class MapBase_Dptr : public MapBaseQObject_Dptr
{
public:
    virtual ~MapBase_Dptr() {}

    const QMap<quint32, Type *> &data() const { return m_data; }

    int count() const Q_DECL_OVERRIDE
    {
        return m_data.count();
    }

    int indexOfObject(QObject *object) const Q_DECL_OVERRIDE
    {
        int index = 0;
        QMapIterator<quint32, Type *> it(m_data);
        while (it.hasNext()) {
            it.next();
            if (it.value() == object) {
                return index;
            }
            index++;
        }
        return -1;
    }

    QObject *objectAt(int index) const Q_DECL_OVERRIDE
    {
        return (m_data.constBegin() + index).value();
    }

    void reset()
    {
        while (!m_data.isEmpty()) {
            removeEntry(m_data.lastKey());
        }
        m_pendingRemovals.clear();
    }

    void insert(Type *object)
    {
        Q_ASSERT(!m_data.contains(object->index()));

        int modelIndex = 0;
        for (auto it = m_data.constBegin(); it != m_data.constEnd(); ++it) {
            if (object->index() < it.key()) {
                break;
            }
            modelIndex++;
        }

        Q_EMIT aboutToBeAdded(modelIndex);
        m_data.insert(object->index(), object);
        Q_ASSERT(modelIndex == m_data.keys().indexOf(object->index()));
        Q_EMIT added(modelIndex);
    }

    // Context is passed in as parent because context needs to include the maps
    // so we'd cause a circular dep if we were to try to use the instance here.
    // Plus that's weird separation anyway.
    void updateEntry(const PAInfo *info, QObject *parent)
    {
        Q_ASSERT(info);

        if (m_pendingRemovals.remove(info->index)) {
            // Was already removed again.
            return;
        }

        auto *obj = m_data.value(info->index, nullptr);
        if (!obj) {
            obj = new Type(parent);
        }
        obj->d->update(info);

        if (!m_data.contains(info->index)) {
            insert(obj);
        }
    }

    void removeEntry(quint32 index)
    {
        if (!m_data.contains(index)) {
            m_pendingRemovals.insert(index);
        } else {
            const int modelIndex = m_data.keys().indexOf(index);
            Q_EMIT aboutToBeRemoved(modelIndex);
            delete m_data.take(index);
            Q_EMIT removed(modelIndex);
        }
    }

protected:
    QMap<quint32, Type *> m_data;
    QSet<quint32> m_pendingRemovals;
};

typedef MapBase_Dptr<Sink, pa_sink_info> SinkMap;
typedef MapBase_Dptr<Source, pa_source_info> SourceMap;

} // PulseAudioQt
