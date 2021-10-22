#include "storage.h"
#include "rea.h"
#include <filesystem>
#include <QFile>
#include <QJsonDocument>
#include <QDir>
#include <QBuffer>
#include <QVector>
#include <QDateTime>

namespace rea2 {

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
    auto pth = stgRoot(aPath);
    if (aPath.indexOf(".") >= 0)
        QDir().remove(pth);
    else{
        if (std::filesystem::is_directory(std::filesystem::u8path(pth.toStdString())))
            QDir(pth).removeRecursively();
        else
            QDir().remove(pth);
    }
}

long long fsStorage::lastModifiedTime(const QString& aPath){
    QFileInfo inf(aPath);
    return inf.lastModified().toMSecsSinceEpoch();
}

void fsStorage::initialize(){

#define READSTORAGE(aType) \
    rea2::pipeline::instance()->add<bool, rea2::pipePartial>([this](rea2::stream<bool>* aInput) { \
        Q##aType dt; \
        auto ret = read##aType(aInput->scope()->data<QString>("path"), dt); \
        aInput->scope()->cache("data", dt); \
        aInput->setData(ret)->out(); \
    }, rea2::Json("name", m_root + STR(read##aType), "thread", 10))

#define WRITESTORAGE(aType) \
    rea2::pipeline::instance()->add<bool, rea2::pipePartial>([this](rea2::stream<bool>* aInput){ \
        aInput->setData(write##aType(aInput->scope()->data<QString>("path"), aInput->scope()->data<Q##aType>("data")))->out(); \
}, rea2::Json("name", m_root + STR(write##aType), "thread", 11))

//https://blog.csdn.net/github_37382319/article/details/104723421 for file system
    READSTORAGE(JsonObject);
    READSTORAGE(ByteArray);
    READSTORAGE(Image);
    WRITESTORAGE(JsonObject);
    WRITESTORAGE(ByteArray);
    WRITESTORAGE(Image);

    rea2::pipeline::instance()->add<QString, pipePartial>([this](rea2::stream<QString>* aInput){
        auto fls = listFiles(aInput->data());
        aInput->scope()->cache("data", fls);
        aInput->out();
    }, rea2::Json("name", m_root + "listFiles"));

    rea2::pipeline::instance()->add<QString, pipePartial>([this](rea2::stream<QString>* aInput){
        std::vector<QString> fls;
        listAllFiles(aInput->data(), fls);
        aInput->scope()->cache("data", fls);
        aInput->out();
    }, rea2::Json("name", m_root + "listAllFiles"));

    rea2::pipeline::instance()->add<QString, pipePartial>([this](rea2::stream<QString>* aInput){
        deletePath(aInput->data());
        aInput->out();
    }, rea2::Json("name", m_root + "deletePath", "thread", 11));

    rea2::pipeline::instance()->add<QString, pipePartial>([this](rea2::stream<QString>* aInput){
        aInput->outs(lastModifiedTime(aInput->data()));
    }, rea2::Json("name", m_root + "lastModified"));
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
    std::vector<QString> ret;
    if (std::filesystem::is_directory(std::filesystem::u8path(aDirectory.toStdString()))){
        auto path = std::filesystem::u8path(stgRoot(aDirectory).toStdString());
        try{
            for (const auto & entry : std::filesystem::directory_iterator(path)){
                QString dir = QString::fromLocal8Bit(entry.path().string().data());
                dir = dir.split(aDirectory + (aDirectory.endsWith("/") ? "" : "\\")).back();
                ret.push_back(dir);
            }
        }catch(...){

        }
    }
    //QDir dir(stgRoot(aDirectory));
    //auto lst = dir.entryList();
    //for (auto i : lst)
    //    ret.push_back(i);
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
