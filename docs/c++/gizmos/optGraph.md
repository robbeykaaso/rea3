# Abstract
* a configurable multithread graph framework for connecting different processes  

# Usage
* the steps includes: create a graph instance, feed in the graph model and run the model, such as:  
```
    auto graph = std::make_shared<rea::operatorGraph>();
    QJsonObject mdy;
    graph->build(mdy);
    std::vector<QImage> imgs;
    graph->run(imgs);
```
</br>

# Data interface between pipes  
* stream->data(): bool : the result of the previous process  
* stream->scope():  
    - param: QJsonObject: the parameter of the process  
    - images: std::vector<QImage\>: the images source  
    - from: QString: the previous process id  

# Process model format  
* input: integer: whether is the starting process by existing and the value decides the execution sequence  
* next: QJsonArray: the next process ids  
* type: QString: the process name  
* view: QString: the qsgBoard name  
* qsg: QJsonObject: the gui data  
    - pos: QJsonArray: the aabb boundbox  
* thread: integer: the execution thread  
* imageIndex: QJsonArray: the handling image source sequence  
examples:  
```
{
    "operators": {
        "shp_3dec967a-3b27-45a6-a755-6e6899ab1c82": {
            "input": 0,
            "next": [
                "shp_f144855d-718d-48d2-98a0-382955cb7550"
            ],
            "qsg": {
                "pos": [
                    -36,
                    -32,
                    64,
                    -2
                ]
            },
            "type": "inputImage",
            "view": "reagrid2_ide_image"
        },
        "shp_52f6479d-1e84-4f9a-b0dd-5c330e5df808": {
            "imageIndex": 0,
            "next": [
            ],
            "qsg": {
                "pos": [
                    102,
                    236,
                    202,
                    266
                ]
            },
            "type": "showOneImage",
            "view": "reagrid3_ide_image"
        },
        "shp_f144855d-718d-48d2-98a0-382955cb7550": {
            "imageIndex": [
                0
            ],
            "next": [
                "shp_52f6479d-1e84-4f9a-b0dd-5c330e5df808"
            ],
            "qsg": {
                "pos": [
                    51,
                    118,
                    151,
                    148
                ]
            },
            "seq": [
                "shp_3dec967a-3b27-45a6-a755-6e6899ab1c82"
            ],
            "thread": 0,
            "type": "setImageOpacity",
            "value": 110
        }
    }
}
```
</br>