#include "storage.h"
#include "rea.h"
#include <QFile>
#include <QJsonDocument>
#include <QDir>
#include <QBuffer>
#include <QVector>

namespace rea {

QString QSImage::uri(){
    return "data:image/png;base64, " + rea::QImage2Base64(*this);
}

QVariant storageCache::cache(const QVariant& aData, const QString& aTag){
    if (aData.type() == QVariant::Type::Map)
        cache(aData.toJsonObject(), aTag);
    else if (aData.type() == QVariant::Type::List)
        cache(aData.toJsonArray(), aTag);
    else if (aData.type() == QVariant::Type::String)
        cache(aData.toString(), aTag);
    else if (aData.type() == QVariant::Type::Bool)
        cache(aData.toBool(), aTag);
    else if (aData.type() == QVariant::Type::Double)
        cache(aData.toDouble(), aTag);
    else{
        std::cout << aData.type() << std::endl;
        assert(0);
    }
    return QVariant::fromValue(this);
}

QVariant storageCache::nextQData(){
    m_cur++;
    if (m_cur > int(m_data.size()) - 1){
        m_cur = - 1;
        return QVariant();
    }
    auto tmp = m_data.at(size_t(m_cur))->QData();
    tmp = QVariant::fromValue<QObject*>(this);
    return tmp;
}

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

bool fsStorage::writeSImage(const QString& aPath, const QSImage& aData){
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

QJsonObject fsStorage::readJsonObject(const QString& aPath){
    QJsonObject ret;
    QFile fl(stgRoot(aPath));
    if (fl.open(QFile::ReadOnly)){
        QJsonDocument doc = QJsonDocument::fromJson(fl.readAll());
        ret = doc.object();
        fl.close();
    }else{
//        qDebug() << "read json: " + aPath + " failed!\n";
    }
    return ret;
}

QSImage fsStorage::readSImage(const QString& aPath){
    return QSImage(stgRoot(aPath));
}

QByteArray fsStorage::readByteArray(const QString& aPath){
    QByteArray ret;
    QFile fl(stgRoot(aPath));
    if (fl.open(QFile::ReadOnly)){
        ret = fl.readAll();
        fl.close();
    }else{
//        qDebug() << "read bytearray: " + aPath + " failed!\n";
    }
    return ret;
}

void fsStorage::deletePath(const QString& aPath){
    if (aPath.indexOf(".") >= 0)
        QDir().remove(stgRoot(aPath));
    else
        QDir(stgRoot(aPath)).removeRecursively();
}

std::vector<QString> fsStorage::getFileList(const QString& aPath){
    std::vector<QString> ret;
    auto pth = stgRoot(aPath);
    QDir dir(pth);
    auto lst = dir.entryList();
    for (auto i : lst)
        if (i != "." && i != ".."){
            if (i.indexOf(".") >= 0)
                ret.push_back(aPath + "/" + i);
            else{
                auto clst = getFileList(aPath + "/" + i);
                ret.insert(ret.end(), clst.begin(), clst.end());
            }
        }
    return ret;
}

fsStorage::fsStorage(const QString& aRoot){
    m_root = aRoot;

    pipeline::instance()->supportType<std::shared_ptr<storageCache>>([](stream0* aInput){
        auto ret = reinterpret_cast<stream<std::shared_ptr<storageCache>>*>(aInput)->data().get();
        return QVariant::fromValue<QObject*>(new storageCache(*ret));
    });

    READSTORAGE(JsonObject);
    READSTORAGE(ByteArray);
    //READSTORAGE(SImage);

    rea::pipeline::instance()->add<bool, pipeParallel>([this](rea::stream<bool>* aInput) { \
        QString pth;
        QSImage tmp;
        auto dt = aInput->scope()->data<std::shared_ptr<storageCache>>("data"); \
        if (dt->reset()->nextData(tmp, pth)){
            dt->cache(readSImage(pth), pth);
            aInput->setData(true);
        }else
            aInput->setData(false);
        aInput->out();
    }, rea::Json("name", m_root + "readSImage"));

    WRITESTORAGE(JsonObject);
    WRITESTORAGE(ByteArray);
    WRITESTORAGE(SImage);

    rea::pipeline::instance()->add<QString, pipePartial>([this](rea::stream<QString>* aInput){
        auto fls = listFiles(aInput->data());
        QJsonArray ret;
        for (auto i : fls)
            ret.push_back(i);
        aInput->scope()->cache("data", std::make_shared<storageCache>()->cache(ret));
        aInput->out();
    }, rea::Json("name", m_root + "listFiles"));

    rea::pipeline::instance()->add<QString, pipePartial>([this](rea::stream<QString>* aInput){
        std::vector<QString> fls;
        listAllFiles(aInput->data(), fls);
        QJsonArray ret;
        for (auto i : fls)
            ret.push_back(i);
        aInput->scope()->cache("data", std::make_shared<storageCache>()->cache(ret));
        aInput->out();
    }, rea::Json("name", m_root + "listAllFiles"));

    rea::pipeline::instance()->add<QString, pipeParallel>([this](rea::stream<QString>* aInput){
        deletePath(aInput->data());
        aInput->out();
    }, rea::Json("name", m_root + "deletePath"));
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
