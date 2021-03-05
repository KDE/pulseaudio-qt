# KF5 PulseAudio Qt Bindings - libpulse bindings for Qt

pulseaudio-qt is a Qt-style wrapper for [libpulse](https://freedesktop.org/software/pulseaudio/doxygen/). 

It allows querying and manipulation of various PulseAudio objects such as Sinks, Sources and Streams. It does not wrap the full feature set of libpulse. If you are missing a certain feature please [file a report](https://bugs.kde.org/enter_bug.cgi?product=pulseaudio-qt) or send a patch using [KDE's GitLab](https://invent.kde.org/libraries/pulseaudio-qt/).

For an overview of PulseAudio's architecture and terminology see https://gavv.github.io/blog/pulseaudio-under-the-hood/.

## Building
This is a cmake project, so you should be able to simply run `cmake` (with the proper command-line flags for your system setup), then `make`, then `make install` if desired.
