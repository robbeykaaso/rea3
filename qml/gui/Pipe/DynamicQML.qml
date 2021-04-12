import QtQuick 2.12
import Pipeline 1.0

Item{
    property string name
    signal loaded(var aItem)

    Component{
        id: loader
        Loader{
            Component.onDestruction: {
                //console.log("hi2")
            }
        }
    }

    Component.onCompleted: {
        Pipeline.find("loadDynamicQMLs")
        .next(function(aInput){
            var pths = aInput.data()
            for (var i in pths){
                var ld = loader.createObject(parent)
                ld.loaded.connect(
                    function(aLoader){
                        return function(){
                            loaded(aLoader.item)
                        }
                    }(ld)
                )
                ld.source = pths[i]
            }
        }, name, {vtype: "array"})
        Pipeline.run("loadDynamicQMLs", name, name)
    }
}
