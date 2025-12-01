/*
    SPDX-FileCopyrightText: 2021 Nicolas Fella <nicolas.fella@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import org.kde.kirigami as Kirigami

Column {
    Kirigami.FormData.label: "Properties:"
    Repeater {
        model: Object.keys(Properties)
        delegate: Label {
            text: modelData + ": " + Properties[modelData]
        }
    }
}
