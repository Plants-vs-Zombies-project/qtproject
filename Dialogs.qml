//好友对话窗口
//发消息和传输文件的窗口
import QtQuick
import QtCore
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import tcpclient 1.0
import udpclient 1.0
import "i.js"  as Controller
ApplicationWindow{
    id: _dialog
    title: name
    height: 700
    width: 600
    Udpclient{
        id:_udpC
        onMessageReceived: {_messageModel.append({ timestamp: new Date().toLocaleTimeString() ,message:message,type:"message"})}
        onImageReceived: {_messageModel.append({timestamp: new Date().toLocaleTimeString() ,text:imageData.toBase64(),type:"image"})}
    }
    /*Tcpclient{
        id:_tcpC
    }*/


    //查看图片的弹窗
    Popup{
        id:_popimage
        width: 400
        height: 400
        contentItem: Image{
            id:_popI
            anchors.centerIn: parent
            fillMode: Image.PreserveAspectFit
        }
    }
    //聊天消息列表
    ColumnLayout {
        anchors.fill: parent
        spacing: 10
        ListModel {id: _messageModel; }
        Rectangle{
            anchors.fill: parent
            ListView {
                anchors.fill: parent
                id: _messageList
                width: parent.width
                height: parent.height - _inputRow.height
                model: _messageModel
                delegate: Row {
                        width: parent.width
                        height:100
                        Text {
                            width: parent.width-100
                            id:_text
                            text: model.timestamp+"      "+model.message
                            wrapMode: Text.WordWrap
                            elide: Text.ElideRight
                        }
                        Image {
                            id:_image
                            source: model.type === "image" ? model.text : ""
                            visible: model.type === "image"
                            width: 60
                            height: 100
                            //fillMode: _image.PreserveAspectFit
                            TapHandler{
                                onTapped: _popimage.open()
                            }
                        }
                        Text {
                            //text: model.type === "file" ? model.message : ""
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
        //输入文字的对话框
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
                            _messageModel.append({ timestamp: new Date().toLocaleTimeString(),message:Controller.getName()+":"+inputField.text, type: "message" });
                            _udpC.sendMessage(Controller.getName()+":"+inputField.text);
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
                        _messageModel.append({ timestamp: new Date().toLocaleTimeString() ,message: Controller.getName()+":"+inputField.text, type: "message" });
                        _udpC.sendMessage(Controller.getName()+":"+inputField.text);
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
            //发送照片对话框
            FileDialog {
                id: fileDialog1
                //folder: shortcuts.home
                nameFilters: ["Images (*.png *.jpg)", "All files (*)"]

                onAccepted: {
                    var url=fileDialog1.selectedFile;
                    var source=url.toString();
                    Controller.setSource(source);
                    if (url !== ""&& url !== undefined) {
                        _messageModel.append({timestamp: new Date().toLocaleTimeString() ,text:url.toString(),message:Controller.getName()+":"+url.toString().split("/").pop(), type: "image" });
                        _udpC.sendImage(url);
                        _udpC.sendMessage(Controller.getName()+":"+url.toString().split("/").pop());
                    }
                }
            }
            Button {
                text: qsTr("sent file")
                onClicked: {
                    fileDialog2.open()
                }
            }
            //传输文件对话框
            FileDialog {
                id: fileDialog2
                onAccepted: {
                    var url=fileDialog2.selectedFile
                    if (url !== ""&& url !== undefined) {
                        _messageModel.append({ timestamp: new Date().toLocaleTimeString() ,message:Controller.getName()+":"+url.toString().split("/").pop(), type: "file" });
                        _udpC.sendMessage(Controller.getName()+":"+url.toString().split("/").pop());
                        //_tcpC.filetransmitC(url.toString());
                        //_tcpC.foldertransmitC(url.toString());
                    }
                }
            }
            //清空消息
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
