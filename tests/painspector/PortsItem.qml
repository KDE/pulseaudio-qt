/*
    SPDX-FileCopyrightText: 2021 Nicolas Fella <nicolas.fella@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import org.kde.kirigami as Kirigami

import org.kde.pulseaudioqt.painspector

Column {
    Kirigami.FormData.label: "Ports:"

    function availabilityToString(availability) {
        if (availability === 0) {
            return "Unknown"
        }
        if (availability === 1) {
            return "Available"
        }
        if (availability === 2) {
            return "Unavailable"
        }
    }

    Repeater {
        model: Ports
        delegate: Label {
            text: modelData.name + ": " + modelData.description + ", Priority: " + modelData.priority + ", Type: " + Enums.portTypeToString(modelData.type) + ", Availability: " + availabilityToString(modelData.availability)
        }
    }
}
