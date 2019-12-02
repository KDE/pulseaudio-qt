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

#ifndef PULSEAUDIO_H
#define PULSEAUDIO_H

#include <QAbstractListModel>

#include "kf5pulseaudioqt_export.h"

namespace PulseAudioQt
{
class Context;
class MapBaseQObject;
class Sink;
class Source;
class AbstractModelPrivate;
class SinkModelPrivate;

class KF5PULSEAUDIOQT_EXPORT AbstractModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ItemRole { PulseObjectRole = Qt::UserRole + 1 };

    ~AbstractModel() override;
    QHash<int, QByteArray> roleNames() const final override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const final override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) final override;

    int role(const QByteArray &roleName) const;

protected:
    AbstractModel(const MapBaseQObject *map, QObject *parent);
    void initRoleNames(const QMetaObject &qobjectMetaObject);
    Context *context() const;

private Q_SLOTS:
    void propertyChanged();

private:
    void onDataAdded(int index);
    void onDataRemoved(int index);
    QMetaMethod propertyChangedMetaMethod() const;

    AbstractModelPrivate *d;

    // Prevent leaf-classes from default constructing as we want to enforce
    // them passing us a context or explicit nullptrs.
    AbstractModel()
    {
    }
};

class KF5PULSEAUDIOQT_EXPORT CardModel : public AbstractModel
{
    Q_OBJECT
public:
    CardModel(QObject *parent = nullptr);

private:
    void *d;
};

class KF5PULSEAUDIOQT_EXPORT SinkModel : public AbstractModel
{
    Q_OBJECT
    Q_PROPERTY(PulseAudioQt::Sink *defaultSink READ defaultSink NOTIFY defaultSinkChanged)
    Q_PROPERTY(PulseAudioQt::Sink *preferredSink READ preferredSink NOTIFY preferredSinkChanged)
public:
    enum ItemRole { SortByDefaultRole = PulseObjectRole + 1 };
    Q_ENUM(ItemRole)

    SinkModel(QObject *parent = nullptr);
    virtual ~SinkModel();
    Sink *defaultSink() const;
    Sink *preferredSink() const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

Q_SIGNALS:
    void defaultSinkChanged();
    void preferredSinkChanged();

private:
    void sinkAdded(int index);
    void sinkRemoved(int index);
    void updatePreferredSink();
    Sink *findPreferredSink() const;
    SinkModelPrivate *d;
};

class KF5PULSEAUDIOQT_EXPORT SinkInputModel : public AbstractModel
{
    Q_OBJECT
public:
    SinkInputModel(QObject *parent = nullptr);

private:
    void *d;
};

class KF5PULSEAUDIOQT_EXPORT SourceModel : public AbstractModel
{
    Q_OBJECT
    Q_PROPERTY(PulseAudioQt::Source *defaultSource READ defaultSource NOTIFY defaultSourceChanged)
public:
    enum ItemRole { SortByDefaultRole = PulseObjectRole + 1 };
    Q_ENUM(ItemRole)

    SourceModel(QObject *parent = nullptr);
    Source *defaultSource() const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

Q_SIGNALS:
    void defaultSourceChanged();

private:
    void *d;
};

class KF5PULSEAUDIOQT_EXPORT SourceOutputModel : public AbstractModel
{
    Q_OBJECT
public:
    SourceOutputModel(QObject *parent = nullptr);

private:
    void *d;
};

class KF5PULSEAUDIOQT_EXPORT StreamRestoreModel : public AbstractModel
{
    Q_OBJECT
public:
    StreamRestoreModel(QObject *parent = nullptr);

private:
    void *d;
};

class KF5PULSEAUDIOQT_EXPORT ModuleModel : public AbstractModel
{
    Q_OBJECT
public:
    ModuleModel(QObject *parent = nullptr);

private:
    void *d;
};

} // PulseAudioQt

#endif // PULSEAUDIO_H
