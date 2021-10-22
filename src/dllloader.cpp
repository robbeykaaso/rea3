#include "rea.h"
#include <QDir>
#include <QApplication>
#include <QQmlApplicationEngine>

namespace rea2 {

#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#include <direct.h>
//copy from: https://blog.csdn.net/adong76/article/details/39432467
void getAllFormatFiles0( std::string path, std::vector<std::string>& files, std::string format)
{
    //文件句柄
    long long   hFile   =   0;
    //文件信息
    struct _finddata_t fileinfo;
    std::string p;
    if((hFile = _findfirst(p.assign(path).append("\\*" + format).c_str(),&fileinfo)) !=  -1)
    {
        do
        {
            if((fileinfo.attrib &  _A_SUBDIR))
            {
                if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)
                {
                    //files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
                    getAllFormatFiles0( p.assign(path).append("\\").append(fileinfo.name), files,format);
                }
            }
            else
            {
                files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
            }
        }while(_findnext(hFile, &fileinfo)  == 0);

        _findclose(hFile);
    }
}

void loadExtendLibraries(){
    std::vector<std::string> m_list;
    getAllFormatFiles0((QApplication::applicationDirPath() + "/plugin").toStdString(), m_list, ".dll");
    for (auto i : m_list){
        auto hdl = LoadLibrary(i.data());
        std::cout << "load: " << i << " " << (hdl == NULL ? "fail" : "success") << std::endl;
    }
}
#else
#include <unistd.h>
#endif

/*REGISTERPipe(registerQMLClass, regDynamicDlls, [](std::shared_ptr<streamData> aInput){

}, 0);*/

static regPip<QQmlApplicationEngine*> reg_dynamic_dll([](stream<QQmlApplicationEngine*>* aInput){
    auto cfg = aInput->scope()->data<QJsonObject>("rea-dll");
    if (cfg.value("use").toBool(true)) //ref from: https://stackoverflow.com/questions/25403363/how-to-implement-a-singleton-provider-for-qmlregistersingletontype
        loadExtendLibraries();
    aInput->out();
}, rea2::Json("name", "initRea"));

static regPip<QString, pipePartial> reg_dynamic_qml([](stream<QString>* aInput){
    std::vector<std::string> m_list;
    getAllFormatFiles0((QDir::currentPath() + "/plugin/" + aInput->data()).toStdString(), m_list, ".qml");
    QJsonArray qmls;
    for (auto i : m_list){
        auto pth = QString::fromStdString(i);
        pth.replace('\\', '/');
        qmls.push_back("file:" + pth);
    }
    aInput->outs<QJsonArray>(qmls);
}, Json("name", "loadDynamicQMLs", "external", getDefaultQMLPipelineName()));

}

