#include <reportHilo.h>


CReportHilo::CReportHilo(QObject *parent): QThread(parent){
    Q_UNUSED(parent);

}

void CReportHilo::SetParams(QString repo, QStringList existFiles){
    this->repo=repo;
    this->xmlFilePath=repo+"/replink.xml";
    this->existFiles=existFiles;

}

void CReportHilo::run()
{
    QDomDocument domDocument;

    QStringList usedApks;
    QStringList unused;
    QStringList mised;
    QString unidas="";

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

                QDomNodeList apks=fdroid.elementsByTagName("apkname");
                for (int k = 0; k < apks.length(); ++k) {
                    usedApks << apks.at(k).firstChild().nodeValue();
                }

                for (int i = 0; i < existFiles.length(); ++i) {
                    if(usedApks.indexOf(existFiles.at(i))==-1){
                        unused << existFiles.at(i);
                    }
                }

                for (int i = 0; i < usedApks.length(); ++i) {
                    if(existFiles.indexOf(usedApks.at(i))==-1){
                        mised << usedApks.at(i);
                    }
                }

                ListaSE<QStringList> exacts=ExactSizeApps();

                for (int i = 1; i <= exacts.Longitud(); ++i) {
                    unidas+="<p>";
                    for (int j = 0; j < exacts.ObtenerPorPos(i).length(); ++j){
                        if(unused.indexOf(exacts.ObtenerPorPos(i).at(j))==-1){
                            unidas+= "<b>(Used) "+exacts.ObtenerPorPos(i).at(j)+"</b>";
                        }else{
                            unidas+= exacts.ObtenerPorPos(i).at(j)+"";
                        }
                        if(j<exacts.ObtenerPorPos(i).length()-1){
                            unidas+="<br/>";
                        }

                    }
                    unidas+="</p>";
                }

                emit Report(unidas,unused,mised);

            }
            //this->msleep(15+ (i==0? 150:0));

            //emit Porciento(100);
        }
    }
}

ListaSE<QStringList> CReportHilo::ExactSizeApps()
{
    ListaSE<QStringList> exacts;
    QFileInfo infoFileX;
    QFileInfo infoFileY;

    QStringList auxExistFiles=existFiles;

    QStringList temp;

    QStringList filters;
    filters << "*.apk";

    QFileInfoList apks= QDir(repo).entryInfoList(filters);

    for (int i = 0; i < apks.length()-1; ++i) {
        for (int j = i+1; j < apks.length(); ++j) {
            if(i!=j){
                if(apks.at(i).size()==apks.at(j).size()){
                     temp << apks.at(i).completeBaseName()+".apk";
                     temp << apks.at(j).completeBaseName()+".apk";
                }
            }
        }
        if(temp.length()>1){
            exacts.AdicionarCola(temp);
            temp.clear();
        }
    }
    return exacts;
}


void CReportHilo::Cerrar()
{
    this->terminate();
}

