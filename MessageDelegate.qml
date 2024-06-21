//对话框输入的渲染
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
Item {
    width: parent.width
    Text {
        text: model.text
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
}
