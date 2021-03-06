/*
    SPDX-FileCopyrightText: 2014-2015 Harald Sitter <sitter@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef SINK_H
#define SINK_H

#include "device.h"

struct pa_sink_info;

namespace PulseAudioQt
{
/**
 * A PulseAudio sink. This class is based on https://freedesktop.org/software/pulseaudio/doxygen/structpa__sink__info.html.
 */
class PULSEAUDIOQT_EXPORT Sink : public Device
{
    Q_OBJECT

public:
    ~Sink();

    /**
     * Set the volume for this sink.
     * This affects all channels.
     * The volume must be between PulseAudioQt::minimumVolume() and PulseAudioQt::maximumVolume().
     */
    void setVolume(qint64 volume) override;

    /**
     * Set whether this sink is muted.
     */
    void setMuted(bool muted) override;

    void setActivePortIndex(quint32 port_index) override;

    /**
     * Set the volume for a specific channel of this sink.
     * The volume must be between PulseAudioQt::minimumVolume() and PulseAudioQt::maximumVolume().
     */
    void setChannelVolume(int channel, qint64 volume) override;

    /**
     * Whether this is the default sink.
     */
    bool isDefault() const override;

    /**
     * Set whether this is the default sink.
     */
    void setDefault(bool enable) override;

private:
    explicit Sink(QObject *parent);

    class SinkPrivate *const d;
    friend class MapBase<Sink, pa_sink_info>;
};

} // PulseAudioQt

#endif // SINK_H
