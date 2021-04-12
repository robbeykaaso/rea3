import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Window 2.13
import QtQuick.Controls.Universal 2.3
import '../Basic'

Window{
    id: root
    //flags: Qt.Dialog
    property string caption
    property alias content: mid.data
    property var titlebuttons
    property var footbuttons
    property bool titlevisible: true
    property string titlecolor: "lightskyblue"
    property string buttoncolor: "gray"
    property string fontcolor: "black"
    property string bodycolor: "lightskyblue"
    signal closed()
    flags: Qt.Window | Qt.CustomizeWindowHint //Qt.FramelessWindowHint
    modality: Qt.WindowModal
    width: 400
    height: 300
    color: bodycolor

    function show() {
        root.visible = true
        cont.visible = true
    }

    function close(c) {
        if (!c) {
            close(true)
            root.visible = false
        }
        cont.close()
    }

    Window{
        id: cont
        width: root.width
        x: root.x
        height: root.height + 7
        y: root.y - 7
        color: bodycolor
        visible: true
        flags: Qt.Window | Qt.FramelessWindowHint
        transientParent: root

        MouseArea{
            property int coor_x
            property int coor_y
            width: parent.width
            height: Screen.desktopAvailableHeight * 0.03
            z: -1
            onPressed: function(aInput){
                coor_x = aInput["x"]
                coor_y = aInput["y"]
            }
            onPositionChanged: function(aInput){
                root.x += aInput["x"] - coor_x
                root.y += aInput["y"] - coor_y
            }
        }

        Column{
            anchors.fill: parent
            Rectangle{
                width: parent.width
                height: Screen.desktopAvailableHeight * 0.03
                color: bodycolor
                visible: !titlevisible
            }
            Rectangle{
                width: parent.width
                height: Screen.desktopAvailableHeight * 0.03
                color: titlecolor
                visible: titlevisible
                Row{
                    anchors.fill: parent
                    Item{
                        x: parent.height * 0.2
                        y: parent.height * 0.2
                        height: parent.height * 0.6
                        width: height
                        ShaderEffect{
                            id: logo
                            anchors.fill: parent
                            property variant source: Image{
                                anchors.fill: parent
                                source: "../../resource/logo.png"
                            }
                            property real amplitude: 0
                            property real frequency: 20
                            property real time: 0
                            NumberAnimation on time {
                                id: animation
                                running: false
                                loops: Animation.Infinite; from: 0; to: Math.PI * 2; duration: 600
                            }
                            /*fragmentShader: "
                                varying highp vec2 qt_TexCoord0;
                                uniform sampler2D source;
                                uniform lowp float qt_Opacity;
                                uniform lowp float redChannel;
                                void main(){
                                    gl_FragColor = texture2D(source, qt_TexCoord0) * vec4(redChannel, 1.0, 1.0, 1.0) * qt_Opacity;
                                }
                            "*/
                            fragmentShader: "
                                uniform sampler2D source;
                                varying highp vec2 qt_TexCoord0;
                                uniform lowp float qt_Opacity;
                                uniform highp float amplitude;
                                uniform highp float frequency;
                                uniform highp float time;
                                void main()
                                {
                                    highp vec2 p = sin(time + frequency * qt_TexCoord0);
                                    gl_FragColor = texture2D(source, qt_TexCoord0 + amplitude * vec2(p.y, -p.x)) * qt_Opacity;
                                }
                            "
                            MouseArea{
                                anchors.fill: parent
                                onClicked: {
                                    animation.running = !animation.running
                                    if (animation.running)
                                        logo.amplitude = 0.02
                                    else
                                        logo.amplitude = 0
                                }
                            }
                        }
                    }
                    Item{
                        id: titbar
                        height: parent.height
                        width: parent.width - btns.parent.width - logo.width
                        Text{
                            leftPadding: parent.width * 0.05
                            anchors.verticalCenter: parent.verticalCenter
                            text: caption
                            font.pixelSize: 12
                            color: fontcolor
                        }
                    }
                    Row{
                        width: childrenRect.width
                        height: parent.height

                        Repeater {
                            id: btns
                            model: ListModel {
                            }

                            delegate: Button {
                                width: parent.height
                                height: parent.height
                                contentItem: Text{
                                    text: cap
                                    color: parent.hovered ? buttoncolor : fontcolor
                                    font.pixelSize: 12
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                                background: Rectangle{
                                    border.color: parent.hovered ? "gray" : "transparent"
                                    color: parent.hovered ? "#ececec" : 'transparent'
                                }
                            }
                        }

                        Component.onCompleted: {
                            if (titlebuttons){
                                for (var i in titlebuttons){
                                    btns.model.append({cap: titlebuttons[i]["cap"]})
                                    btns.itemAt(btns.count - 1).clicked.connect(titlebuttons[i]["func"])
                                }
                            }
                            btns.model.append({cap: "X"})
                            btns.itemAt(btns.count - 1).clicked.connect(function(){
                                closed()
                                close()
                            })
                        }
                    }
                }

            }
            Item{
                id: mid
                width: parent.width
                height: parent.height - Screen.desktopAvailableHeight * (0.045 + (footbuttons ? 0.03 : 0))
            }
            Row{
                anchors.right: parent.right
                anchors.rightMargin: height * 0.4
                width: childrenRect.width
                height: footbuttons ? Screen.desktopAvailableHeight * 0.045 : 0
                spacing: 5

                Repeater{
                    id: btns2
                    model: ListModel{

                    }
                    delegate: Button{
                        height: parent.height * 0.5
                        width: height * 2.5
                        contentItem: Text{
                            text: cap
                            color: parent.hovered ? buttoncolor : fontcolor
                            font.pixelSize: 12
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        anchors.verticalCenter: parent.verticalCenter
                        background: Rectangle{
                            color: parent.hovered ? "#d7d7d7" : buttoncolor
                            border.color: parent.hovered ? buttoncolor : "#d7d7d7"
                        }
                    }
                }

                Component.onCompleted: {
                    if (footbuttons)
                        for (var i in footbuttons){
                            btns2.model.append({cap: footbuttons[i]["cap"]})
                            btns2.itemAt(btns2.count - 1).clicked.connect(footbuttons[i]["func"])
                        }
                }
            }
        }
    }

    onHeightChanged: {
        mid.height = height - Screen.desktopAvailableHeight * (0.045 + (footbuttons ? 0.03 : 0))
    }

    function setHeight(aHeight){
        height = aHeight
        mid.height = height - Screen.desktopAvailableHeight * (0.045 + (footbuttons ? 0.03 : 0))
    }
}
