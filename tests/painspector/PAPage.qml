/*
    SPDX-FileCopyrightText: 2021 Nicolas Fella <nicolas.fella@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import org.kde.pulseaudioqt.tests as PulseAudioQt

import org.kde.kirigami as Kirigami

Kirigami.ScrollablePage {

    property alias model: repeater.model
    property alias delegate: repeater.delegate

    // Initially invisible, managed by NavigationTabBar's actions
    visible: false

    ColumnLayout {
        Kirigami.FormLayout {
            id: mainLayout
        }

        Repeater {
            id: repeater
            onItemAdded: (index, item) => item.twinFormLayouts = mainLayout
        }
    }
}
