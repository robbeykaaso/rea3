#include "rea.h"
#include "optGraph.h"
#include "qsgModel.h"
#include <QImage>

//stream->data(): bool
//stream->scope():
//  "param": QJsonObject
//  "images": std::vector<QImage>
//  "results": std::vector<std::shared_ptr<rea::stream0>>
//  "from": QString
//{
//    "id0": {
//        "type": "setImageOpacity",
//        "input": 0,
//        "value": 22,
//        "imageIndex": [],
//        "thread": 2,
//        "next": ["id2", "id3"]
//    },
//    "id1": {
//        "type": "convertFormat",
//        "input": 0,
//        "origin": "",
//        "target": "",
//        "imageIndex": [],
//        "thread": 2,
//        "next": ["id2", "id3"]
//    },
//    "id2": {
//        "type": "updateImagePath",
//        "value": ["img_id"],
//        "imageIndex": [],
//        "view": "xxx"
//    },
//    "id3": {
//        "type": "showOneImage",
//        "imageIndex": 0,
//        "view": "xxx"
//    },
//    "id4": {
//        "type": "inputImage",
//        "input": 0,
//        "view": "xxx", //optional
//        "file": "xxx", //optional
//        "next": []
//    },
//    "id5": {
//        "seq": ["id0", "id1"]
//    }
//}

