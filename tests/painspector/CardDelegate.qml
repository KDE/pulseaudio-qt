/*
    SPDX-FileCopyrightText: 2021 Nicolas Fella <nicolas.fella@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import org.kde.kirigami as Kirigami

Kirigami.FormLayout {

    Label {
        Kirigami.FormData.label: "Name:"
        text: Name
    }

    Row {
        Kirigami.FormData.label: "Icon:"
        spacing: 5
        Label {
            id: iconNameLabel
            text: IconName
        }
        Kirigami.Icon {
            height: iconNameLabel.height
            width: height
            source: IconName
        }
    }

    PropertiesItem {}

    PortsItem {}

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

    Column {
        Kirigami.FormData.label: "Profiles:"
        Repeater {
            model: Profiles
            delegate: Label {
                text: modelData.name + ": " + modelData.description + ", Priority: " + modelData.priority + ", Availability: " + availabilityToString(modelData.availability)
            }
        }
    }
}
