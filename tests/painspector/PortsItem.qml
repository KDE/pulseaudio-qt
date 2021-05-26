/*
    SPDX-FileCopyrightText: 2021 Nicolas Fella <nicolas.fella@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/
import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

import org.kde.kirigami 2.10 as Kirigami

import org.kde.pulseaudioqt.painspector 1.0

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
