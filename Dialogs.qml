//好友对话窗口
//发消息和传输文件的窗口
import QtQuick
import QtCore
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

ApplicationWindow{
    id: _dialog
    title: name
    height: 700
    width: 600
    ColumnLayout {
        anchors.fill: parent
        spacing: 10
        ListModel {id: _messageModel }
        Rectangle{
            anchors.fill: parent
            //color: ""
            ListView {
                anchors.fill: parent
                id: _messageList
                width: parent.width
                height: parent.height - _inputRow.height
                model: _messageModel
                delegate: Item {
                        width: parent.width
                        height:50
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
                            onTapped: _messageList.currentIndex = index
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
            anchors.bottom: parent.bottom
            id: _inputRow
            width: parent.width
            spacing: 10
            ScrollView {
                width: 400
                height: 100
                TextField {
                    id:inputField
                    wrapMode: Text.Wrap
                    width: parent.width
                    placeholderText: qsTr("enter message")

                    //回车键也能发消息
                    onAccepted: {
                        if (inputField.text.trim() !== "") {
                            _messageModel.append({ timestamp: new Date().toLocaleTimeString() ,message: inputField.text, type: "text" });
                            inputField.text = "";
                        }
                    }
                }
            }


            Button {
                id: sendButton
                text: qsTr("sent message")
                onClicked: {
                    if (inputField.text.trim() !== "") {
                        _messageModel.append({ timestamp: new Date().toLocaleTimeString() ,message: inputField.text, type: "text" });
                        inputField.text = "";
                    }
                }
            }
            Button {
                text:  qsTr("sent image")
                onClicked: {_messageList
                    fileDialog1.open()
                }
            }

            FileDialog {
                id: fileDialog1
                //folder: shortcuts.home
                nameFilters: ["Images (*.png *.jpg)", "All files (*)"]

                onAccepted: {
                    if (fileDialog1.fileUrl !== "") {
                        _messageModel.append({timestamp: new Date().toLocaleTimeString() ,message:qsTr("image sent successfully"), type: "image" });
                    }
                }
            }
            Button {
                text: qsTr("sent file")
                onClicked: {
                    fileDialog2.open()
                }
            }

            FileDialog {
                id: fileDialog2
                onAccepted: {
                    if (fileDialog2.fileUrl !== "") {
                        _messageModel.append({ timestamp: new Date().toLocaleTimeString() ,message:qsTr("file sent successfully"), type: "file" ,type:"message"});
                        console.log(fileDialog2.fileUrls[0].toString);
                    }
                }
            }
            Button {
                        text: "Clear List"
                        onClicked: {
                            // Clear the ListModel
                             _messageModel.clear();
                        }
            }
        }//RowLayout

    }
}
