/*
    SPDX-FileCopyrightText: 2018 David Rosca <nowrep@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/
import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

import org.kde.pulseaudioqt.tests 0.1 as PulseAudioQt

ApplicationWindow {
    visible: true

    ScrollView {
        anchors.fill: parent
        contentWidth: width

        ColumnLayout {
            width: parent.width

            PaModelHeader {
                Layout.fillWidth: true
                text: "Cards"
            }

            PaModelView {
                Layout.fillWidth: true
                Layout.preferredHeight: contentHeight
                model: PulseAudioQt.CardModel {}
            }

            PaModelHeader {
                Layout.fillWidth: true
                text: "Sinks"
            }

            PaModelView {
                Layout.fillWidth: true
                Layout.preferredHeight: contentHeight
                model: PulseAudioQt.SinkModel {}
            }

            PaModelHeader {
                Layout.fillWidth: true
                text: "Sink Inputs"
            }

            PaModelView {
                Layout.fillWidth: true
                Layout.preferredHeight: contentHeight
                model: PulseAudioQt.SinkInputModel {}
            }

            PaModelHeader {
                Layout.fillWidth: true
                text: "Sources"
            }

            PaModelView {
                Layout.fillWidth: true
                Layout.preferredHeight: contentHeight
                model: PulseAudioQt.SourceModel {}
            }

            PaModelHeader {
                Layout.fillWidth: true
                text: "Source Outputs"
            }

            PaModelView {
                Layout.fillWidth: true
                Layout.preferredHeight: contentHeight
                model: PulseAudioQt.SourceOutputModel {}
            }

            PaModelHeader {
                Layout.fillWidth: true
                text: "Stream Restores"
            }

            PaModelView {
                Layout.fillWidth: true
                Layout.preferredHeight: contentHeight
                model: PulseAudioQt.StreamRestoreModel {}
            }

            PaModelHeader {
                Layout.fillWidth: true
                text: "Modules"
            }

            PaModelView {
                Layout.fillWidth: true
                Layout.preferredHeight: contentHeight
                model: PulseAudioQt.ModuleModel {}
            }

            Item { Layout.fillHeight: true; }
        }
    }
}
