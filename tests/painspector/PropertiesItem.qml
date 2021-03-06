/*
    SPDX-FileCopyrightText: 2021 Nicolas Fella <nicolas.fella@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/
import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

import org.kde.kirigami 2.10 as Kirigami

Column {
    Kirigami.FormData.label: "Properties:"
    Repeater {
        model: Object.keys(Properties)
        delegate: Label {
            text: modelData + ": " + Properties[modelData]
        }
    }
}
