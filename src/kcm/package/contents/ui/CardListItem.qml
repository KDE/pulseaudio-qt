import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

import org.kde.kquickcontrolsaddons 2.0

ColumnLayout {
    id: item

    property int uniformLabelWidth: Math.max(profileLabel.contentWidth, portLabel.contentWidth)

    width: parent.width

    RowLayout {
        QIconItem {
            id: clientIcon
            Layout.alignment: Qt.AlignHCenter
            width: height
            height: nameLabel.contentHeight
            icon: PulseObject.properties['device.icon_name'] ? PulseObject.properties['device.icon_name'] : 'audio-card'
        }

        Label {
            id: nameLabel
            Layout.fillWidth: true
            text: PulseObject.properties['device.description'] ? PulseObject.properties['device.description'] : Name
            elide: "ElideRight"
        }
    }

    RowLayout {
        Label {
            id: profileLabel
            Layout.minimumWidth: item.uniformLabelWidth
            Layout.maximumWidth: Layout.minimumWidth
            // FIXME: i18n
            text: "Profile:"
            font.bold: true
        }
        ComboBox {
            Layout.fillWidth: true
            model: PulseObject.profiles
            textRole: "description"
            currentIndex: PulseObject.activeProfileIndex
            onCurrentIndexChanged: {
                // Get name.
                textRole = "name";
                var name = currentText;
                textRole = "description";
                kcm.context.setCardProfile(Index, name);
            }
        }
    }

    RowLayout {
        Label {
            id: portLabel
            Layout.minimumWidth: item.uniformLabelWidth
            Layout.maximumWidth: Layout.minimumWidth
            // FIXME: i18n
            text: "Ports:"
            font.bold: true
        }
        ComboBox {
            Layout.fillWidth: true
            model: PulseObject.ports
            textRole: "description"
        }
    }

    Rectangle {
        // FIXME: needs kcolorscheme backing
        color: "grey"
        Layout.fillWidth: true
        height: 1
    }
}
