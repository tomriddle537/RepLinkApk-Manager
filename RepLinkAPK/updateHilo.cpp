#include <updateHilo.h>


CUpdateHilo::CUpdateHilo(QObject *parent): QThread(parent){
    Q_UNUSED(parent);

}

void CUpdateHilo::SetParams(QString repo){
    this->repo=repo;
    this->xmlFilePath=repo+"/replink.xml";
}

void CUpdateHilo::run()
{
    QDomDocument domDocument;

    QString salida1="Error";
    QString salida2="Error";
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

                salida1=QString::number(fdroid.elementsByTagName("package").length());

                salida2=QString::number(fdroid.elementsByTagName("application").length());

                QString desc="Error";
                CXMLRepo xrepo;
                QDomNodeList repo=fdroid.elementsByTagName("repo");
                desc=repo.at(0).toElement().elementsByTagName("description").at(0).firstChild().nodeValue();
                QString url=repo.at(0).attributes().namedItem("url").nodeValue();
                QString name=repo.at(0).attributes().namedItem("name").nodeValue();
                QString pubkey=repo.at(0).attributes().namedItem("pubkey").nodeValue();
                QString version=repo.at(0).attributes().namedItem("version").nodeValue();

                QString icon=repo.at(0).attributes().namedItem("icon").nodeValue();

                xrepo=CXMLRepo(name,url,desc,icon,pubkey,version);
                xrepo.SetTimeStamp(repo.at(0).attributes().namedItem("timestamp").nodeValue());


                emit UpdateRepoInfo(salida2,salida1,xrepo);

                //this->msleep(15+ (i==0? 150:0));

                //emit Porciento(100);
            }
        }
    }
}


void CUpdateHilo::Cerrar()
{
    this->terminate();
}

