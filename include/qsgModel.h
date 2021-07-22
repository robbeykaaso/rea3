#ifndef REA_IMAGEMODEL_H_
#define REA_IMAGEMODEL_H_

#include "rea.h"
#include <memory>
#include <QJsonObject>
#include <QSGNode>
#include <QQuickWindow>
#include <QSGSimpleTextureNode>

namespace rea {

using pointList = std::vector<QPointF>;
using IUpdateQSGAttr = std::function<void(QSGNode*)>;

DSTDLL QRectF calcBoundBox(const pointList& aPoints);

class qsgModel;

class DSTDLL qsgObject : public QJsonObject{
public:
    qsgObject(const QJsonObject& aConfig) : QJsonObject(aConfig){

    }
    qsgObject(qsgObject&&) = default;
    virtual ~qsgObject(){

    }
    virtual std::vector<QSGNode*> getQSGNodes(QQuickItem* aWindow = nullptr, QSGNode* = nullptr, QSGTransformNode* = nullptr){
        if (aWindow)
            m_window = aWindow;
        return std::vector<QSGNode*>();
    }
    virtual void removeQSGNodes();
    virtual void transformChanged();
    virtual IUpdateQSGAttr updateQSGAttr(const QString& aModification);
    virtual bool bePointSelected(double, double);
    virtual void setParent(qsgModel* aParent) {m_parent = aParent;}
    static QMatrix4x4 getTransform(const QJsonObject& aConfig);
    QRectF getBoundBox() {return m_bound;}
protected:
    void checkTextVisible();
    QJsonObject getTextConfig();
    void updateTextLocation(const QJsonObject& aTextConfig);
    void updateTextValue(const QJsonObject& aTextConfig);
    QString getText();
    QColor getColor();
    virtual QSGNode* getRootQSGNode(){return nullptr;}
    virtual void checkColor();
    void checkCaption();
    virtual void checkTransform();
    virtual void updateTransform();
    QQuickItem* m_window;
    qsgModel* m_parent;
    QSGSimpleTextureNode* m_text = nullptr;
    QRectF m_bound = QRectF(0, 0, 0, 0); //leftbottomrighttop
};

class DSTDLL imageObject : public qsgObject{
public:
    imageObject(const QJsonObject& aConfig);
    std::vector<QSGNode*> getQSGNodes(QQuickItem* aWindow = nullptr, QSGNode* aParent = nullptr, QSGTransformNode* aTransform = nullptr) override;
    void removeQSGNodes() override;
    IUpdateQSGAttr updateQSGAttr(const QString& aModification) override;
    QImage getImage();
protected:
    virtual QImage updateImagePath(bool aForce = false);
    QSGNode* getRootQSGNode() override {return m_node;}
    QSGSimpleTextureNode* m_node = nullptr;
    void updateTransform() override;
private:
    QString getPath();
    QRectF getRange(const QImage& aImage);
};

class DSTDLL shapeObject : public qsgObject{
public:
    shapeObject(const QJsonObject& aConfig) : qsgObject(aConfig){

    }
    std::vector<QSGNode*> getQSGNodes(QQuickItem* aWindow = nullptr, QSGNode* aParent = nullptr, QSGTransformNode* aTransform = nullptr) override;
    void removeQSGNodes() override;
    IUpdateQSGAttr updateQSGAttr(const QString& aModification) override;
protected:
    void setQSGGemoetry(const pointList& aPointList, QSGGeometryNode& aNode, unsigned int aMode,
                        const QString& aStyle, std::vector<uint32_t>* aIndecies = nullptr);
    void setQSGColor(QSGGeometryNode& aNode, const QColor& aColor);
    void checkArrowVisible(int aCount);
    void checkFaceOpacity();
    void checkColor() override;
    void checkWidth();
    void checkTransform() override;
    virtual size_t updateGeometry(bool aCalcPoints = true){return 0;}
    virtual void updateArrowLocation(){}
    void calcArrow(const QPointF& aStart, const QPointF& aEnd, QSGGeometryNode& aNode);
    void updateQSGFace(QSGGeometryNode& aNode, int aOpacity);
    QSGNode* getRootQSGNode() override {return m_outline;}
    virtual void toPoints() {}
    void updateTransform() override;
    std::vector<pointList> m_points;
    QSGGeometryNode* m_outline = nullptr;
    QSGGeometryNode* m_face = nullptr;
    std::vector<QSGGeometryNode*> m_holes;
    std::vector<QSGGeometryNode*> m_arrows;
private:
    void updateQSGFaceColor(QSGGeometryNode& aNode, int aOpacity);
    void updateArrowCount(int aCount);
protected:
    QString getLineStyle();
    int getWidth();
    int getFaceOpacity();
    QJsonObject getArrowConfig();
    bool getArrowVisible(const QJsonObject& aConfig);
    bool getPoleArrow(const QJsonObject& aConfig);
    QSGTransformNode* m_trans_node = nullptr;
};

class DSTDLL polyObject : public shapeObject{
public:
    polyObject(const QJsonObject& aConfig);
    std::vector<QSGNode*> getQSGNodes(QQuickItem* aWindow = nullptr, QSGNode* aParent = nullptr, QSGTransformNode* aTransform = nullptr) override;
    void transformChanged() override;
    IUpdateQSGAttr updateQSGAttr(const QString& aModification) override;
    void toPoints() override;
protected:
    QJsonArray getPoints();
    size_t updateGeometry(bool aCalcPoints = true) override;
    void updateArrowLocation() override;
    void checkArrowPole();
    void checkGeometry();
private:
};

class DSTDLL ellipseObject : public shapeObject{
public:
    ellipseObject(const QJsonObject& aConfig);
    std::vector<QSGNode*> getQSGNodes(QQuickItem* aWindow = nullptr, QSGNode* aParent = nullptr, QSGTransformNode* aTransform = nullptr) override;
    void transformChanged() override;
    IUpdateQSGAttr updateQSGAttr(const QString& aModification) override;
    void toPoints() override;
protected:
    size_t updateGeometry(bool aCalcPoints = true) override;
    void updateArrowLocation() override;
private:
    class l_qsgPoint3D : public QPointF{
    public:
        l_qsgPoint3D(float aX, float aY, float aZ) : QPointF(double(aX), double(aY)), z(aZ){}
        float z;
        std::shared_ptr<l_qsgPoint3D> nxt = nullptr;
    };
    void checkRadius();
    std::shared_ptr<l_qsgPoint3D> evalPoint(const QPointF& aRadius, double aParam);
    bool getCCW();
    QPointF getRadius();
};

class DSTDLL qsgModel : public QJsonObject{
public:
    qsgModel(){}
    qsgModel(const QJsonObject& aConfig);
    qsgModel(qsgModel&&) = default;
    ~qsgModel();

