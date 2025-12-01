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

    Label {
        Kirigami.FormData.label: "Virtual:"
        text: VirtualStream ? "true" : "false"
    }

    Label {
        Kirigami.FormData.label: "Corked:"
        text: Corked ? "true" : "false"
    }

    Label {
        Kirigami.FormData.label: "Has volume:"
        text: HasVolume ? "true" : "false"
    }

    PropertiesItem {}

}
