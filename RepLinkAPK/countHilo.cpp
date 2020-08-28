#include <countHilo.h>


CCountHilo::CCountHilo(QObject *parent): QThread(parent){
    Q_UNUSED(parent);

}

void CCountHilo::SetParams(QString repo){
    this->repo=repo;
    this->xmlFilePath=repo+"/replink.xml";
}

void CCountHilo::run()
{
    QDomDocument domDocument;
    QStringList result;

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

                ListaSE<QString> categories;
                QList<int> contadores;

                QDomNodeList listaApps = fdroid.elementsByTagName("application");
                QString auxCat="";
                int index;

                for (int i = 0; i < listaApps.length(); ++i) {

                    auxCat= domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("category").at(0).firstChild().nodeValue();
                    index=categories.Buscar(auxCat);

                    if(index==-1){
                         categories.AdicionarCola(auxCat);
                         contadores.append(1);
                    }else{
                         contadores.replace(index-1, contadores.at(index-1)+1);
                    }
                }



                for (int j = 1; j <= categories.Longitud(); ++j) {
                       result.append(categories.ObtenerPorPos(j)+": "+QString::number(contadores.at(j-1)));
                }

                emit AppPerCat(result);
            }
        }
    }
}


void CCountHilo::Cerrar()
{
    this->terminate();
}

