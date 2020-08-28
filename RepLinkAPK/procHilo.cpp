#include <procHilo.h>


CProcHilo::CProcHilo(QObject *parent): QThread(parent){
    Q_UNUSED(parent);

}

void CProcHilo::SetParametros( CXMLMethods *xmlM, QString repository, ListaSE<NewAPKFile> files, QDomDocument &domDocument, bool remoteFiles, bool bDefault,  QString catDefault){
    this->repository=repository;
    this->files=files;
    this->remoteFiles=remoteFiles;
    this->bDefault=bDefault;
    this->domDocument=domDocument;
    this->catDefault=catDefault;
    this->xmlM=xmlM;
}

void CProcHilo::run()
{

    bop = new CBinOutputParser();
    connect(bop,SIGNAL(ImgActual(QPixmap)),this, SLOT(slot_Img(QPixmap)));
    connect(bop,SIGNAL(Notice(QString,int)),this, SLOT(slot_Notice(QString,int)));

    CXMLAplication appaux;

    QString url;

    int porciento=0;

    QFileInfo infoFile;
    for (int f = 1; f <= files.Longitud(); ++f) {
        url=repository+"/"+files.ObtenerPorPos(f).ObtenerAPKName();
        infoFile=QFileInfo(url);

        if(infoFile.exists()){
            QProcess cmd;
            QString appDir="\"";

            QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                               "RepLink APK Manager", "RepLinkAPK");

            appDir+=settings.value("AppDirs/path").toString()+"/";

            QString command=appDir+"bins/aapt.exe\" dump badging \""+url+"\"";
            cmd.start("cmd.exe /c " + command.replace("/","\\") );
            cmd.waitForFinished(-1);
            QByteArray respose=cmd.readAllStandardOutput();
            QString result=QString().fromStdString(respose.toStdString());



            QStringList lines=result.split("\r\n");

            appaux=bop->binOutputParser(repository, lines,url, infoFile);


            if(bDefault){
                appaux.SetCategorYies(catDefault);
            }else{
                if(files.ObtenerPorPos(f).ObtenerCategory()=="Uncategorized"){
                    appaux.SetCategorYies("No Category");
                }else{
                    appaux.SetCategorYies(files.ObtenerPorPos(f).ObtenerCategory());
                }
            }
            if(appaux.ObtenerSummary()=="Unknown"){
                appaux.SetSummary(appaux.ObtenerCategorYies());
            }

            if(!appaux.ObtenerId().isEmpty()){
                xmlM->XMLParse(domDocument, appaux, infoFile, remoteFiles);
            }
            porciento=f*100/files.Longitud();
            emit Porciento(porciento);
        }
    }


}

void CProcHilo::slot_Img(QPixmap img)
{
    emit Imagen(img);
}

void CProcHilo::slot_Notice(QString msg, int type)
{
    emit Mensaje(msg,type);
}

void CProcHilo::Cerrar()
{
    delete bop;
    this->terminate();
}

