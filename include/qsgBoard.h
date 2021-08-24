#ifndef REA_IMAGEBOARD_H_
#define REA_IMAGEBOARD_H_

#include "qsgModel.h"
#include "util.h"
#include <QQuickItem>
#include <QQueue>

namespace rea {

class qsgBoardPlugin;

DSTDLL std::shared_ptr<scopeCache> qsgImages(std::initializer_list<std::pair<QString, QImage>> aImages);

class qsgBoard : public QQuickItem{
    Q_OBJECT
    Q_PROPERTY(QString name WRITE setName READ getName)
    Q_PROPERTY(QJsonArray plugins WRITE installPlugins READ getPlugins NOTIFY pluginsChanged)
public:
    Q_INVOKABLE void beforeDestroy();
public:
    explicit qsgBoard(QQuickItem *parent = nullptr);
    ~qsgBoard() override;
signals:
    void pluginsChanged();
protected:
    QSGNode *updatePaintNode(QSGNode* aOldNode, UpdatePaintNodeData* nodedata) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void hoverMoveEvent(QHoverEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    //QSGClipNode* m_clip_node = nullptr;
    QSGTransformNode* m_trans_node = nullptr;
    QQueue<std::shared_ptr<qsgModel>> m_models;
private:
    void setName(const QString& aName);
    void addUpdate(const IUpdateQSGAttr& aUpdate);
    QString getName() {return m_name;}
    void installPlugins(const QJsonArray& aPlugins);
    QJsonArray getPlugins();
    QString m_name;
    QMap<QString, std::shared_ptr<qsgBoardPlugin>> m_plugins;
    QJsonArray m_plugins_config;
    QQueue<IUpdateQSGAttr> m_updates;
    QQueue<int> m_updates_model_index;
    QJsonArray m_updates_modification;
    friend qsgBoardPlugin;
};

class DSTDLL qsgBoardPlugin{
public:
    qsgBoardPlugin(const QJsonObject& aConfig) {
        m_name = aConfig.value("name").toString();
        if (m_name == "")
            m_name = rea::generateUUID();
    }
    virtual ~qsgBoardPlugin() = default;
    virtual QString getName(qsgBoard* aParent = nullptr) {
        if (aParent)
            m_parent = aParent;
        return m_name;
    }
    virtual void beforeDestroy(){
        for (auto i : m_handles)
            i->removeQSGNodes();
        m_handles.clear();
    }
    virtual void keyPressEvent(QKeyEvent*){}
    virtual void mousePressEvent(QMouseEvent*){}
    virtual void mouseReleaseEvent(QMouseEvent*){}
    virtual void mouseMoveEvent(QMouseEvent*){}
    virtual void wheelEvent(QWheelEvent*){}
    virtual void hoverMoveEvent(QHoverEvent*){}
private:
    friend qsgBoard;
protected:
    QString getParentName() {return m_parent ? m_parent->getName() : "";}
    std::shared_ptr<qsgModel> getQSGModel() {return m_parent->m_models.size() > 0 ? m_parent->m_models.back() : nullptr;}
    void updateParent(IUpdateQSGAttr aUpdate){
        m_parent->addUpdate(aUpdate);
        m_parent->update();
    }
    void updateParent(std::vector<IUpdateQSGAttr> aUpdates){
        for (auto i : aUpdates)
            m_parent->addUpdate(i);
        m_parent->update();
    }
    QSGTransformNode* getTransNode(){
        return m_parent->m_trans_node;
    }
    qsgBoard* m_parent = nullptr;
    QString m_name;
protected:
    QString newShapeID(const QString& aSuffix = "shp_");
    std::shared_ptr<shapeObject> createEllipseHandle(QSGNode* aTransformNode, int aRadius, int aFace = - 1,
                                                     const QJsonArray& aCenter = rea::JArray(0, 0), const QString& aColor = "red");
    void updateHandlePos(size_t aIndex, const QPoint& aPos);
    void updateHandleRadius(size_t aIndex, int aRadius);
    std::vector<std::shared_ptr<shapeObject>> m_handles;
private:
    rea::qsgModel m_mdl;
};

class DSTDLL qsgPluginTransform : public qsgBoardPlugin{
public:
    qsgPluginTransform(const QJsonObject& aConfig) : qsgBoardPlugin(aConfig){}
    ~qsgPluginTransform() override;
    QString getName(qsgBoard* aParent = nullptr) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void hoverMoveEvent(QHoverEvent *event) override;
    QPoint scsPos(){return m_lastpos;}
    QPointF wcsPos(){return m_wcspos;}
    bool tryMoveWCS(QMouseEvent * event, Qt::MouseButton aFlag);
    void updatePos(const QPoint& aPos); 
protected:
    virtual QJsonObject getMenu();
    QPoint m_lastpos;
    QPointF m_wcspos;
};

}

#endif
