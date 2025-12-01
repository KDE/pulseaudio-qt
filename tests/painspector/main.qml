/*
    SPDX-FileCopyrightText: 2018 David Rosca <nowrep@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import org.kde.pulseaudioqt.tests as PulseAudioQt

import org.kde.kirigami as Kirigami

Kirigami.ApplicationWindow {
    id: root

    pageStack.initialPage: pageCards

    PAPage {
        id: pageCards
        title: "Cards"
        model: PulseAudioQt.CardModel {}
        delegate: CardDelegate {}
    }

    PAPage {
        id: pageSinks
        title: "Sinks"
        model: PulseAudioQt.SinkModel {}
        delegate: DeviceDelegate {}
    }

    PAPage {
        id: pageSources
        title: "Sources"
        model: PulseAudioQt.SourceModel {}
        delegate: DeviceDelegate {}
    }

    PAPage {
        id: pageSinkInputs
        title: "Sink Inputs"
        model: PulseAudioQt.SinkInputModel {}
        delegate: StreamDelegate {}
    }

    PAPage {
        id: pageSourceOutputs
        title: "Source Outputs"
        model: PulseAudioQt.SourceOutputModel {}
        delegate: StreamDelegate {}
    }

    PAPage {
        id: pageStreamRestores
        title: "Stream Restores"
        model: PulseAudioQt.StreamRestoreModel {}
        delegate: StreamDelegate {}
    }

    PAPage {
        id: pageModules
        title: "Modules"
        model: PulseAudioQt.ModuleModel {}
        delegate: ModuleDelegate {}
    }

    component PageAction : Kirigami.Action {
        required property PAPage page

        text: page.title
        checked: page.visible

        onTriggered: {
            root.pageStack.clear();
            root.pageStack.push(page);
        }
    }

    footer: Kirigami.NavigationTabBar {
        actions: [
            PageAction {
                page: pageCards
            },
            PageAction {
                page: pageSinks
            },
            PageAction {
                page: pageSources
            },
            PageAction {
                page: pageSinkInputs
            },
            PageAction {
                page: pageSourceOutputs
            },
            PageAction {
                page: pageStreamRestores
            },
            PageAction {
                page: pageModules
            }
        ]
    }
}
