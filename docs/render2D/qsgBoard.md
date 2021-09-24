# Abstract
* a qtquick component for show and modify shapes or images. a container for the business plugins  

# API Pipe
* **updateQSGAttr_ + name**  
    - renew attributes of the objects or the whole model for the qsgModel  
    - type: pipeDelegate: QSGAttrUpdated_ + name  
    - pipeline: c++; trigger    
    - input: QJsonArray  
    - scope:
        - model: the qsgModel format  
        - image: QHash<QString, QImage\> : the image data  
        - image_data: QJsonObject<QString, QString-Base64\> : the image data  
    - output: input  
_sample_:  
```
    //scope image
    QHash<QString, QImage> imgs;
    imgs.insert("image_0.xxx", QImage("xxxxx"));
    //scope image_data
    QJsonObject img_data;
    img_data.insert("image_1.xxx", rea::QImage2Base64(QImage("yyyyy")));
    //scope image_path
    QJsonArray img_path;
    img_path.push_back("image_2.xxx");
    //scope storage_config
    QJsonObject cfg = rea::Json("config", rea::Json("root", "", "access", "", "secret", "", "ip", ""), "root", "aws0")    
    //scope model
    auto mdl = rea::Json("width", 600,
                         "height", 600,
                         "objects", rea::Json( 
                            "img_0", rea::Json(
                                "type", "image",
                                "range", rea::JArray(0, 0, 300, 400),
                                "path", "image_0.xxx"
                            )),
                            "img_1", rea::Json(
                                "type", "image",
                                "range", rea::JArray(300, 0, 400, 400),
                                "path", "image_1.xxx"
                            ),
                            "shp_0", rea::Json(
                                "type", "poly",
                                "points", rea::JArray(QJsonArray(),
                                                    rea::JArray(0, 0, 300, 0, 300, 400, 0, 400, 0, 0),
                                                    rea::JArray(10, 10, 50, 50, 10, 50, 10, 10))
                            )
                         )
    //modification
    auto mdy = rea::Json(
        obj, "shp_0",
        key, ["text", "visible"],
        val, true,
        force, false,
        id, ""
    )
    //prepare scope                         
    auto scp = std::make_shared<rea::scopeCache>();
    scp->cache<QJsonObject>("model", mdl)
       ->cache<QHash<QString, QImage>>("image", imgs)
       ->cache<QJsonObject>("image_data", img_data)
       ->cache<QJsonArray>("image_path", img_path)
       ->cache<QJsonObject>("storage_config", cfg);
    //run
    rea::pipeline::instance()->run<QJsonArray>("updateQSGAttr_testbrd", rea::JArray(mdy), "", scp); 

```
</br>

* **QSGAttrUpdated_ + name**  
    - output the updated attribute modifications  
    - type: pipe  
    - pipeline: c++; listener    
    - output: QJsonArray  
</br>

* **updateQSGCtrl_ + name**  
    - remove the old plugins and install the new plugin  
    - type: pipe  
    - pipeline: c++; trigger  
    - input: QJsonArray  
    - output: input  
_sample_:  
```
rea::pipeline::instance()->run<QJsonArray>("updateQSGCtrl_testbrd",{rea::Json(type, "select")})  //switch to select mode
```  
</br>

# Sample
```
import QSGBoard 1.0
QSGBoard{
    name: "testbrd"
    plugins: [{type: "transform"}]  //plugin id
    anchors.fill: parent
    Component.onDestruction: {
        beforeDestroy()  //it is necessary for safely destroy qsgBoard
    }
}
```  
</br>

# Reference
[qsgModel](qsgModel.md)  
[qsgBoardPlugin](qsgBoardPlugin.md)