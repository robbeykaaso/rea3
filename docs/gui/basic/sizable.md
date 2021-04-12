# Abstract
the sizable component for qml  

# Sample
```
    Row{
        Item{
            property int del_size: 0
            width: 200 + del_size
        }
        Sizable{

        }
        Item{
            property int del_size: 0
            width: 200 + del_size
        }
    }

    Column{
        Item{
            property int del_size: 0
            height: 200 + del_size
        }
        Sizable{

        }
        Item{
            property int del_size: 0
            height: 200 + del_size
        }
    }
```  
</br>

# Test and Demo
test.qml: Sizable  
</br>