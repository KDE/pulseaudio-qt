/*
    SPDX-FileCopyrightText: 2014-2015 Harald Sitter <sitter@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef CONTEXT_H
#define CONTEXT_H

#include "pulseaudioqt_export.h"
#include <QObject>

struct pa_context;

/**
 * The primary namespace of PulseAudioQt.
 */
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

/**
 * The normal volume (100%, 0 dB). Equivalent to PA_VOLUME_NORM.
 */
PULSEAUDIOQT_EXPORT qint64 normalVolume();
/**
 * The minimum volume (0%). Equivalent to PA_VOLUME_MUTED.
 */
PULSEAUDIOQT_EXPORT qint64 minimumVolume();
/**
 * The maximum volume PulseAudio can store. Equivalent to PA_VOLUME_MAX.
 * \warning For UI elements like volume sliders use maximumUIVolume instead.
 */
PULSEAUDIOQT_EXPORT qint64 maximumVolume();

/**
 * The maximum volume suitable to display in a UI. Equivalent to PA_VOLUME_UI_MAX.
 */
PULSEAUDIOQT_EXPORT qint64 maximumUIVolume();

class PULSEAUDIOQT_EXPORT Context : public QObject
{
    Q_OBJECT
    /**
     * The state of the Context. This is further augmented by the autoConnecting property.
     *
     * @since 1.6
     */
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    /**
     * Regardless of the state, this property indicates whether the Context is presently trying to
     * automatically establish a connection. When this is false it may be useful to give the
     * user a manual way to trigger a connection attempt. AutoConnecting is subject to internal
     * timeouts that when hit will prevent further auto connecting until the Context managed to
     * connect.
     */
    Q_PROPERTY(bool autoConnecting READ isAutoConnecting NOTIFY autoConnectingChanged)

public:
    /**
     * @since 1.6
     */
    enum class State {
        Unconnected = 0,
        Connecting,
        Authorizing,
        SettingName,
        Ready,
        Failed,
        Terminated,
    };
    Q_ENUM(State)

    ~Context() override;

    static Context *instance();

    /**
     * Set the application id that is reported to PulseAudio.
     * This needs to be called before accessing the context singleton the first time.
     * If not set QGuiApplication::desktopFileName() is used.
     */
    static void setApplicationId(const QString &applicationId);

    bool isValid();

    /**
     * Returns a list of all sinks.
     *
     * @return list of sinks
     */
    QList<Sink *> sinks() const;

    /**
     * Returns a list of all sink inputs.
     *
     * @return list of sink inputs
     */
    QList<SinkInput *> sinkInputs() const;

    /**
     * Returns a list of all sources.
     *
     * @return list of sources
     */
    QList<Source *> sources() const;

    /**
     * Returns a list of all source outputs.
     *
     * @return list of source outputs
     */
    QList<SourceOutput *> sourceOutputs() const;

    /**
     * Returns a list of all clients.
     *
     * @return list of clients
     */
    QList<Client *> clients() const;

    /**
     * Returns a list of all cards.
     *
     * @return list of cards
     */
    QList<Card *> cards() const;

    /**
     * Returns a list of all modules.
     *
     * @return list of modules
     */
    QList<Module *> modules() const;

    /**
     * Returns a list of all stream restores.
     *
     * @return list of stream restores
     */
    QList<StreamRestore *> streamRestores() const;

    Server *server() const;

    /**
     *  Returns a pointer to the raw PulseAudio context.
     */
    pa_context *context() const;

    void setCardProfile(quint32 index, const QString &profile);
    void setDefaultSink(const QString &name);
    void setDefaultSource(const QString &name);

    /**
     * @returns the state of the context.
     *
     * @since 1.6
     */
    [[nodiscard]] State state() const;

    /**
     * @returns whether the Context is currently trying to auto-connect to the daemon
     *
     * @since 1.6
     */
    [[nodiscard]] bool isAutoConnecting() const;

public Q_SLOTS:
    /**
     * When the Context is not auto-connecting this may be used to give the user a manual trigger (e.g. a button)
     *
     * @since 1.6
     */
    void reconnectDaemon();

Q_SIGNALS:
    /**
     * Indicates that sink was added.
     */
    void sinkAdded(PulseAudioQt::Sink *sink);

    /**
     * Indicates that sink was removed.
     */
    void sinkRemoved(PulseAudioQt::Sink *sink);

    /**
     * Indicates that sink input was added.
     */
    void sinkInputAdded(PulseAudioQt::SinkInput *sinkInput);

    /**
     * Indicates that sink input was removed.
     */
    void sinkInputRemoved(PulseAudioQt::SinkInput *sinkInput);

    /**
     * Indicates that source was added.
     */
    void sourceAdded(PulseAudioQt::Source *source);

    /**
     * Indicates that source was removed.
     */
    void sourceRemoved(PulseAudioQt::Source *source);

    /**
     * Indicates that source output was added.
     */
    void sourceOutputAdded(PulseAudioQt::SourceOutput *sourceOutput);

    /**
     * Indicates that source output was removed.
     */
    void sourceOutputRemoved(PulseAudioQt::SourceOutput *sourceOutput);

    /**
     * Indicates that client was added.
     */
    void clientAdded(PulseAudioQt::Client *client);

    /**
     * Indicates that client was removed.
     */
    void clientRemoved(PulseAudioQt::Client *client);

    /**
     * Indicates that card was added.
     */
    void cardAdded(PulseAudioQt::Card *card);

    /**
     * Indicates that card was removed.
     */
    void cardRemoved(PulseAudioQt::Card *card);

    /**
     * Indicates that module was added.
     */
    void moduleAdded(PulseAudioQt::Module *module);

    /**
     * Indicates that module was removed.
     */
    void moduleRemoved(PulseAudioQt::Module *module);

    /**
     * Indicates that stream restore was added.
     */
    void streamRestoreAdded(PulseAudioQt::StreamRestore *streamRestore);

    /**
     * Indicates that streamRestore was removed.
     */
    void streamRestoreRemoved(PulseAudioQt::StreamRestore *streamRestore);

    /**
     * Context state changed.
     *
     * @since 1.6
     */
    void stateChanged();

    /**
     * Indicates that autoConnecting changed.
     *
     * @since 1.6
     */
    void autoConnectingChanged();

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
