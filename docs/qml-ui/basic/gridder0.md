# Abstract
* an automatic grid component for qml

# Attribute
* name: QString: a grid name and also the prefix of its item name, the item name is `name + "_gridder" + index`
* com: Component: the component of the grid item, it must have `width`, `height` and `name` and `index` attributes
* scale: QJsonObject:
    * sum: int: the whole grid count
    * layout: QJsonArray: the rows and columns
    * invisible: QJsonObject: the invisible grid index
* size: int or QJsonArray: the grid size. if it is integer, the layout will be automatically calculated. else if it is an array, the layout will be specified by the size
_sample_:
```
Gridder{
    name: qsTr("demo")
    scale: {"layout": [2, 2]}
    com: Component{
        Rectangle{
            property string name
            width: parent.width / parent.columns
            height: parent.height / parent.rows

            color: "transparent"
            border.color: "red"
        }
    }
}
```
</br>
