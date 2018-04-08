/*
    Copyright 2018 David Rosca <nowrep@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
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
