//好友对话窗口
//发消息和传输文件的窗口
import QtQuick
import QtCore
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

ApplicationWindow{
    title: "friendname"
    property alias dialog: _dialog
    id: _dialog
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

        ListView {
            id: messageList
            width: parent.width
            height: parent.height - inputRow.height
            model: messageModel
            delegate: MessageDelegate {
                width: messageList.width

                // You can extend this delegate to handle different message types
            }
            clip: true
            focus: false
            orientation: ListView.Vertical
            spacing: 5
            highlight: Rectangle {
                color: "lightblue"
            }
        }

        RowLayout {
            id: inputRow
            width: parent.width
            spacing: 10

            TextField {
                id: inputField
                width: parent.width - sendButton.width - 20
                placeholderText: "输入消息..."
                onAccepted: {
                    if (inputField.text.trim() !== "") {
                        messageModel.append({ text: inputField.text, type: "text" });
                        inputField.text = "";
                    }
                }
            }

            Button {
                id: sendButton
                text: "发送"
                onClicked: {
                    if (inputField.text.trim() !== "") {
                        messageModel.append({ text: inputField.text, type: "text" });
                        inputField.text = "";
                    }
                }
            }
            Button {
                text: "发送图片"
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
                        messageModel.append({ text: fileDialog1-p.fileUrl.toString(), type: "image" });
                    }
                }
            }
            Button {
                text: "发送文件"
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
    ListModel {
        id: messageModel
    }

}
