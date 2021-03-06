/*
    SPDX-FileCopyrightText: 2021 Nicolas Fella <nicolas.fella@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/
import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

import org.kde.pulseaudioqt.tests 0.1 as PulseAudioQt

import org.kde.kirigami 2.15 as Kirigami

Kirigami.ScrollablePage {

    property alias model: repeater.model
    property alias delegate: repeater.delegate


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
