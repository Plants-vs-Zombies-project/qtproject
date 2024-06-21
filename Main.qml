//主窗口
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    property alias main:_main
    property alias content: _content
    id:_main
    width: 340
    height: 480
    visible: true
    title: qsTr("ipmsg")
    menuBar: MenuBar {

        Menu {
            title: qsTr("File")
            MenuItem { action: actions.open }
            MenuItem { action: actions.quit }

        }
        Menu {
            title: qsTr("Setting")
            MenuItem { action: actions.setting }
        }
    }

     ToolBar {
        anchors.bottom: parent.bottom
        RowLayout{
            ToolButton{ action: {actions.open} }
            ToolButton{ action: actions.quit }
        }
    }
    Actions {
        id: actions
        setting.onTriggered: {
            var component = Qt.createComponent("Windows.qml");
            if (component.status === Component.Ready) {
                var window = component.createObject(_main);
                window.show();
            } else {
                console.log("Error loading component:", component.errorString());
            }
        }
    }
    Content {
        id:_content
        height: 400
        width: parent.width
        }
    }

    //Content Area


