/*
    SPDX-FileCopyrightText: 2018 David Rosca <nowrep@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/
import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

ListView {
    boundsBehavior: Flickable.StopAtBounds

    delegate: ItemDelegate {
        width: parent.width
        text: Name
    }
}