namespace rea {

operatorGraph::operatorGraph(const QString& aID){
    m_name = aID;
    if (m_name == "")
        m_name = rea::generateUUID();
}

operatorGraph::~operatorGraph(){
    for (auto i : m_pipes)
        rea::pipeline::instance()->remove(i);
}

void operatorGraph::build(const QJsonObject& aConfig){
    QHash<QString, int> merges;
    for (auto i : aConfig.keys()){
        auto nxts = aConfig.value(i).toObject().value("next").toArray();
        for (auto j : nxts){
            auto nm = j.toString() + m_name;
            merges.insert(nm, merges.value(nm, 0) + 1);
        }
    }
    for (auto i : aConfig.keys()){
        auto cfg = aConfig.value(i).toObject();
        m_pipes.push_back(i + m_name);

        auto operate = [i, cfg, this](rea::stream<bool>* aInput){
            aInput->scope()->cache("param", cfg);
            rea::pipeline::instance()->run("operatorGraphRunning", i, "", std::make_shared<rea::scopeCache>(rea::Json("progress", 0, "id", m_name)));
            rea::pipeline::instance()->call<bool>(cfg.value("type").toString(), std::dynamic_pointer_cast<rea::stream<bool>>(aInput->shared_from_this()));
            rea::pipeline::instance()->run("operatorGraphRunning", i, "", std::make_shared<rea::scopeCache>(rea::Json("progress", 100, "id", m_name)));
            if (cfg.contains("next")){
                auto nxts = cfg.value("next").toArray();
                for (auto j : nxts)
                    aInput->outs(aInput->data(), j.toString() + m_name)->scope()->cache("from", i);
            }
        };
        auto nm = i + m_name;
        if (merges.value(nm, 0) > 1){
            rea::pipeline::instance()->add<bool, pipeMerge>(operate, rea::Json(cfg, "name", nm, "thread", cfg.value("thread").toInt()));
        }else
            rea::pipeline::instance()->add<bool>(operate, rea::Json("name", nm, "thread", cfg.value("thread").toInt()));

        if (cfg.contains("input"))
            m_starts.push_back(std::pair<int, QString>(cfg.value("input").toInt(), nm));
    }
    std::sort(m_starts.begin(), m_starts.end(), [](const std::pair<int, QString> & a, const std::pair<int, QString> & b) -> bool{
        return a.first < b.first;
    });
}

void operatorGraph::run(std::vector<QImage> aImages){
    for (auto i : m_starts)
        rea::pipeline::instance()->run<bool>(i.second, true, "", std::make_shared<rea::scopeCache>()->cache("images", aImages)->cache("graph", shared_from_this()));
}

void operatorGraph::run(){
    for (auto i : m_starts)
        rea::pipeline::instance()->run<bool>(i.second, true, "", std::make_shared<rea::scopeCache>()->cache("graph", shared_from_this()));
}

//    {
//        "type": "setImageOpacity",
//        "input": 0,
//        "value": 22,
//        "imageIndex": [],
//        "thread": 2,
//        "next": ["output1", "output2"]
//    },
static rea::regPip<bool> setImageOpacity([](rea::stream<bool>* aInput){
    auto prms = aInput->scope()->data<QJsonObject>("param");
    auto idxes = prms.value("imageIndex").toArray();
    auto imgs = aInput->scope()->data<std::vector<QImage>>("images");

    auto a = prms.value("value").toInt();
    if (a >= 0 && a <= 255){
        aInput->setData(true);
        for (auto i : idxes){
            auto idx = size_t(i.toInt());
            auto img = imgs.at(idx);
            auto img2 = img;
            if (img.format() == QImage::Format_RGB32 || img.format() == QImage::Format_RGB888)
                img2 = QImage(img.size(), QImage::Format_ARGB32);
            if (img2.format() != QImage::Format_ARGB32)
                return;
            for (auto x = 0; x < img.width(); x++)
                for (auto y = 0; y < img.height(); y++){
                    auto clr = img.pixelColor(x, y);
                    img2.setPixelColor(x, y, QColor(clr.red(), clr.green(), clr.blue(), int(a)));
                }
            imgs[idx] = img2;
        }
        aInput->scope()->cache("images", imgs);
    }else
        aInput->setData(false);
    aInput->out();
}, rea::Json("name", "setImageOpacity"));

//    {
//      "type": "updateImagePath",
//      "value": ["img_id"],
//      "imageIndex": [],
//      "view": "xxx"
//    },
static rea::regPip<bool> updateImagePath([](rea::stream<bool>* aInput){
    auto prms = aInput->scope()->data<QJsonObject>("param");
    auto idxes = prms.value("imageIndex").toArray();
    auto imgs = aInput->scope()->data<std::vector<QImage>>("images");
    auto nms = prms.value("value").toArray();
    for (auto i = 0; i < idxes.size(); ++i){
        QHash<QString, QImage> imgs0;
        auto idx = idxes[i].toInt();
        auto nm = nms[i].toString();
        imgs0.insert(nm, imgs.at(size_t(idx)));
        auto mdy = rea::JArray(rea::Json("obj", nm, "key", rea::JArray("path"), "val", nm, "force", true));
        aInput->outs(mdy, "updateQSGAttr_" + prms.value("view").toString())->scope(true)->cache("image", imgs0);
    }
}, rea::Json("name", "updateImagePath"));

//{
//    "type": "inputImage",
//    "input": 0,
//    "view": "xxx",
//    "next": ["setImageOpacity"]
//}
static rea::regPip<bool> inputImage([](rea::stream<bool>* aInput){
    auto prms = aInput->scope()->data<QJsonObject>("param");
    auto imgs = aInput->scope()->data<std::vector<QImage>>("images");
    if (prms.contains("view")){
        auto mdl = rea::in<rea::qsgModel*>(nullptr, "", nullptr, true)->asyncCall("getQSGModel_" + prms.value("view").toString(), false)->data();
        auto objs = mdl->getQSGObjects();
        for (auto i : objs.keys())
            if (i.startsWith("img_"))
                imgs.push_back(reinterpret_cast<rea::imageObject*>(objs.value(i).get())->getImage());
    }
    aInput->scope()->cache("images", imgs);
    aInput->out();
}, rea::Json("name", "inputImage"));

//{
//    "type": "showOneImage",
//    "imageIndex": 0,
//    "view": "reagrid2_ide_image"
//}
static rea::regPip<bool> showOneImage([](rea::stream<bool>* aInput){
    auto prms = aInput->scope()->data<QJsonObject>("param");
    auto imgs = aInput->scope()->data<std::vector<QImage>>("images");
    auto img_idx = prms.value("imageIndex").toInt();
    auto img = imgs.at(size_t(img_idx));
    auto vw = prms.value("view").toString();
    auto inf = vw.mid(QString("reagrid").length(), vw.length());
    auto infs = inf.split("_ide_");
    auto tp = infs[1];
    if (tp == "img")
        tp = "image";

    aInput->outs<QString>("result", "qml_modelOpened")
            ->scope(true)
            ->cache<bool>("noCache", true)
            ->cache<QString>("type", tp)
            ->cache<double>("index", infs[0].toInt());

    QHash<QString, QImage> imgs0;
    imgs0.insert("img_0", img);
    auto cfg = rea::Json("id", "img_0",
                         "width", img.width() ? img.width() : 600,
                         "height", img.height() ? img.height() : 600,
                         "max_ratio", 100,
                         "min_ratio", 0.01,
                         "objects", rea::Json(
                                        "img_0", rea::Json(
                                                     "type", "image",
                                                     "range", rea::JArray(0, 0, img.width(), img.height()),
                                                     "path", "img_0")));
    aInput->outs<QJsonArray>(QJsonArray(), "updateQSGAttr_" + vw)
            ->scope(true)
            ->cache<QJsonObject>("model", cfg)
            ->cache<QHash<QString, QImage>>("image", imgs0);
}, rea::Json("name", "showOneImage"));

}
