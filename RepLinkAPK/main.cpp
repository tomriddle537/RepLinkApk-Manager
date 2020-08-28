#include "mainwindow.h"
#include "apkinfoview.h"
#include <QDebug>
#include <QFileInfo>
#include <QApplication>
#include <QFileInfo>
#include <QSettings>
#include <QMessageBox>
#include <iostream>

#ifdef Q_OS_WIN
#include <windows.h>
QString shortPathName(const QString & file) // returns 8.3 file format from long path
{
    wchar_t* input=new wchar_t[file.size()+1];
    file.toWCharArray(input);
    input[file.size()]=L'\0'; // terminate string
    long length = GetShortPathName(input, NULL, 0);
    wchar_t* output=new wchar_t[length];
    GetShortPathName(input,output,length);
    QString ret=QString::fromWCharArray(output,length-1); // discard
    delete [] input;
    delete [] output;



    //QString aux="D:/www/MY RepLink APK Manager";

   /* if(ret.indexOf(" ")!=-1){
        QStringList partes=ret.split("/");
        ret="";
        for (int var = 0; var < partes.length(); ++var) {
            QString part= partes.at(var);
            QString test= part;

            if(part.indexOf(" ")!=-1){
                test=test.replace(" ","");
            }

            if(part.length()>6){
                test=test.toUpper();
            }

            if(var!=partes.length()-1){
                ret+= test.length()>6 ? test.mid(0,6)+"~1/" :test+"/"  ;
            }else{
                ret+= test.length()>6 ? test.mid(0,6)+"~1" :test  ;
            }
        }
    }*/

    return ret;
}

QString driveName(const QString & drive)
{
    wchar_t* input=new wchar_t[drive.size()+1];
    drive.toWCharArray(input);
    input[drive.size()]=L'\0'; // terminate string
    long length = GetVolumeNameForVolumeMountPoint(input, NULL, 0);
    wchar_t* output=new wchar_t[length];
    GetVolumeNameForVolumeMountPoint(input,output,length);
    QString ret=QString::fromWCharArray(output,length-1); // discard
    delete [] input;
    delete [] output;
    return ret;
}
#endif


static void checkFilesNeeded(QString path){
    QStringList neededFiles=  QStringList() << "aapt.exe" << "unzip.exe" << "GetSig.jar"<<"config.sqlitedb";

    QFileInfo fInfo(path+"/bins/"+neededFiles.at(0));
    QStringList msg;
    QString qt5=QFileInfo(path+"/"+neededFiles.at(3)).exists()? "": "config.sqlitedb not found." ;

    if(!qt5.isEmpty()){
        QMessageBox::critical(NULL, "Error", qt5);
    }

    msg<< QString(!fInfo.exists()?"Android Asset Packaging Tool not found! ":"");

    fInfo= QFileInfo(path+"/bins/"+neededFiles.at(1));

    msg << QString(!fInfo.exists()?"UnZip Tool not found! ":"");

    fInfo= QFileInfo(path+"/bins/"+neededFiles.at(2));

    msg << QString(!fInfo.exists()?"GetSig Tool not found! ":"");

    if((QString(msg.join(""))!="")){
        QMessageBox::critical(NULL, "Atention", msg.join("\n"));
    }

}

