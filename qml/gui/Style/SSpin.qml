pragma Singleton
import QtQuick 2.12
import QtQuick.Controls.Styles 1.4

//Nesting QtObjects not work
//http://imaginativethinking.ca/make-qml-component-singleton/ qmldir
QtObject {
    property double ratio: 0.3
    property int width: 120
    property int height: 30
    function spacing(aSpin){
        return aSpin.width * 0.05
    }
    property QtObject text: QtObject{
        property int fontSize: 12
        function width(aText){
            return aText.parent.width * aText.parent.ratio
        }
        function height(aText){
            return aText.parent.height * 0.8
        }
    }
    property QtObject rect: QtObject{
        function width(aRect){
            return aRect.parent.width * (1 - aRect.parent.ratio - 0.1)
        }
        function height(aRect){
            return aRect.parent.height * 0.8
        }
        property QtObject spinBox: QtObject{
            property int fontSize: 12
            property int min: 0
            property int max: 200
            property Component style: SpinBoxStyle {
                background: Rectangle {
                    //radius: 6
                }
            }
        }
    }
}
