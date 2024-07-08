//设置窗口
import QtQuick 2.15
import QtQuick.Controls 2.15
import "i.js"  as Controller
ApplicationWindow {
    id:_setting
    width: 330
    height: 380
    title: "Seting-About"
    TabBar{
        id:_tabbar
        TabButton{text: "Setting";width: 70;onClicked: _stackView.push(page1)}
        TabButton{text: "About";width: 70;onClicked: _stackView.push(page2)}
    }
    StackView {
            id: _stackView
            anchors.top: _tabbar.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            initialItem: page1 // 设置初始页面

            Component {
                id: page1
                Page {
                    Column{
                        leftPadding: 10
                        spacing: 10
                        TextInput{
                            width: 200
                            Row{
                               Text{
                                   text: "name :"
                                   font.pointSize: 16
                                   color: "white"
                               }
                               TextField{
                                   id:_inputname
                                   width: 100
                                   placeholderText: "press EnterKey to save"
                                   onAccepted: {
                                       var name=_inputname.text;
                                       Controller.setName(name);
                                       _inputname.text="";
                                   }
                               }
                            }
                        }
                        TextInput{
                            width: 200
                            anchors.margins: 10
                            Row{
                               Text{
                                   text: "    ip   :"
                                   font.pointSize: 16
                                   color: "white"
                               }
                               TextField{
                                   id:_inputip
                                   width: 100
                                   placeholderText: "press EnterKey to save"
                                   onAccepted: {_inputip.text="";}
                               }
                            }
                        }
                        TextInput{
                            width: 200
                            Row{
                               Text{
                                   text: "status:"
                                   font.pointSize: 16
                                   color: "white"
                               }
                               RadioButton {
                                               text: "Online"
                                               onClicked: {
                                                   var status=text;
                                                   Controller.setStatus(status)
                                               }
                               }
                               RadioButton {
                                               text: "Away"
                                               onClicked: {
                                                   var status=text;
                                                   Controller.setStatus(status)
                                               }
                               }
                               RadioButton {
                                               text: "Offline"
                                               onClicked: {
                                                   var status=text;
                                                   Controller.setStatus(status)
                                               }
                               }
                            }
                        }
                    }
                }
            }

            Component {
                id: page2
                Page {
                    Label {
                        text: qsTr("ipmsg 1.0")
                        anchors.centerIn: parent
                    }
                }
            }
        }

}
