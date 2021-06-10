#ifndef REAL_FRAMEWORK_STORAGE0_H_
#define REAL_FRAMEWORK_STORAGE0_H_

#include "rea.h"
#include <QImage>
#include <QJsonObject>
#include <QString>
#include "util.h"

namespace rea {

class DSTDLL fsStorage {
public:
    fsStorage(const QString& aRoot = "");
    fsStorage(const fsStorage&&){}
    virtual ~fsStorage();
    virtual bool isValid() { return true; }
    virtual void initialize();
protected:
    virtual std::vector<QString> listFiles(const QString& aDirectory);
    virtual bool writeJsonObject(const QString& aPath, const QJsonObject& aData);
    virtual bool writeImage(const QString& aPath, const QImage& aData);
    virtual bool writeByteArray(const QString& aPath, const QByteArray& aData);
    virtual bool readJsonObject(const QString& aPath, QJsonObject& aData);
    virtual bool readImage(const QString& aPath, QImage& aData);
    virtual bool readByteArray(const QString& aPath, QByteArray& aData);
    virtual void deletePath(const QString& aPath);
    virtual std::vector<QString> getFileList(const QString& aPath);
    virtual QString stgRoot(const QString& aPath);
    void checkPath(const QString& aPath);
    void listAllFiles(const QString& aDirectory, std::vector<QString>& aList);
protected:
    QString m_root = "";
};

}  // namespace rea

#endif
