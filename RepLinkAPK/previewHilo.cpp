#include <previewHilo.h>


CPreviewHilo::CPreviewHilo(QObject *parent): QThread(parent){
    Q_UNUSED(parent);
    bop=new CBinOutputParser();
}

void CPreviewHilo::SetParams(QString repo, ListaSE<NewAPKFile> files, bool fromApk){
    this->repo=repo;
    this->files=files;
    this->fromApk=fromApk;
}

void CPreviewHilo::run()
{

    if(files.Longitud()>0){
        if(fromApk){
            QProcess cmd;
            QString appDir="\"";

            QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                               "RepLink APK Manager", "RepLinkAPK");

            appDir+=settings.value("AppDirs/path").toString()+"/";
            QString command="";
            QString url="";

            for (int i = 1; i <= files.Longitud(); ++i) {
                if(files.ObtenerPorPos(i).ObtenerCategory()=="Uncategorized"){
                    url=repo+"/"+files.ObtenerPorPos(i).ObtenerAPKName();
                    command=appDir+"bins/aapt.exe\" dump badging \""+url+"\"";
                    cmd.start("cmd.exe /c " + command.replace("/","\\") );
                    cmd.waitForFinished(-1);
                    QByteArray respose=cmd.readAllStandardOutput();
                    QString result=QString().fromStdString(respose.toStdString());

                    QStringList lines=result.split("\r\n");

                    ApkPreview apkPrev= bop->binOutputParser(lines,url);

                    emit Preview(apkPrev);
                    this->msleep(15+ (i==0? 100:0));
                }

                //return apkPrev;
            }

            //emit Porciento(100);
        }else{

            QPixmap pixmap;

            if(QFileInfo(repo+"/icons/"+files.ObtenerPorPos(1).ObtenerCategory()+".png").exists()){
                pixmap.load(repo+"/icons/"+files.ObtenerPorPos(1).ObtenerCategory()+".png");
            }else if(QFileInfo(repo+"/icons/"+files.ObtenerPorPos(1).ObtenerCategory()+".jpg").exists()){
                pixmap.load(repo+"/icons/"+files.ObtenerPorPos(1).ObtenerCategory()+".jpg");
            }else{
                pixmap.load(":/info_small.png");
            }

            ApkPreview apkPrev= ApkPreview("",files.ObtenerPorPos(1).ObtenerCategory(),files.ObtenerPorPos(1).ObtenerAPKName(),pixmap,0,"","");

            emit Preview(apkPrev);
        }
    }

}


void CPreviewHilo::Cerrar()
{
    this->terminate();
}

