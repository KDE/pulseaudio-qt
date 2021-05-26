/*
    SPDX-FileCopyrightText: 2021 Nicolas Fella <nicolas.fella@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/
import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

import org.kde.kirigami 2.10 as Kirigami

Kirigami.FormLayout {

    Label {
        Kirigami.FormData.label: "Name:"
        text: Name
    }

    Label {
        Kirigami.FormData.label: "Description:"
        text: Description
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
        Kirigami.FormData.label: "Volume:"
        text: Volume
    }

    Label {
        Kirigami.FormData.label: "Muted:"
        text: Muted
    }

    Label {
        Kirigami.FormData.label: "Volume writable:"
        text: VolumeWritable
    }

    PortsItem {}

    PropertiesItem {}
}
