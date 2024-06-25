import QtQuick
import Qt.labs.folderlistmodel
import QtQuick.Dialogs
import QtQuick.Controls  as QQC
import QtMultimedia
import QtQuick.Layouts

Item {
    property alias friendModel: _friendModel
    // 定义一个模型来存储好友信息，这里使用一个简单的 ListModel
        ListModel {
            id: _friendModel
            ListElement { name: "Alice"; status: "Online"; }
            // 添加更多好友信息
        }

        ListView{
            anchors.fill: parent  // 填充父元素，即整个窗口
            model: _friendModel   // 设置模型为之前定义的 ListModel

            // 定义列表项的外观
            delegate: Item {
                width: parent.width
                height: 50

                Rectangle {
                    width: parent.width
                    height: 50
                    color: index % 2 === 0 ? "#f0f0f0" : "#ffffff" // 隔行变色

                    Text {
                        anchors.centerIn: parent
                        text: model.name + " - " + model.status
                    }
                }


                TapHandler{
                    onTapped: {
                        var component = Qt.createComponent("Dialogs.qml");
                        if (component.status === Component.Ready) {
                            var window = component.createObject(main);
                            window.show();
                        } else {
                            console.log("Error loading component:", component.errorString());
                        }

                        console.log("Clicked on " + model.name);
                        // 在此处添加处理点击列表项的逻辑
                    }
                }
            }
        }
}
