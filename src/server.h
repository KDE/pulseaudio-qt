/*
    SPDX-FileCopyrightText: 2016 David Rosca <nowrep@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef SERVER_H
#define SERVER_H

#include "pulseaudioqt_export.h"
#include <QObject>

namespace PulseAudioQt
{
class Sink;
class Source;
class Context;

class PULSEAUDIOQT_EXPORT Server : public QObject
{
    Q_OBJECT

public:
    ~Server();

    Sink *defaultSink() const;
    void setDefaultSink(Sink *sink);

    Source *defaultSource() const;
    void setDefaultSource(Source *source);

Q_SIGNALS:
    void defaultSinkChanged(PulseAudioQt::Sink *sink);
    void defaultSourceChanged(PulseAudioQt::Source *source);

private:
    explicit Server(Context *context);

    void reset();
    void updateDefaultDevices();

    class ServerPrivate *const d;

    friend class ServerPrivate;
    friend class Context;
    friend class ContextPrivate;
};

} // PulseAudioQt

#endif // CONTEXT_H
