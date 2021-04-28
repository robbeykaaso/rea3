#ifndef REAL_FRAMEWORK_STORAGE0_H_
#define REAL_FRAMEWORK_STORAGE0_H_

#include "rea.h"
#include <QImage>
#include <QJsonObject>
#include <QString>
#include "util.h"

namespace rea {

class DSTDLL QSImage : public QObject, public QImage{
    Q_OBJECT
public:
    QSImage() : QImage() {}
    QSImage(const QString &fileName, const char *format = nullptr) : QImage(fileName, format){}
    QSImage(const QSImage&) {}
    QSImage& operator=(const QSImage&){return *this;}
    Q_INVOKABLE QString uri();
};

class DSTDLL storageCache : public QObject, public std::enable_shared_from_this<storageCache>{
    Q_OBJECT
public:
    storageCache(){}
    storageCache(const storageCache& aCopy){
        m_data = aCopy.m_data;
        m_tags = aCopy.m_tags;
        m_cur = aCopy.m_cur;
    }
    template<typename T>
    std::shared_ptr<storageCache> cache(const T& aData, const QString& aTag = ""){
        m_data.push_back(std::make_shared<stream<T>>(aData));
        m_tags.push_back(aTag);
        return shared_from_this();
    }

    std::shared_ptr<storageCache> reset(){
        m_cur = - 1;
        return shared_from_this();
    }

    template<typename T>
    bool nextData(T& aData, QString& aTag){
        m_cur++;
        if (m_cur > int(m_data.size()) - 1){
            m_cur = - 1;
            return false;
        }
        aData = std::dynamic_pointer_cast<stream<T>>(m_data.at(size_t(m_cur)))->data();
        aTag = m_tags.at(size_t(m_cur));
        return true;
    }
    template<typename T>
    bool nextData(T& aData){
        QString tg;
        return nextData<T>(aData, tg);
    }

    Q_INVOKABLE QVariant cache(const QVariant& aData, const QString& aTag = "");
    Q_INVOKABLE QVariant nextQData();
private:
    std::vector<std::shared_ptr<stream0>> m_data;
    std::vector<QString> m_tags;
    int m_cur = - 1;
};

class DSTDLL fsStorage {
 public:
  fsStorage(const QString& aRoot = "");
  fsStorage(const fsStorage&&){}
  virtual ~fsStorage();
  virtual bool isValid() { return true; }

 protected:
  virtual std::vector<QString> listFiles(const QString& aDirectory);
  virtual bool writeJsonObject(const QString& aPath, const QJsonObject& aData);
  virtual bool writeSImage(const QString& aPath, const QSImage& aData);
  virtual bool writeByteArray(const QString& aPath, const QByteArray& aData);
  virtual QJsonObject readJsonObject(const QString& aPath);
  virtual QSImage readSImage(const QString& aPath);
  virtual QByteArray readByteArray(const QString& aPath);
  virtual void deletePath(const QString& aPath);
  virtual std::vector<QString> getFileList(const QString& aPath);
  virtual QString stgRoot(const QString& aPath);
  void checkPath(const QString& aPath);

 protected:
  QString m_root;
 private:
     void listAllFiles(const QString& aDirectory, std::vector<QString>& aList);
};

#define READSTORAGE(aType) \
    rea::pipeline::instance()->add<bool, pipeParallel>([this](rea::stream<bool>* aInput) { \
        QString pth; \
        Q##aType tmp; \
        auto dt = aInput->scope()->data<std::shared_ptr<storageCache>>("data"); \
        if (dt->reset()->nextData(tmp, pth)){ \
            dt->cache(read##aType(pth), pth); \
            aInput->setData(true); \
        }else \
            aInput->setData(false); \
        aInput->out(); \
    }, rea::Json("name", m_root + STR(read##aType)))

#define WRITESTORAGE(aType) \
    rea::pipeline::instance()->add<bool, rea::pipeParallel>([this](rea::stream<bool>* aInput){ \
        QString pth; \
        Q##aType dt; \
        if (aInput->scope()->data<std::shared_ptr<storageCache>>("data")->reset()->nextData(dt, pth)){ \
            aInput->setData(write##aType(pth, dt))->out(); \
        }else \
            aInput->setData(false)->out(); \
}, rea::Json("name", m_root + STR(write##aType)))

}  // namespace rea

#endif
