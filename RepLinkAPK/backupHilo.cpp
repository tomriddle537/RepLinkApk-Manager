#include <backupHilo.h>


CBackupHilo::CBackupHilo(QObject *parent): QThread(parent){
    Q_UNUSED(parent);

}

void CBackupHilo::SetParams(QString repo, QString backupDir){
    this->repo=repo;
    this->xmlFilePath=repo+"/replink.xml";
    this->backupDir=backupDir;
}

void CBackupHilo::run()
{
    QDomDocument domDocument;

    QString error="";
    QFile* file = new QFile(xmlFilePath);

    if (!file->open(QFile::ReadOnly | QFile::Text)) {
        error=QString("Can\'t read file %1:\n%2.").arg(xmlFilePath).arg(file->errorString());
        std::cout<< error.toStdString()<< std::endl;

        delete file;
    }else{
        QString errorStr;
        int errorLine;
        int errorColumn;

        if (!domDocument.setContent(file, true, &errorStr, &errorLine, &errorColumn)) {
            error=QString("Error de Parseo en la linea %1, columna %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
            std::cout<< error.toStdString()<< std::endl;


            delete file;
        }else{
            file->close();
            delete file;
            QDomElement fdroid = domDocument.documentElement();

            if (fdroid.tagName() != "fdroid") {
                error=QString("El archivo no posee la estructura RepLink.");
                std::cout<< error.toStdString()<< std::endl;
            }else {

                if(QDir(backupDir).exists()){
                    QString category;
                    QString categories;
                    //ListaSE<PkgInfo> packages;

                    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                                       "RepLink APK Manager", "RepLinkAPK");

                    if(settings.value("Options/apkbackup").toString()=="1"){
                        QDomNodeList listaApps = fdroid.elementsByTagName("application");

                        for (int i = 0; i < listaApps.length(); ++i) {
                            //packages=ListaSE<PkgInfo>();
                            category= listaApps.at(i).toElement().elementsByTagName("category").at(0).firstChild().nodeValue();
                            categories= listaApps.at(i).toElement().elementsByTagName("categories").at(0).firstChild().nodeValue();

                            QStringList partes= categories.split(",");
                            for (int k = 0; k < partes.length(); ++k) {
                                if(partes.at(k).indexOf("Colecc")!=-1 || partes.at(k).indexOf("Google")!=-1 || partes.at(k).indexOf("Cuban")!=-1){
                                    category=partes.at(k);
                                }
                            }

                            QDir backRuta(backupDir+"/"+category);
                            if(!backRuta.exists()){
                                backRuta.mkdir(backupDir+"/"+category);
                            }
                            QDomNodeList pkgs=listaApps.at(i).toElement().elementsByTagName("package");
                            //PkgInfo aux;
                            QString apkname;
                            for (int j = 0; j < pkgs.count(); ++j) {
                                apkname=pkgs.at(j).toElement().elementsByTagName("apkname").at(0).firstChild().nodeValue();
                                // aux= PkgInfo(id,v,vc,apkname,sdkver,size);
                                //packages.AdicionarCola(aux);

                                QFile backFile(repo+"/"+apkname);

                                if(backFile.exists()){
                                    backFile.copy(backupDir+"/"+category+"/"+apkname);
                                }
                                emit Porciento(i*100/listaApps.length(),category,apkname);
                                this->msleep(5);
                            }
                        }
                    }

                    QString fecha = QDate().currentDate().toString("dd-MM-yyyy");
                    emit Porciento(99, "BackUp...","Repo Files on "+fecha);

                    QDir backRuta(backupDir+"/[RepoFiles]");
                    if(!backRuta.exists()){
                        backRuta.mkdir(backupDir+"/[RepoFiles]");
                    }

                    QDir dirRuta(backupDir+"/[RepoFiles]/"+fecha);
                    if(!dirRuta.exists()){
                        dirRuta.mkdir(backupDir+"/[RepoFiles]/"+fecha);
                    }

                    QFile replink(backupDir+"/[RepoFiles]/"+fecha+"/replink.xml");
                    QFile cat(backupDir+"/[RepoFiles]/"+fecha+"/categories.txt");
                    QFile jar(backupDir+"/[RepoFiles]/"+fecha+"/index.jar");
                    QFile php(backupDir+"/[RepoFiles]/"+fecha+"/index.php");

                    if(replink.exists()){
                        replink.remove();
                    }
                    if(cat.exists()){
                        cat.remove();
                    }
                    if(jar.exists()){
                        jar.remove();
                    }
                    if(php.exists()){
                        php.remove();
                    }

                    replink.copy(repo+"/replink.xml",backupDir+"/[RepoFiles]/"+fecha+"/replink.xml");
                    replink.copy(repo+"/categories.txt",backupDir+"/[RepoFiles]/"+fecha+"/categories.txt");
                    replink.copy(repo+"/index.jar",backupDir+"/[RepoFiles]/"+fecha+"/index.jar");
                    replink.copy(repo+"/index.php",backupDir+"/[RepoFiles]/"+fecha+"/index.php");


                    emit Porciento(100, "BackUp...","Finished");
                }
            }
        }
    }
}


void CBackupHilo::Cerrar()
{
    this->terminate();
}

