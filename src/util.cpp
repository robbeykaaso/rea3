#include "util.h"
#include "rea.h"
#include <QImage>
#include <QBuffer>
#include <QDir>
#include <QTime>
#include <QNetworkInterface>
#include <QProcess>
#include <QJsonDocument>
#include <QUuid>
#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <string>

namespace rea {

#define GUID_LEN 64

//https://www.cnblogs.com/keepsimple/p/3250245.html
QJsonObject Json(QJsonObject&& aTarget){
    return std::forward<QJsonObject>(aTarget);
};

void Json(QJsonObject&){

}

QJsonArray JArray(QJsonArray&& aTarget){
    return std::forward<QJsonArray>(aTarget);
};

void JArray(QJsonArray&){

}

QString generateUUID(){
    return QUuid::createUuid().toByteArray(QUuid::WithoutBraces);
}

QString getCWD(const char* aSuffix)
{
    QDir dir;
    if (!dir.exists("config_"))
        dir.mkdir("config_");
    auto ret = QDir::currentPath() + "/config_" + QString::fromStdString(aSuffix);
    return ret;
}

QString GetMachineFingerPrint(){
    static QString ret;
    if (ret == ""){
        auto ret0 = getWMIC("wmic cpu get processorid");
        auto ret2 = getWMIC("wmic baseboard get serialnumber");
        ret2.replace("/", "_");
        ret = ret0 + ret2;
    }
    return ret;
    /*QString localHostName = QHostInfo::localHostName();
    QHostInfo info = QHostInfo::fromName(localHostName);
    auto addr = info.addresses()[0].toString();
    return localHostName + "_" + addr;*/
}

int getRandom(int min,int max)
{
    qsrand(uint(QTime(0, 0, 0).secsTo(QTime::currentTime())));
    if (min == max)
        return 0;
    else
        return qrand()%(max-min);
}

QHostAddress GetLocalIP()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    QList<QHostAddress> rets;
    foreach (QHostAddress address, list)
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol){
            if (address.toString().indexOf("172") == 0){
                return address;
            }
            rets.push_back(address);
        }
    }
    return rets.size() > 0 ? rets.first() : QHostAddress::AnyIPv4;
}

//https://www.cnblogs.com/feiyangqingyun/p/10443149.html
QString getWMIC(const QString &cmd)
{
    //获取cpu名称：wmic cpu get Name
    //获取cpu核心数：wmic cpu get NumberOfCores
    //获取cpu线程数：wmic cpu get NumberOfLogicalProcessors
    //查询cpu序列号：wmic cpu get processorid
    //查询主板序列号：wmic baseboard get serialnumber
    //查询BIOS序列号：wmic bios get serialnumber
    //查看硬盘：wmic diskdrive get serialnumber
    QProcess p;
    p.start(cmd);
    p.waitForFinished();
    QString result = QString::fromLocal8Bit(p.readAllStandardOutput());
    QStringList list = cmd.split(" ");
    result = result.remove(list.last(), Qt::CaseInsensitive);
    result = result.replace("\r", "");
    result = result.replace("\n", "");
    result = result.simplified();
    return result;
}

QString int2Hex(int aInt){
    char hex[10];
    sprintf_s(hex, "%X", aInt);
    QString ch = QString::fromStdString(hex);
    if (ch.size() < 2)
        ch = "0" + ch;
    return ch;
}

QStringList parseJsons(const QString& aContent){
    QStringList ret;
    auto tt = aContent.split("}{");

    for (int i = 0; i < tt.size(); ++i){
        auto str = tt.at(i);
        if (i > 0)
            str = "{" + str;
        if (i < tt.size() - 1)
            str = str + "}";
        ret.push_back(str);
    }
    return ret;
}

QString QImage2Base64(const QImage& aImage){
    QBuffer bf;
    bf.open(QIODevice::WriteOnly);
    aImage.save(&bf, "png");
    auto ret = bf.data().toBase64();
    bf.close();
    return ret;
}

//https://stackoverflow.com/questions/29772271/c-opencv-convert-mat-to-base64-and-vice-versa
//https://stackoverflow.com/questions/180947/base64-decode-snippet-in-c
std::string base64_encode(const std::string &in) {

    std::string out;

    int val=0, valb=-6;
    for (uchar c : in) {
        val = (val<<8) + c;
        valb += 8;
        while (valb>=0) {
            out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val>>valb)&0x3F]);
            valb-=6;
        }
    }
    if (valb>-6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val<<8)>>(valb+8))&0x3F]);
    while (out.size()%4) out.push_back('=');
    return out;
}

std::string base64_decode(const std::string &in) {

    std::string out;

    std::vector<int> T(256,-1);
    for (int i=0; i<64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

    int val=0, valb=-8;
    for (uchar c : in) {
        if (T[c] == -1) break;
        val = (val<<6) + T[c];
        valb += 6;
        if (valb>=0) {
            out.push_back(char((val>>valb)&0xFF));
            valb-=8;
        }
    }
    return out;
}

void progress_display::restart(size_t expected_count_){
    //  Effects: display appropriate scale
    //  Postconditions: count()==0, expected_count()==expected_count_
    _count = _next_tic_count = _tic = 0;
    _expected_count                 = expected_count_;

    m_os << m_s1 << "0%   10   20   30   40   50   60   70   80   90   100%\n"
         << m_s2 << "|----|----|----|----|----|----|----|----|----|----|"
         << std::endl  // endl implies flush, which ensures display
         << m_s3;
    if (!_expected_count) _expected_count = 1;  // prevent divide by zero
}                                             // restart

void progress_display::display_tic(){
    // use of floating point ensures that both large and small counts
    // work correctly.  static_cast<>() is also used several places
    // to suppress spurious compiler warnings.
    size_t tics_needed = static_cast<size_t>(
        (static_cast<double>(_count) / _expected_count) * 50.0);
    do {
        m_os << '*' << std::flush;
    } while (++_tic < tics_needed);
    _next_tic_count = static_cast<size_t>((_tic / 50.0) * _expected_count);
    if (_count == _expected_count) {
        if (_tic < 51) m_os << '*';
        m_os << std::endl;
    }
    pipeline::instance()->run<QJsonObject>("updateCommandlineProgress", rea::Json("ratio", _tic / 51.0), QString::fromStdString(m_nm));
} // display_tic

}
