/*
    Copyright 2014-2015 Harald Sitter <sitter@kde.org>
    Copyright 2016 David Rosca <nowrep@gmail.com>

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

#include "models.h"

#include "card.h"
#include "context.h"
#include "context_p.h"
#include "debug.h"
#include "maps.h"
#include "module.h"
#include "server.h"
#include "sink.h"
#include "sinkinput.h"
#include "source.h"
#include "sourceoutput.h"
#include "streamrestore.h"

#include "models_p.h"
#include <QMetaEnum>

namespace PulseAudioQt
{
AbstractModel::AbstractModel(const MapBaseQObject *map, QObject *parent)
    : QAbstractListModel(parent)
    , d(new AbstractModelPrivate(this, map))
{
    Context::instance()->ref();

    connect(d->m_map, &MapBaseQObject::aboutToBeAdded, this, [this](int index) { beginInsertRows(QModelIndex(), index, index); });
    connect(d->m_map, &MapBaseQObject::added, this, [this](int index) {
        onDataAdded(index);
        endInsertRows();
    });
    connect(d->m_map, &MapBaseQObject::aboutToBeRemoved, this, [this](int index) { beginRemoveRows(QModelIndex(), index, index); });
    connect(d->m_map, &MapBaseQObject::removed, this, [this](int index) {
        Q_UNUSED(index);
        endRemoveRows();
    });
}

AbstractModel::~AbstractModel()
{
    // deref context after we've deleted this object
    // see https://bugs.kde.org/show_bug.cgi?id=371215
    Context::instance()->unref();
    delete d;
}

AbstractModelPrivate::AbstractModelPrivate(AbstractModel *q, const MapBaseQObject *map)
    : q(q)
    , m_map(map)
{
}

AbstractModelPrivate::~AbstractModelPrivate()
{
}

QHash<int, QByteArray> AbstractModel::roleNames() const
{
    if (!d->m_roles.empty()) {
        qCDebug(PULSEAUDIOQT) << "returning roles" << d->m_roles;
        return d->m_roles;
    }
    Q_UNREACHABLE();
    return QHash<int, QByteArray>();
}

int AbstractModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return d->m_map->count();
}

QVariant AbstractModel::data(const QModelIndex &index, int role) const
{
    if (!hasIndex(index.row(), index.column())) {
        return QVariant();
    }
    QObject *data = d->m_map->objectAt(index.row());
    Q_ASSERT(data);
    if (role == PulseObjectRole) {
        return QVariant::fromValue(data);
    } else if (role == Qt::DisplayRole) {
        return static_cast<PulseObject *>(data)->properties().value(QStringLiteral("name")).toString();
    }
    int property = d->m_objectProperties.value(role, -1);
    if (property == -1) {
        return QVariant();
    }
    return data->metaObject()->property(property).read(data);
}

bool AbstractModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!hasIndex(index.row(), index.column())) {
        return false;
    }
    int propertyIndex = d->m_objectProperties.value(role, -1);
    if (propertyIndex == -1) {
        return false;
    }
    QObject *data = d->m_map->objectAt(index.row());
    auto property = data->metaObject()->property(propertyIndex);
    return property.write(data, value);
}

int AbstractModel::role(const QByteArray &roleName) const
{
    qCDebug(PULSEAUDIOQT) << roleName << d->m_roles.key(roleName, -1);
    return d->m_roles.key(roleName, -1);
}

Context *AbstractModel::context() const
{
    return Context::instance();
}

void AbstractModel::initRoleNames(const QMetaObject &qobjectMetaObject)
{
    d->m_roles[PulseObjectRole] = QByteArrayLiteral("PulseObject");

    QMetaEnum enumerator;
    for (int i = 0; i < metaObject()->enumeratorCount(); ++i) {
        if (metaObject()->enumerator(i).name() == QLatin1String("ItemRole")) {
            enumerator = metaObject()->enumerator(i);
            break;
        }
    }

    for (int i = 0; i < enumerator.keyCount(); ++i) {
        // Clip the Role suffix and glue it in the hash.
        const int roleLength = 4;
        QByteArray key(enumerator.key(i));
        // Enum values must end in Role or the enum is crap
        Q_ASSERT(key.right(roleLength) == QByteArrayLiteral("Role"));
        key.chop(roleLength);
        d->m_roles[enumerator.value(i)] = key;
    }

    int maxEnumValue = -1;
    for (auto it = d->m_roles.constBegin(); it != d->m_roles.constEnd(); ++it) {
        if (it.key() > maxEnumValue) {
            maxEnumValue = it.key();
        }
    }
    Q_ASSERT(maxEnumValue != -1);
    auto mo = qobjectMetaObject;
    for (int i = 0; i < mo.propertyCount(); ++i) {
        QMetaProperty property = mo.property(i);
        QString name(property.name());
        name.replace(0, 1, name.at(0).toUpper());
        d->m_roles[++maxEnumValue] = name.toLatin1();
        d->m_objectProperties.insert(maxEnumValue, i);
        if (!property.hasNotifySignal()) {
            continue;
        }
        d->m_signalIndexToProperties.insert(property.notifySignalIndex(), i);
    }
    qCDebug(PULSEAUDIOQT) << d->m_roles;

    // Connect to property changes also with objects already in model
    for (int i = 0; i < d->m_map->count(); ++i) {
        onDataAdded(i);
    }
}

void AbstractModel::propertyChanged()
{
    if (!sender() || senderSignalIndex() == -1) {
        return;
    }
    int propertyIndex = d->m_signalIndexToProperties.value(senderSignalIndex(), -1);
    if (propertyIndex == -1) {
        return;
    }
    int role = d->m_objectProperties.key(propertyIndex, -1);
    if (role == -1) {
        return;
    }
    int index = d->m_map->indexOfObject(sender());
    qCDebug(PULSEAUDIOQT) << "PROPERTY CHANGED (" << index << ") :: " << role << roleNames().value(role);
    Q_EMIT dataChanged(createIndex(index, 0), createIndex(index, 0), {role});
}

void AbstractModel::onDataAdded(int index)
{
    QObject *data = d->m_map->objectAt(index);
    const QMetaObject *mo = data->metaObject();
    // We have all the data changed notify signals already stored
    auto keys = d->m_signalIndexToProperties.keys();
    foreach (int index, keys) {
        QMetaMethod meth = mo->method(index);
        connect(data, meth, this, propertyChangedMetaMethod());
    }
}

QMetaMethod AbstractModel::propertyChangedMetaMethod() const
{
    auto mo = metaObject();
    int methodIndex = mo->indexOfMethod("propertyChanged()");
    if (methodIndex == -1) {
        return QMetaMethod();
    }
    return mo->method(methodIndex);
}

SinkModel::SinkModel(QObject *parent)
    : AbstractModel(&context()->d->m_sinks, parent)
    , d(new SinkModelPrivate(this))
{
    initRoleNames(Sink::staticMetaObject);

    for (int i = 0; i < context()->d->m_sinks.count(); ++i) {
        sinkAdded(i);
    }

    connect(&context()->d->m_sinks, &MapBaseQObject::added, this, &SinkModel::sinkAdded);
    connect(&context()->d->m_sinks, &MapBaseQObject::removed, this, &SinkModel::sinkRemoved);

    connect(context()->server(), &Server::defaultSinkChanged, this, [this]() {
        updatePreferredSink();
        Q_EMIT defaultSinkChanged();
    });
}

SinkModel::~SinkModel()
{
    delete d;
}

SinkModelPrivate::SinkModelPrivate(SinkModel *q)
    : q(q)
    , m_preferredSink(nullptr)
{
}

SinkModelPrivate::~SinkModelPrivate()
{
}

Sink *SinkModel::defaultSink() const
{
    return context()->server()->defaultSink();
}

Sink *SinkModel::preferredSink() const
{
    return d->m_preferredSink;
}

QVariant SinkModel::data(const QModelIndex &index, int role) const
{
    if (role == SortByDefaultRole) {
        // Workaround QTBUG-1548
        const QString pulseIndex = data(index, AbstractModel::role(QByteArrayLiteral("Index"))).toString();
        const QString defaultDevice = data(index, AbstractModel::role(QByteArrayLiteral("Default"))).toString();
        return defaultDevice + pulseIndex;
    }
    return AbstractModel::data(index, role);
}

void SinkModel::sinkAdded(int index)
{
    Q_ASSERT(qobject_cast<Sink *>(context()->d->m_sinks.objectAt(index)));
    Sink *sink = static_cast<Sink *>(context()->d->m_sinks.objectAt(index));
    connect(sink, &Sink::stateChanged, this, &SinkModel::updatePreferredSink);

    updatePreferredSink();
}

void SinkModel::sinkRemoved(int index)
{
    Q_UNUSED(index);

    updatePreferredSink();
}

void SinkModel::updatePreferredSink()
{
    Sink *sink = findPreferredSink();

    if (sink != d->m_preferredSink) {
        qCDebug(PULSEAUDIOQT) << "Changing preferred sink to" << sink << (sink ? sink->name() : "");
        d->m_preferredSink = sink;
        Q_EMIT preferredSinkChanged();
    }
}

Sink *SinkModel::findPreferredSink() const
{
    const auto &sinks = context()->d->m_sinks;

    // Only one sink is the preferred one
    if (sinks.count() == 1) {
        return static_cast<Sink *>(sinks.objectAt(0));
    }

    auto lookForState = [&](Device::State state) {
        Sink *ret = nullptr;
        const auto data = sinks.data();
        for (Sink *sink : data) {
            if (sink->state() != state) {
                continue;
            }
            if (!ret) {
                ret = sink;
            } else if (sink == defaultSink()) {
                ret = sink;
                break;
            }
        }
        return ret;
    };

    Sink *preferred = nullptr;

    // Look for playing sinks + prefer default sink
    preferred = lookForState(Device::RunningState);
    if (preferred) {
        return preferred;
    }

    // Look for idle sinks + prefer default sink
    preferred = lookForState(Device::IdleState);
    if (preferred) {
        return preferred;
    }

    // Fallback to default sink
    return defaultSink();
}

SourceModel::SourceModel(QObject *parent)
    : AbstractModel(&context()->d->m_sources, parent)
{
    initRoleNames(Source::staticMetaObject);

    connect(context()->server(), &Server::defaultSourceChanged, this, &SourceModel::defaultSourceChanged);
}

Source *SourceModel::defaultSource() const
{
    return context()->server()->defaultSource();
}

QVariant SourceModel::data(const QModelIndex &index, int role) const
{
    if (role == SortByDefaultRole) {
        // Workaround QTBUG-1548
        const QString pulseIndex = data(index, AbstractModel::role(QByteArrayLiteral("Index"))).toString();
        const QString defaultDevice = data(index, AbstractModel::role(QByteArrayLiteral("Default"))).toString();
        return defaultDevice + pulseIndex;
    }
    return AbstractModel::data(index, role);
}

SinkInputModel::SinkInputModel(QObject *parent)
    : AbstractModel(&context()->d->m_sinkInputs, parent)
{
    initRoleNames(SinkInput::staticMetaObject);
}

SourceOutputModel::SourceOutputModel(QObject *parent)
    : AbstractModel(&context()->d->m_sourceOutputs, parent)
{
    initRoleNames(SourceOutput::staticMetaObject);
}

CardModel::CardModel(QObject *parent)
    : AbstractModel(&context()->d->m_cards, parent)
{
    initRoleNames(Card::staticMetaObject);
}

StreamRestoreModel::StreamRestoreModel(QObject *parent)
    : AbstractModel(&context()->d->m_streamRestores, parent)
{
    initRoleNames(StreamRestore::staticMetaObject);
}

ModuleModel::ModuleModel(QObject *parent)
    : AbstractModel(&context()->d->m_modules, parent)
{
    initRoleNames(Module::staticMetaObject);
}

} // PulseAudioQt
