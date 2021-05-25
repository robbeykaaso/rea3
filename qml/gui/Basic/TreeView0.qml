//ref: https://blog.csdn.net/colouroo/article/details/44700357
import QtQuick 2.0

Item {
    id: root
    width: 600
    height: 600
    property string sel
    property string tree_name
    // 对应type的图片路径
    property var imagePath
    property string selectWay: 'color' // color 选中改变颜色，background 选中改变背景,bold 选中字体加粗
    property string selectColor: 'red' // 选中改变字体或背景颜色
    property string openWay: 'single' // 打开关闭文件夹方式，single单击打开，double双击打开，all 单击双击都要，
    // 设置当前更改的选中的模块
    signal setCurrentSelect(string idd)

    property var currentText
    property var currentRectangle

    function buildTree(aModel, aData, aLevel) {
        if (aData === undefined)
            return
        aModel.clear()
        var idx = 0
        for (var i in aData) {
            aModel.append({
                              "name": aData[i]["name"],
                              "type": aData[i]["type"],
                              "level": aLevel,
                              "subNode": [],
                              "idd": aData[i]["id"]
                          })
            buildTree(aModel.get(idx++).subNode, aData[i]["children"],
                      aLevel + 1)
        }
    }

    function buildDefaultTree(data, select) {
        if (select) {
            sel = select
        }
        if (data) {
            buildTree(objModel, data, 0)
        }
    }

    function valid(){
        return objModel.count > 0
    }

    //Model
    ListModel {
        id: objModel
    }

    //Delegate
    Component {
        id: objRecursiveDelegate
        Column {
            id: objRecursiveColumn

            function getTypeImage(type) {
                if (imagePath[type]) {
                    return imagePath[type]
                } else {
                    return '../../resource/text.png'
                }
            }

            function changeActive() {
                if (selectWay === 'color') {
                    txt.color = selectColor
                    if (currentText && currentText !== txt) {
                        currentText.color = '#000'
                    }
                } else if (selectWay === 'bold') {
                    txt.font.bold = true
                    if (currentText && currentText !== txt) {
                        currentText.font.bold = false
                    }
                } else if (selectWay === 'background') {
                    rowRectangle.color = selectColor
                    rowRectangle.width = root.width
                    txt.color = '#fff'
                    if (currentText && currentText !== txt) {
                        currentText.color = '#000'
                        currentRectangle.color = 'transparent'
                    }
                    currentRectangle = rowRectangle
                }
                currentText = txt
            }

            function openOrCloseFolder(parent) {
                for (var i = 1; i < parent.children.length; ++i) {
                    parent.children[i].visible = !parent.children[i].visible
                }

                changeActive()
            }

            MouseArea {
                width: objRow.implicitWidth
                height: objRow.implicitHeight
                onDoubleClicked: {
                    if (openWay === 'double' || openWay === 'all') {
                        openOrCloseFolder(parent)
                    }
                }
                onClicked: {
                    sel = idd
                    changeActive()
                    setCurrentSelect(sel)
                }

                Row {
                    id: objRow
                    Rectangle {
                        id: rowRectangle
                        width: root.width
                        height: 20
                        color: (selectWay === 'background'
                                && sel === idd) ? selectColor : 'transparent'

                        Item {
                            id: leftItem
                            height: 1
                            x: 0
                            y: 0
                            width: model.level * 20
                                   + (objRecursiveColumn.children.length > 2 ? 0 : 20)
                        }

                        Image {
                            id: openOrClose
                            width: 10
                            height: 10
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: leftItem.right
                            visible: objRecursiveColumn.children.length > 2
                                     && (openWay === 'single'
                                         || openWay === 'all')
                            anchors.rightMargin: openOrClose.visible ? 20 : 0
                            source: objRecursiveColumn.children[1].visible ? "../../resource/triangle-down.png" : "../../resource/triangle-right.png"

                            MouseArea {
                                anchors.fill: parent
                                onClicked: openOrCloseFolder(objRecursiveColumn)
                            }
                        }

                        Image {
                            id: folder
                            width: 10
                            height: 10
                            anchors.left: openOrClose.right
                            anchors.verticalCenter: parent.verticalCenter
                            visible: objRecursiveColumn.children.length > 2
                                     && (openWay === 'double'
                                         || openWay === 'all')
                            anchors.leftMargin: folder.visible ? 5 : 0
                            source: objRecursiveColumn.children[1].visible ? "../../resource/open.png" : "../../resource/folder.png"
                        }

                        Image {
                            id: type
                            width: 10
                            height: 10
                            anchors.left: folder.right
                            anchors.verticalCenter: parent.verticalCenter
                            visible: objRecursiveColumn.children.length <= 2
                            source: getTypeImage(model.type)
                        }
                        Text {
                            id: txt
                            text: model.name
                            color: (selectWay === 'color' && sel
                                    === idd) ? selectColor : ((selectWay === 'background'
                                                               && sel === idd) ? '#fff' : '#000')
                            font.bold: (selectWay === 'blod' && sel === idd)
                            anchors.left: type.right
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }
            }
            Repeater {
                model: subNode
                delegate: objRecursiveDelegate
            }
        }
    }

    //View
    ListView {
        anchors.fill: parent
        model: objModel
        delegate: objRecursiveDelegate
    }
}
