#include "storage.h"
#include "rea.h"
#include <QFile>
#include <QJsonDocument>
#include <QDir>
#include <QBuffer>
#include <QVector>

namespace rea {

void fsStorage::checkPath(const QString &aPath){
    auto dirs = aPath.split("/");
    QDir dir;
    QString origin = "";
    for (int i = 0; i < dirs.size() - 1; ++i){
        if (i > 0)
            origin += "/";
        origin += dirs[i];
        if (!dir.exists(origin))
            dir.mkdir(origin);
    }
}

bool fsStorage::writeJsonObject(const QString& aPath, const QJsonObject& aData){
    return writeByteArray(aPath, QJsonDocument(aData).toJson());
}

QString fsStorage::stgRoot(const QString& aPath){
    if (m_root == "")
        return aPath;
    else
        return m_root + "/" + aPath;
}

bool fsStorage::writeImage(const QString& aPath, const QImage& aData){
    auto pth = stgRoot(aPath);
    checkPath(pth);
    return aData.save(pth);
}

bool fsStorage::writeByteArray(const QString& aPath, const QByteArray& aData){
    auto pth = stgRoot(aPath);
    checkPath(pth);
    QFile fl(pth);
    if (fl.open(QFile::WriteOnly)){
        fl.write(aData);
        fl.close();
        return true;
    }
    return false;
}

bool fsStorage::readJsonObject(const QString& aPath, QJsonObject& aData){
    QFile fl(stgRoot(aPath));
    if (fl.open(QFile::ReadOnly)){
        QJsonDocument doc = QJsonDocument::fromJson(fl.readAll());
        aData = doc.object();
        fl.close();
        return true;
    }
    return false;
}

bool fsStorage::readImage(const QString& aPath, QImage& aData){
    aData = QImage(stgRoot(aPath));
    return !aData.isNull();
}

bool fsStorage::readByteArray(const QString& aPath, QByteArray& aData){
    QFile fl(stgRoot(aPath));
    if (fl.open(QFile::ReadOnly)){
        aData = fl.readAll();
        fl.close();
        return true;
    }
    return false;
}

void fsStorage::deletePath(const QString& aPath){
    if (aPath.indexOf(".") >= 0)
        QDir().remove(stgRoot(aPath));
    else
        QDir(stgRoot(aPath)).removeRecursively();
}

void fsStorage::initialize(){

#define READSTORAGE(aType) \
    rea::pipeline::instance()->add<bool, rea::pipePartial>([this](rea::stream<bool>* aInput) { \
        Q##aType dt; \
        auto ret = read##aType(aInput->scope()->data<QString>("path"), dt); \
        aInput->scope()->cache("data", dt); \
        aInput->setData(ret)->out(); \
    }, rea::Json("name", m_root + STR(read##aType), "thread", 10))

#define WRITESTORAGE(aType) \
    rea::pipeline::instance()->add<bool, rea::pipePartial>([this](rea::stream<bool>* aInput){ \
        aInput->setData(write##aType(aInput->scope()->data<QString>("path"), aInput->scope()->data<Q##aType>("data")))->out(); \
}, rea::Json("name", m_root + STR(write##aType), "thread", 11))

//https://blog.csdn.net/github_37382319/article/details/104723421 for file system
    READSTORAGE(JsonObject);
    READSTORAGE(ByteArray);
    READSTORAGE(Image);
    WRITESTORAGE(JsonObject);
    WRITESTORAGE(ByteArray);
    WRITESTORAGE(Image);

    rea::pipeline::instance()->add<QString, pipePartial>([this](rea::stream<QString>* aInput){
        auto fls = listFiles(aInput->data());
        aInput->scope()->cache("data", fls);
        aInput->out();
    }, rea::Json("name", m_root + "listFiles"));

    rea::pipeline::instance()->add<QString, pipePartial>([this](rea::stream<QString>* aInput){
        std::vector<QString> fls;
        listAllFiles(aInput->data(), fls);
        aInput->scope()->cache("data", fls);
        aInput->out();
    }, rea::Json("name", m_root + "listAllFiles"));

    rea::pipeline::instance()->add<QString, pipePartial>([this](rea::stream<QString>* aInput){
        deletePath(aInput->data());
        aInput->out();
    }, rea::Json("name", m_root + "deletePath", "thread", 11));
}

fsStorage::fsStorage(const QString& aRoot){
    m_root = aRoot;
}

/*bool safetyWrite(const QString& aPath, const QByteArray& aData){
    QDir().mkdir("Temp");
    auto tmp = "Temp/" + aPath.mid(aPath.lastIndexOf("/") + 1, aPath.length());
    QFile fl(tmp);
    if (fl.open(QFile::WriteOnly)){
        fl.write(aData);
        fl.close();
        if (!MoveFileExA(tmp.toLocal8Bit().toStdString().data(), aPath.toLocal8Bit().toStdString().data(), MOVEFILE_REPLACE_EXISTING)){
            std::cout << "write file error: " << GetLastError() << std::endl;
            return false;
        }
        return true;
    }
    return false;
}*/

fsStorage::~fsStorage(){

}

std::vector<QString> fsStorage::listFiles(const QString& aDirectory){
    QDir dir(stgRoot(aDirectory));
    std::vector<QString> ret;
    auto lst = dir.entryList();
    for (auto i : lst)
        ret.push_back(i);
    return ret;
}

void fsStorage::listAllFiles(const QString& aDirectory, std::vector<QString>& aList){
    auto ret = listFiles(aDirectory);
    for (auto i : ret)
        if (i != "." && i != ".."){
            if (i.contains("."))
                aList.push_back(aDirectory + "/" + i);
            else
                listAllFiles(aDirectory + "/" + i, aList);
        }
}

}
