//好友对话窗口
//发消息和传输文件的窗口
import QtQuick
import QtCore
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

ApplicationWindow{
    title: name
    id: _dialog
    height: 800
    width: 600
    ToolBar {
        anchors.top: parent.top
        anchors.right: parent.right
        RowLayout{
            ToolButton{
                action: actions.clear
            }

        }
    }

    ColumnLayout {
        anchors.bottom: parent.bottom
        spacing: 10
        ListModel {id: messageModel}
        Rectangle{
            height: 700
            width: 600
            ListView {
                id: messageList
                width: parent.width
                height: parent.height - inputRow.height
                model: messageModel
                delegate: Item {
                        width: parent.width
                        height: 20
                        Text {
                            text: model.timestamp+"      "+model.message
                            wrapMode: Text.WordWrap
                            width: parent.width - 20
                            elide: Text.ElideRight
                        }

                        Image {
                            source: model.type === "image" ? model.text : ""
                            visible: model.type === "image"
                            width: parent.width
                            height: parent.width / 2
                        }

                        Text {
                            text: model.type === "file" ? "文件路径：" + model.text : ""
                            visible: model.type === "file"
                        }
                        TapHandler{
                            onTapped: messageList.currentIndex = index
                        }
                    }
                clip: true
                focus: true
                orientation: ListView.Vertical
                spacing: 5
                highlight: Rectangle {
                    color: "lightblue"
                }
            }
        }

        RowLayout {
            id: inputRow
            width: parent.width
            spacing: 10

            TextField {
                id: inputField
                width: parent.width
                placeholderText: "输入消息..."

                //回车键也能发消息
                onAccepted: {
                    if (inputField.text.trim() !== "") {
                        messageModel.append({ timestamp: new Date().toLocaleTimeString() ,message: inputField.text, type: "text" });
                        inputField.text = "";
                    }
                }
            }

            Button {
                id: sendButton
                text: qsTr("send message")
                onClicked: {
                    if (inputField.text.trim() !== "") {
                        messageModel.append({ timestamp: new Date().toLocaleTimeString() ,message: inputField.text, type: "text" });
                        inputField.text = "";
                    }
                }
            }
            Button {
                text:  qsTr("send image")
                onClicked: {
                    fileDialog1.open()
                }
            }

            FileDialog {
                id: fileDialog1
                //folder: shortcuts.home
                nameFilters: ["Images (*.png *.jpg)", "All files (*)"]

                onAccepted: {
                    if (fileDialog1.fileUrl !== "") {
                        messageModel.append({ text: fileDialog1.fileUrl.toString(), type: "image" });
                    }
                }
            }
            Button {
                text: qsTr("send file")
                onClicked: {
                    fileDialog2.open()
                }
            }

            FileDialog {
                id: fileDialog2
                onAccepted: {
                    if (fileDialog2.fileUrl !== "") {
                        messageModel.append({ text: fileDialog2.fileUrl.toString(), type: "file" });
                    }
                }
            }


        }//RowLayout
    }
}
