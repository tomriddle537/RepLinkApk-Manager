#include <listHilo.h>


CListHilo::CListHilo(QObject *parent): QThread(parent){
    Q_UNUSED(parent);

}

void CListHilo::SetParams(QString repo){
    this->repo=repo;
    this->xmlFilePath=repo+"/replink.xml";
}

void CListHilo::run()
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
                QString id;
                QString name;
                QString summary;
                QString desc;
                QString license;
                QString category;
                QString categories;
                QString source;
                QString version;
                QString requirements;
                ListaSE<PkgInfo> packages;

                QDomNodeList listaApps = fdroid.elementsByTagName("application");

                for (int i = 0; i < listaApps.length(); ++i) {
                    packages=ListaSE<PkgInfo>();

                    id= listaApps.at(i).toElement().elementsByTagName("id").at(0).firstChild().nodeValue();
                    name= listaApps.at(i).toElement().elementsByTagName("name").at(0).firstChild().nodeValue();
                    summary= listaApps.at(i).toElement().elementsByTagName("summary").at(0).firstChild().nodeValue();

                    desc=listaApps.at(i).toElement().elementsByTagName("desc").at(0).firstChild().nodeValue();

                    license= listaApps.at(i).toElement().elementsByTagName("license").at(0).firstChild().nodeValue();
                    category= listaApps.at(i).toElement().elementsByTagName("category").at(0).firstChild().nodeValue();
                    categories= listaApps.at(i).toElement().elementsByTagName("categories").at(0).firstChild().nodeValue();
                    source= listaApps.at(i).toElement().elementsByTagName("source").at(0).firstChild().nodeValue();
                    version= listaApps.at(i).toElement().elementsByTagName("package").at(0).toElement().elementsByTagName("version").at(0).firstChild().nodeValue();
                    requirements= listaApps.at(i).toElement().elementsByTagName("requirements").at(0).firstChild().nodeValue();

                    QDomNodeList pkgs=listaApps.at(i).toElement().elementsByTagName("package");

                    PkgInfo aux;
                    QString v;
                    QString vc;
                    QString apkname;
                    QString sdkver;
                    QString size;
                    QString mainObb;
                    QString patchObb;

                    for (int j = 0; j < pkgs.count(); ++j) {
                        v=pkgs.at(j).toElement().elementsByTagName("version").at(0).firstChild().nodeValue();
                        vc=pkgs.at(j).toElement().elementsByTagName("versioncode").at(0).firstChild().nodeValue();
                        apkname=pkgs.at(j).toElement().elementsByTagName("apkname").at(0).firstChild().nodeValue();
                        sdkver=pkgs.at(j).toElement().elementsByTagName("sdkver").at(0).firstChild().nodeValue();
                        size=pkgs.at(j).toElement().elementsByTagName("size").at(0).firstChild().nodeValue();
                        mainObb=pkgs.at(j).toElement().elementsByTagName("obbMainFile").at(0).firstChild().nodeValue();
                        patchObb=pkgs.at(j).toElement().elementsByTagName("obbPatchFile").at(0).firstChild().nodeValue();
                        aux= PkgInfo(id,v,vc,apkname,sdkver,size,mainObb,patchObb);
                        packages.AdicionarCola(aux);
                    }


                    emit Porciento(i*100/listaApps.length());
                    emit SendInfo(AppInfo(id,name,summary,desc,category, categories,license,source,version,packages,requirements));

                    this->msleep(15+ (i==0? 150:0));

                }
                emit Porciento(100);
            }
        }
    }
}


void CListHilo::Cerrar()
{
    this->terminate();
}

