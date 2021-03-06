/*
    SPDX-FileCopyrightText: 2018 David Rosca <nowrep@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/
import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

import org.kde.pulseaudioqt.tests 0.1 as PulseAudioQt

import org.kde.kirigami 2.15 as Kirigami

Kirigami.ApplicationWindow {

    Kirigami.SwipeNavigator {
        anchors.fill: parent

        PAPage {
            title: "Cards"
            model: PulseAudioQt.CardModel {}
            delegate: CardDelegate {}
        }

        PAPage {
            title: "Sinks"
            model: PulseAudioQt.SinkModel {}
            delegate: DeviceDelegate {}
        }

        PAPage {
            title: "Sources"
            model: PulseAudioQt.SourceModel {}
            delegate: DeviceDelegate {}
        }

        PAPage {
            title: "Sink Inputs"
            model: PulseAudioQt.SinkInputModel {}
            delegate: StreamDelegate {}
        }

        PAPage {
            title: "Source Outputs"
            model: PulseAudioQt.SourceOutputModel {}
            delegate: StreamDelegate {}
        }

        PAPage {
            title: "Stream Restores"
            model: PulseAudioQt.StreamRestoreModel {}
            delegate: StreamDelegate {}
        }

        PAPage {
            title: "Modules"
            model: PulseAudioQt.ModuleModel {}
            delegate: ModuleDelegate {
            }
        }
    }
}
