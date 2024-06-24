import QtQuick
import QtQuick.Controls
Item {
    property alias open: _open
    property alias about: _about
    property alias quit: _quit
    property alias setting: _setting
    property alias clear: _clear
    //property alias friend: _friend
    Action {
        id:_open
        text: qsTr("&Open...")
        icon.name: "document-open"
        shortcut: "StandardKey.Open"
        onTriggered: console.log("Open action triggered");
    }

    Action {
        id:_quit
        text: qsTr("&Quit")
        icon.name: "application-exit"
        onTriggered: Qt.quit();
    }

    Action {
        id:_about
        text: qsTr("&About")
        icon.name: "help-about"
    }
    Action {
        id:_setting
        text: qsTr("&Sting")
        icon.name: "systemsettings"
    }
    Action {
        id: _clear
        icon.name: "albumfolder-user-trash-symbolic"
        //onTriggered: //清空消息记录
    }

}
