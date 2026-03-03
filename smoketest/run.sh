#!/usr/bin/bash

# SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
# SPDX-FileCopyrightText: 2026 Harald Sitter <sitter@kde.org>

set -ex

if [ "$KDECI_BUILD" = "TRUE" ]; then
    # ci-utilities mangles the environment - unmangle it so systemctl actually manages to talk to the daemon instance.
    DBUS_SESSION_BUS_ADDRESS="unix:path=/run/user/$(id -u)/bus"
    export DBUS_SESSION_BUS_ADDRESS
    XDG_RUNTIME_DIR="/run/user/$(id -u)"
    export XDG_RUNTIME_DIR

    systemctl --user enable --now pipewire.socket pipewire-pulse.socket wireplumber.service

    cat /proc/asound/cards
    cat /proc/asound/devices
    cat /proc/asound/modules
    cat /proc/asound/pcm

    pactl list
    aplay -l
    aplay -L
fi

exec "$@"
