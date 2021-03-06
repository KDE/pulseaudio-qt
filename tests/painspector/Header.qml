/*
    SPDX-FileCopyrightText: 2021 Nicolas Fella <nicolas.fella@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/
import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

import org.kde.kirigami 2.15 as Kirigami

Kirigami.FormLayout {
    property string text
    twinFormLayouts: mainLayout
    Item {
        Kirigami.FormData.isSection: true
        Kirigami.FormData.label: text
    }
}
