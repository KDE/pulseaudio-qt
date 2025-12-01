/*
    SPDX-FileCopyrightText: 2021 Nicolas Fella <nicolas.fella@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import org.kde.kirigami as Kirigami

Kirigami.FormLayout {
    property string text
    twinFormLayouts: mainLayout
    Item {
        Kirigami.FormData.isSection: true
        Kirigami.FormData.label: text
    }
}