    void clearQSGObjects();
    QMap<QString, std::shared_ptr<qsgObject>> getQSGObjects() {return m_objects;}
    void show(QSGTransformNode* aTransform, QQuickItem* aWindow);
    IUpdateQSGAttr updateQSGAttr(const QJsonObject& aModification);
    void cacheImage(const QString& aName, const QImage& aImage);
    QHash<QString, QImage> getImageCache(){return m_image_cache;}
private:
    QString overwriteAttr(QJsonObject& aObject, const QJsonArray& aKeys, const QJsonValue&& aValue, bool aForce);
    void zoom(int aStep, const QPointF& aCenter, double aRatio = 0);
    void move(const QPointF& aDistance);
    void WCS2SCS();
    QTransform getTransform(bool aDeserialize = false);
    QJsonObject getObjects();
    void setObjects(const QJsonObject& aObjects);
    int getWidth();
    int getHeight();
    QJsonObject getArrowConfig();
    bool getArrowVisible(const QJsonObject& aConfig);
    bool getPoleArrow(const QJsonObject& aConfig);
    int getFaceOpacity();
    QJsonObject getTextConfig();
    bool getTextVisible(const QJsonObject& aConfig);
    QPoint getTextSize(const QJsonObject& aConfig);
    QString getTextLocation(const QJsonObject& aConfig);
    void setTransform();
    std::shared_ptr<qsgObject> addObject(const QJsonObject& aConfig);
    QMap<QString, std::shared_ptr<qsgObject>> m_objects;
    QTransform m_trans;
private:
    rea::pipe0* m_add_object;
    double m_max_ratio, m_min_ratio;
    QHash<QString, rea::pipe0*> m_creators;
    QHash<QString, QImage> m_image_cache;
    QQuickItem* m_window = nullptr;
    QSGTransformNode* m_trans_node = nullptr;
    friend qsgObject;
    friend shapeObject;
    friend polyObject;
    friend imageObject;
    friend ellipseObject;
};

}

#endif
