#include "qsgBoard.h"
#include "rea.h"
#include <QSGTransformNode>
#include <QTransform>
#include <QQmlApplicationEngine>

namespace rea{

QString qsgBoardPlugin::newShapeID(){
    return "shp_" + generateUUID();
}

std::shared_ptr<shapeObject> qsgBoardPlugin::createEllipseHandle(QSGNode* aTransformNode, int aRadius, int aFace, const QJsonArray& aCenter, const QString& aColor){
    m_handles.push_back(std::make_shared<rea::ellipseObject>(rea::Json(
        "type", "ellipse",
        "center", aCenter,
        "radius", rea::JArray(aRadius, aRadius),
        "width", 0,
        "color", aColor,
        "face", aFace)));
    m_handles.back()->setParent(&m_mdl);
    m_handles.back()->getQSGNodes(m_parent, aTransformNode->parent());
    return m_handles.back();
}

void qsgBoardPlugin::updateHandlePos(size_t aIndex, const QPoint& aPos){
    if (aIndex < m_handles.size()){
        auto hdl = m_handles[aIndex];
        hdl->insert("center", rea::JArray(aPos.x(), aPos.y()));
        updateParent(hdl->updateQSGAttr("center_"));
    }
}

void qsgBoardPlugin::updateHandleRadius(size_t aIndex, int aRadius){
    if (aIndex < m_handles.size()){
        auto hdl = m_handles[aIndex];
        hdl->insert("radius", rea::JArray(aRadius, aRadius));
        updateParent(hdl->updateQSGAttr("radius_"));
    }
}

qsgPluginTransform::~qsgPluginTransform(){
//    pipeline::remove("updateQSGPos_" + getParentName());
//    pipeline::remove("updateQSGMenu_" + getParentName());
}

QString qsgPluginTransform::getName(qsgBoard* aParent) {
    auto ret = qsgBoardPlugin::getName(aParent);
    pipeline::instance()->add<QJsonObject>([](stream<QJsonObject>* aInput){  //install new plugin may happen before old one is destroyed, so the format mode is used here
        aInput->out();
    }, rea::Json("name", "updateQSGPos_" + getParentName(), "replace", true));
    pipeline::instance()->add<QJsonObject>([](stream<QJsonObject>* aInput){
        aInput->out();
    }, rea::Json("name", "updateQSGMenu_" + getParentName(), "replace", true));
    rea::pipeline::instance()->find("QSGAttrUpdated_" + getParentName())
        ->nextF<QJsonArray>([this](rea::stream<QJsonArray>* aInput){
            auto dt = aInput->data();
            for (auto i : dt){
                if (i.toObject().value("type") == "zoom"){
                    auto mdl = getQSGModel();
                    if (mdl){
                        auto inv = getTransNode()->matrix().inverted();
                        aInput->outs<QJsonObject>(rea::Json("x", m_wcspos.x(),
                                                           "y", m_wcspos.y(),
                                                           "ratio", 100 / inv.data()[0],
                                                           "transform", mdl ? mdl->value("transform") : QJsonArray()), "updateQSGPos_" + getParentName());
                        break;
                    }
                }
            }
        aInput->out();
    }, "", rea::Json("name", "QSGTransformUpdated_" + getParentName()));  //the name is used to overwrite the same pipe of the old transform plugin
    return ret;
}

void qsgPluginTransform::wheelEvent(QWheelEvent *event){
    rea::pipeline::instance()->run<QJsonArray>("updateQSGAttr_" + getParentName(),
                                               {
                                                   rea::Json("key", rea::JArray("transform"),
                                                             "type", "zoom",
                                                             "dir", event->delta() < 0 ? - 1 : 1,
                                                             "center", rea::JArray(m_lastpos.x(), m_lastpos.y()))
                                               },
                                               "zoomWCS");
}

void qsgPluginTransform::mousePressEvent(QMouseEvent*){
    rea::pipeline::instance()->run<QJsonObject>("updateQSGMenu_" + getParentName(), QJsonObject());
}

void qsgPluginTransform::mouseReleaseEvent(QMouseEvent *event){
    auto mn = event->button() == Qt::RightButton ? rea::Json(getMenu(), "x", event->x(), "y", event->y()) : QJsonObject();
    rea::pipeline::instance()->run<QJsonObject>("updateQSGMenu_" + getParentName(), mn);
}

void qsgPluginTransform::mouseMoveEvent(QMouseEvent *event){
    tryMoveWCS(event, Qt::MiddleButton);
    updatePos(event->pos());
}

bool qsgPluginTransform::tryMoveWCS(QMouseEvent *event, Qt::MouseButton aFlag){
    auto ret = event->buttons().testFlag(aFlag);
    if (ret){
        auto cur = event->pos();
        rea::pipeline::instance()->run<QJsonArray>("updateQSGAttr_" + getParentName(),
                                                   {
                                                       rea::Json("key", rea::JArray("transform"),
                                                                 "type", "move",
                                                                 "del", rea::JArray(cur.x() - m_lastpos.x(), cur.y() - m_lastpos.y()))
                                                   },
                                                   "moveWCS");
    }
    return ret;
}

void qsgPluginTransform::hoverMoveEvent(QHoverEvent *event){
    updatePos(event->pos());
}

void qsgPluginTransform::updatePos(const QPoint &aPos){
    auto inv = getTransNode()->matrix().inverted();
    m_lastpos = aPos;
    m_wcspos = inv.map(m_lastpos);
    auto mdl = getQSGModel();
    rea::pipeline::instance()->run<QJsonObject>("updateQSGPos_" + getParentName(), rea::Json("x", m_wcspos.x(),
                                                                                 "y", m_wcspos.y(),
                                                                                 "ratio", 100 / inv.data()[0],
                                                                                 "transform", mdl ? mdl->value("transform") : QJsonArray()));
}

static rea::regPip<QJsonObject, rea::pipePartial> create_qsgboardplugin_transform([](rea::stream<QJsonObject>* aInput){
    aInput->scope()->cache<std::shared_ptr<qsgBoardPlugin>>("result", std::make_shared<qsgPluginTransform>(aInput->data()));
    aInput->out();
}, rea::Json("name", "create_qsgboardplugin_transform"));

void qsgBoard::beforeDestroy(){
    if (m_models.size() > 0){
        //auto lst = m_models.front();
        //lst->clearQSGObjects();
        m_models.clear();
        m_updates.clear();
        m_updates_model_index.clear();
        m_updates_modification = QJsonArray();
    }
    for (auto i : m_plugins)
        i->beforeDestroy();
}

qsgBoard::~qsgBoard(){
    rea::pipeline::instance()->remove("updateQSGAttr_" + m_name);
    rea::pipeline::instance()->remove("updateQSGCtrl_" + m_name);
    rea::pipeline::instance()->remove("QSGAttrUpdated_" + m_name);
}

std::shared_ptr<scopeCache> qsgImages(std::initializer_list<std::pair<QString, QImage>> aImages){
    return std::make_shared<scopeCache>()->cache("image", QHash<QString, QImage>(aImages));
}

qsgBoard::qsgBoard(QQuickItem *parent) : QQuickItem(parent)
{
    setAcceptedMouseButtons(Qt::LeftButton | Qt::MidButton | Qt::RightButton);
    setFlag(ItemHasContents, true);
    setAcceptHoverEvents(true);
    setSmooth(true);
    setAntialiasing(true);
    setClip(true);
    //connect(, QQuickWindow::beforeSynchronizing(), this, SLOT(beforeRender()))
}

void qsgBoard::addUpdate(const IUpdateQSGAttr& aUpdate){
    m_updates_model_index.push_back(m_models.size() - 1);
    m_updates.push_back(aUpdate);
}

void qsgBoard::setName(const QString& aName){
    m_name = aName;

    connect(this, &qsgBoard::heightChanged, [this](){
        if (m_models.size() > 0){
            addUpdate(m_models.back()->updateQSGAttr(rea::Json("key", rea::JArray("transform"), "type", "refresh")));
            update();
        }
    });

    connect(this, &qsgBoard::widthChanged, [this](){
        if (m_models.size() > 0){
            addUpdate(m_models.back()->updateQSGAttr(rea::Json("key", rea::JArray("transform"), "type", "refresh")));
            update();
        }
    });

    rea::pipeline::instance()->add<QJsonArray, pipeDelegate>([this](rea::stream<QJsonArray>* aInput){
        auto dt = aInput->data();
        for (auto i : dt)
            m_updates_modification.push_back(i);

        auto scp = aInput->scope();
        auto mdl = scp->data<QJsonObject>("model");
        if (!mdl.empty())
            m_models.push_back(std::make_shared<qsgModel>(mdl));

        if (m_models.size()){
            auto imgs = scp->data<QHash<QString, QImage>>("image");
            for (auto i : imgs.keys())
                m_models.back()->cacheImage(i, imgs.value(i));
            auto imgs_data = scp->data<QJsonObject>("image_data");
            for (auto i : imgs_data.keys())
                m_models.back()->cacheImage(i, QImage::fromData(QByteArray::fromBase64(imgs_data.value(i).toString().toLocal8Bit())));
            for (auto i : dt)
                addUpdate(m_models.back()->updateQSGAttr(i.toObject()));
            update();
        }else{
            rea::pipeline::instance()->run<QJsonArray>("QSGAttrUpdated_" + m_name, m_updates_modification);
            m_updates_modification = QJsonArray();
        }
        aInput->out();
    }, rea::Json("name", "updateQSGAttr_" + m_name, "delegate", "QSGAttrUpdated_" + m_name));

    rea::pipeline::instance()->add<QJsonArray>([](rea::stream<QJsonArray>* aInput){
        aInput->out();
    }, rea::Json("name", "QSGAttrUpdated_" + m_name));

    rea::pipeline::instance()->add<QJsonArray>([this](rea::stream<QJsonArray>* aInput){
        for (auto i : m_plugins)
            i->beforeDestroy();
        if (aInput->data().size() > 0){
            m_plugins.clear();
            installPlugins(aInput->data());
        }
        aInput->out();
    }, rea::Json("name", "updateQSGCtrl_" + m_name));
}

QJsonArray qsgBoard::getPlugins() {
    return m_plugins_config;
}

void qsgBoard::installPlugins(const QJsonArray& aPlugins){
    m_plugins_config = aPlugins;
    assert(!m_plugins.size());
    for (auto i : aPlugins){
        auto cfg = i.toObject();
        auto plg = in(cfg, "", nullptr, true)
                ->asyncCall("create_qsgboardplugin_" + cfg.value("type").toString())
                ->scope()
                ->data<std::shared_ptr<qsgBoardPlugin>>("result");
        m_plugins.insert(plg->getName(this), plg);
    }
}

QSGNode* qsgBoard::updatePaintNode(QSGNode* aOldNode, UpdatePaintNodeData*){
    auto ret = aOldNode;
    if (ret == nullptr){
        auto root_node = new QSGNode();
        root_node->setFlag(QSGNode::OwnedByParent);
        //clip_node->setClipRect(boundingRect());
        //clip_node->setIsRectangular(true);
        ret = root_node;

        m_trans_node = new QSGTransformNode();
        m_trans_node->setMatrix(QMatrix4x4(QTransform()));
        m_trans_node->setFlag(QSGNode::OwnedByParent);
        ret->appendChildNode(m_trans_node);
        //ret = m_trans_node;
    }
    if (m_models.size() > 1){
        while (m_updates.size() > 0 && m_updates_model_index.front() < m_models.size() - 1){
            if (m_updates.front())
                m_updates.front()(m_trans_node);
            m_updates.pop_front();
            m_updates_model_index.pop_front();
        }
        auto lst = m_models.front();
        lst->clearQSGObjects();
        while (m_models.size() > 1)
            m_models.pop_front();
    }
    if (m_models.size() == 1){
        m_models.front()->show(m_trans_node, this);
    }

    for (auto i : m_updates)
        if (i)
            i(m_trans_node);
    m_updates.clear();
    m_updates_model_index.clear();
    if (m_updates_modification.size() > 0){
        rea::pipeline::instance()->run<QJsonArray>("QSGAttrUpdated_" + m_name, m_updates_modification);
        m_updates_modification = QJsonArray();
    }
    return ret;
}

void qsgBoard::keyPressEvent(QKeyEvent *event){
    for (auto i : m_plugins)
        i->keyPressEvent(event);
}

void qsgBoard::mousePressEvent(QMouseEvent *event){
    for (auto i : m_plugins)
        i->mousePressEvent(event);
}

void qsgBoard::mouseMoveEvent(QMouseEvent *event){
    for (auto i : m_plugins)
        i->mouseMoveEvent(event);
}

void qsgBoard::mouseReleaseEvent(QMouseEvent *event){
    for (auto i : m_plugins)
        i->mouseReleaseEvent(event);
}

void qsgBoard::hoverMoveEvent(QHoverEvent *event){
    for (auto i : m_plugins)
        i->hoverMoveEvent(event);
}

void qsgBoard::wheelEvent(QWheelEvent *event){
    for (auto i : m_plugins)
        i->wheelEvent(event);
}

static rea::regPip<QQmlApplicationEngine*> reg_imageboard([](rea::stream<QQmlApplicationEngine*>* aInput){
    qmlRegisterType<qsgBoard>("QSGBoard", 1, 0, "QSGBoard");
    aInput->out();
}, rea::Json("name", "install1_QSGBoard"), "initRea");

}
