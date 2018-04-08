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

#ifndef CONTEXT_H
#define CONTEXT_H

#include <QObject>
#include "kf5pulseaudioqt_export.h"
#include "pulse/volume.h"

namespace PulseAudioQt
{

class Card;
class Client;
class Sink;
class SinkInput;
class Source;
class SourceOutput;
class StreamRestore;
class Module;
class Server;

class KF5PULSEAUDIOQT_EXPORT Context : public QObject
{
    Q_OBJECT

public:
    ~Context();

    static Context *instance();

    static const qint64 NormalVolume = PA_VOLUME_NORM;
    static const qint64 MinimalVolume = 0;
    static const qint64 MaximalVolume = (PA_VOLUME_NORM / 100.0) * 150;

    void ref();
    void unref();

    bool isValid();

    /**
     * Returns a list of all sinks.
     *
     * @return list of sinks
     */
    QVector<Sink*> sinks() const;

    /**
     * Returns a list of all sink inputs.
     *
     * @return list of sink inputs
     */
    QVector<SinkInput*> sinkInputs() const;

    /**
     * Returns a list of all sources.
     *
     * @return list of sources
     */
    QVector<Source*> sources() const;

    /**
     * Returns a list of all source outputs.
     *
     * @return list of source outputs
     */
    QVector<SourceOutput*> sourceOutputs() const;

    /**
     * Returns a list of all clients.
     *
     * @return list of clients
     */
    QVector<Client*> clients() const;

    /**
     * Returns a list of all cards.
     *
     * @return list of cards
     */
    QVector<Card*> cards() const;

    /**
     * Returns a list of all modules.
     *
     * @return list of modules
     */
    QVector<Module*> modules() const;

    /**
     * Returns a list of all stream restores.
     *
     * @return list of stream restores
     */
    QVector<StreamRestore*> streamRestores() const;

    Server *server() const;

    void setCardProfile(quint32 index, const QString &profile);
    void setDefaultSink(const QString &name);
    void setDefaultSource(const QString &name);

Q_SIGNALS:
    /**
     * Indicates that sink was added.
     */
    void sinkAdded(Sink *sink);

    /**
     * Indicates that sink was removed.
     */
    void sinkRemoved(Sink *sink);

    /**
     * Indicates that sink input was added.
     */
    void sinkInputAdded(SinkInput *sinkInput);

    /**
     * Indicates that sink input was removed.
     */
    void sinkInputRemoved(SinkInput *sinkInput);

    /**
     * Indicates that source was added.
     */
    void sourceAdded(Source *source);

    /**
     * Indicates that source was removed.
     */
    void sourceRemoved(Source *source);

    /**
     * Indicates that source output was added.
     */
    void sourceOutputAdded(SourceOutput *sourceOutput);

    /**
     * Indicates that source output was removed.
     */
    void sourceOutputRemoved(SourceOutput *sourceOutput);

    /**
     * Indicates that client was added.
     */
    void clientAdded(Client *client);

    /**
     * Indicates that client was removed.
     */
    void clientRemoved(Client *client);

    /**
     * Indicates that card was added.
     */
    void cardAdded(Card *card);

    /**
     * Indicates that card was removed.
     */
    void cardRemoved(Card *card);

    /**
     * Indicates that module was added.
     */
    void moduleAdded(Module *module);

    /**
     * Indicates that module was removed.
     */
    void moduleRemoved(Module *module);

    /**
     * Indicates that stream restore was added.
     */
    void streamRestoreAdded(StreamRestore *streamRestore);

    /**
     * Indicates that streamRestore was removed.
     */
    void streamRestoreRemoved(StreamRestore *streamRestore);

private:
    explicit Context(QObject *parent = nullptr);

    class ContextPrivate *const d;

    friend class Sink;
    friend class SinkInput;
    friend class Source;
    friend class SourceOutput;
    friend class Stream;
    friend class StreamRestorePrivate;
    friend class Server;
    friend class SinkModel;
    friend class SinkInputModel;
    friend class SourceModel;
    friend class SourceOutputModel;
    friend class StreamRestoreModel;
    friend class CardModel;
    friend class ModuleModel;
};

} // PulseAudioQt

#endif // CONTEXT_H
