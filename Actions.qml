import QtQuick
import QtQuick.Controls
Item {
    property alias quit: _quit
    property alias setting: _setting
    //退出程序
    Action {
        id:_quit
        text: qsTr("&Quit")
        icon.name: "application-exit"
        onTriggered: Qt.quit();
    }
    //设置
    Action {
        id:_setting
        text: qsTr("&Sting")
        icon.name: "systemsettings"
    }
}