static void associateFileTypes(const QStringList &fileTypes)
{
    QString displayName = QGuiApplication::applicationDisplayName();
    QString filePath = QCoreApplication::applicationFilePath();

    QString fileName = QFileInfo(filePath).fileName();

    QSettings settings("HKEY_CURRENT_USER\\Software\\Classes\\Applications\\" + fileName, QSettings::NativeFormat);
    settings.setValue("FriendlyAppName", displayName);

    settings.beginGroup("SupportedTypes");
    foreach (const QString& fileType, fileTypes)
        settings.setValue(fileType, QString());
    settings.endGroup();

    settings.beginGroup("DefaultIcon");
    settings.setValue("Default",QChar('"') +  QDir::toNativeSeparators(filePath) +"\",1");
    settings.endGroup();

    settings.beginGroup("shell");
    settings.beginGroup("open");
    settings.setValue("FriendlyAppName", displayName);
    settings.beginGroup("Command");

    QStringList partes= filePath.split("/");
    partes.removeLast();
    QString aux=partes.join("/");


    QSettings conf(QSettings::IniFormat, QSettings::UserScope,
                           "RepLink APK Manager", "RepLinkAPK");

   // qDebug() << conf.allKeys();

    aux= shortPathName(aux);

    if(conf.allKeys().length()!=10){
        QStringList env = QStringList() <<"C:\\Program Files\\Java\\jdk1.8.0\\bin"<<"C:\\Archivos de programa\\Java\\jdk1.8.0\\bin"<<"C:\\Program Files (x86)\\Java\\jdk1.8.0\\bin";

        int pos=-1;
        for (int i = 0; i < env.length(); ++i) {
            QFileInfo java= QFileInfo(env.at(i)+"\\java.exe");
            if(java.exists()){
                pos=i;
                break;
            }
        }

        conf.beginGroup("Options");
        conf.setValue("rename", "1");
        conf.setValue("update", "1");
        conf.setValue("preview", "1");
        conf.setValue("apkbackup", "0");
        conf.endGroup();

        conf.beginGroup("KeyStoreConf");
        conf.setValue("keystore", "");
        conf.setValue("storepass", "");
        conf.setValue("key", "");
        conf.setValue("keypass", "");
        conf.endGroup();

        QString javaPath=(pos!=-1)? QString(env.at(pos)).replace("\\","/"):"";

        conf.beginGroup("AppDirs");
        conf.setValue("path", aux.replace("\\","/"));
        conf.setValue("java", javaPath);
        conf.setValue("data", "");
        conf.endGroup();
    }else{
        conf.beginGroup("AppDirs");
        conf.setValue("path", aux.replace("\\","/"));
        conf.endGroup();
    }

    settings.setValue(".", QChar('"') + QDir::toNativeSeparators(filePath) + QString("\" \"%1\""));


    QSettings *settingsIcon= new QSettings("HKEY_CLASSES_ROOT\\.apk\\", QSettings::NativeFormat);
    settingsIcon->setValue("Default", "RepLinkAPK.1");
    settingsIcon->beginGroup("DefaultIcon");
    settingsIcon->setValue("Default", QChar('"') + QDir::toNativeSeparators(filePath) +"\",1");
    settingsIcon->endGroup();

    settingsIcon= new QSettings("HKEY_CLASSES_ROOT\\", QSettings::NativeFormat);
    settingsIcon->beginGroup("RepLinkAPK.1");
    settingsIcon->setValue("Default", "RepLinkAPK.1");
    settingsIcon->beginGroup("DefaultIcon");
    settingsIcon->setValue("Default",QChar('"') +  QDir::toNativeSeparators(filePath) +"\",1");
    settingsIcon->endGroup();
    settingsIcon->endGroup();

    settingsIcon = new QSettings("HKEY_LOCAL_MACHINE\\Software\\Classes\\.apk\\", QSettings::NativeFormat);
    settingsIcon->setValue("Default", "RepLinkAPK.1");
    settingsIcon->beginGroup("DefaultIcon");
    settingsIcon->setValue("Default", QChar('"') + QDir::toNativeSeparators(filePath) +"\",1");
    settingsIcon->endGroup();

    settingsIcon= new QSettings("HKEY_LOCAL_MACHINE\\Software\\Classes\\", QSettings::NativeFormat);
    settingsIcon->beginGroup("RepLinkAPK.1");
    settingsIcon->setValue("Default", "RepLinkAPK.1");
    settingsIcon->beginGroup("DefaultIcon");
    settingsIcon->setValue("Default",QChar('"') +  QDir::toNativeSeparators(filePath) +"\",1");
    settingsIcon->endGroup();
    settingsIcon->endGroup();

    settingsIcon = new QSettings("HKEY_CURRENT_USER\\Software\\Classes\\.apk\\", QSettings::NativeFormat);
    settingsIcon->setValue("Default", "RepLinkAPK.1");
    settingsIcon->beginGroup("DefaultIcon");
    settingsIcon->setValue("Default", QChar('"') + QDir::toNativeSeparators(filePath) +"\",1");
    settingsIcon->endGroup();

    settingsIcon= new QSettings("HKEY_CURRENT_USER\\Software\\Classes\\", QSettings::NativeFormat);
    settingsIcon->beginGroup("RepLinkAPK.1");
    settingsIcon->setValue("Default", "RepLinkAPK.1");
    settingsIcon->beginGroup("DefaultIcon");
    settingsIcon->setValue("Default",QChar('"') +  QDir::toNativeSeparators(filePath) +"\",1");
    settingsIcon->endGroup();
    settingsIcon->endGroup();

    checkFilesNeeded(aux);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("RepLinkAPK");
    app.setOrganizationName("This Company");
    app.setOrganizationDomain("replink.cu");
    app.setApplicationDisplayName("RepLink APK Manager");

    associateFileTypes(QStringList(".apk"));

    QStringList arguments = QCoreApplication::arguments();

    if (arguments.size() == 2){
        ApkInfoView *apkiv;
        if(arguments.at(1).split('.').last().toLower() == "apk"){

                QFileInfo finfo= QFileInfo(arguments.at(1));
                if(finfo.exists()){
                    QString url=arguments.at(1);
                    apkiv= new ApkInfoView(url);
                    apkiv->show();
                    return app.exec();
                }

        }else{
            QMessageBox::warning(NULL, "Notice", "Bad arguments. Please take in count only '.apk' files.");
            std::cout << "Notice: Bad arguments. Please take in count only '.apk' files." << std::endl;
            return 1;
        }
    }else if(arguments.size()==1){
        MainWindow w;
        w.show();
        return app.exec();
    }else{
       std::cout << "Notice: To many arguments" << std::endl;
       return 1;
    }
    return app.exec();
}
