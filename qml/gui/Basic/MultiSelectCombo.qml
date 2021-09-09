import QtQuick 2.12
import QtQuick.Controls 2.12

Row {
    property alias caption: caption
    property alias background: bak
    property alias combo: cmb
    property double ratio: 0.3
    width: 120
    height: 30

    spacing: width * 0.05
    Text {
        id: caption
        text: "hello:"
        //horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: 12
        width: parent.width * ratio
        height: parent.height * 0.8
    }

    ComboBox {
          id: cmb
          width: parent.width * (1 - ratio - 0.1)
          height: parent.height * 0.8
          anchors.verticalCenter: parent.verticalCenter
          background: Rectangle {
              id: bak
              border.color: "transparent"
          }
          model: ListModel{

          }
          delegate: Rectangle {
              width: parent.width
              height: cmb.height
              Row {
                  spacing: 5
                  anchors.fill: parent
                  anchors.margins: 5
                  CheckBox {
                      id: chk
                      height: parent.height * 0.8
                      width: parent.width * 0.2
                      anchors.verticalCenter: parent.verticalCenter
                      checked: seled || false
                      indicator: Rectangle {
                          implicitWidth: parent.height * 0.8
                          implicitHeight: parent.height * 0.8
                          anchors.verticalCenter: chk.verticalCenter
                          radius: width / 4
                          color: "white"
                          border.color: "gray"

                          Rectangle {
                              x: (parent.height - width) * 0.5
                              y: (parent.height - height) * 0.5
                              width: parent.height * 0.55
                              height: parent.height * 0.55
                              //anchors.verticalCenter: chk.verticalCenter
                              //anchors.horizontalCenter: chk.horizontalCenter
                              radius: width / 4
                              color: "gray"
                              visible: parent.parent.checked
                          }
                      }
                      onCheckedChanged: {
                          var sels = cmb.displayText.split("、")
                          var nm = name.toString()
                          var idx = sels.indexOf(nm)
                          if (checked){
                              if (idx < 0)
                                  sels.push(nm)
                          }else if (idx >= 0)
                              sels.splice(idx, 1)

                          var ret = ""
                          for (var i in sels)
                              if (sels[i]){
                                  if (ret !== "")
                                      ret += "、"
                                  ret += sels[i]
                              }
                          cmb.displayText = ret
                      }
                  }
                  Label {
                      text: name
                      width: parent.width - chk.width
                      height: parent.height
                      elide: Text.ElideRight
                      verticalAlignment: Qt.AlignVCenter
                      horizontalAlignment: Qt.AlignHCenter
                  }
              }
          }
          Component.onCompleted: {
              displayText = ""
          }
    }
}
