#include <tohtml.h>


CToHtlmHilo::CToHtlmHilo(QObject *parent): QThread(parent){
    Q_UNUSED(parent);

}

void CToHtlmHilo::SetParams(QString repo, bool latest , bool plain, QString outputDir){
    this->repo=repo;
    this->xmlFilePath=repo+"/replink.xml";
    this->outputDir=outputDir;
    this->latest=latest;
    this->plain=plain;
}

void CToHtlmHilo::run()
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

                if(QDir(outputDir).exists()){

                    QDomNodeList repo=fdroid.elementsByTagName("repo");
                    QString url=repo.at(0).attributes().namedItem("url").nodeValue();
                    QString repoName=repo.at(0).attributes().namedItem("name").nodeValue();

                    QString htmlStart="<html><head><title>"+ repoName+ " ("+QString(latest?"Latest":"All")+")"+"</title></head><body><div class='repo' style='text-align:center;'><h2>"+ repoName + " ("+QString(latest?"Latest":"All")+")"+"</h2>";
                    QString htmlEnd="</div></body></html>";
                    QString plainText;

                    QString name;
                    //ListaSE<PkgInfo> packages;
                    QDomNodeList listaApps = fdroid.elementsByTagName("application");
                    for (int i = 0; i < listaApps.length(); ++i) {
                        //packages=ListaSE<PkgInfo>();
                        name= listaApps.at(i).toElement().elementsByTagName("name").at(0).firstChild().nodeValue();

                        if(!plain){
                            htmlStart+="<div class='app' style='margin:0px;padding:0px;'><h4>"+name+"</h4><ol style='margin:0px;padding:0px;'>";
                        }

                        QDomNodeList pkgs=listaApps.at(i).toElement().elementsByTagName("package");
                        //PkgInfo aux;
                        QString apkname;
                        QString href;

                        int cant=latest? 1 : pkgs.count();

                        for (int j = 0; j < cant; ++j) {
                            apkname=pkgs.at(j).toElement().elementsByTagName("apkname").at(0).firstChild().nodeValue();
                            href=url+"/"+apkname;
                            if(plain){
                                plainText+=href+"\n";
                            }else{
                                htmlStart+="<li ><a href='"+href+"'>"+apkname+"</a></li>";
                            }

                            emit Porciento(i*100/listaApps.length(),name,apkname);
                            this->msleep(5);
                        }
                        if(!plain){
                            htmlStart+="</ol></div>";
                        }

                    }

                    if(!plain){
                        htmlStart+=htmlEnd;
                    }

                    QFile *file;
                    if(!plain){
                        file = new QFile(outputDir+"/"+repoName+ " ("+QString(latest?"Latest":"All")+")"+".html");
                    }else{
                        file = new QFile(outputDir+"/"+repoName+ " ("+QString(latest?"Latest":"All")+")"+".txt");
                    }
                    if (!file->open(QFile::WriteOnly | QFile::Text)) {
                        error=QString("Can\'t read file %1:\n%2.").arg(outputDir+"/"+repoName+".html").arg(file->errorString());
                        delete file;
                        std::cout <<  error.toStdString() << std::endl;
                    }else {
                        //const int IndentSize = 4;
                        QTextStream out(file);
                        //domDocument.save(out, IndentSize);
                        if(!plain){
                            out << htmlStart;
                        }else{
                            out << plainText;
                        }
                        file->close();
                        delete file;
                    }

                    emit Porciento(100, plain?"To Plain...":"To HTML...","Finished");
                }
            }
        }
    }
}


void CToHtlmHilo::Cerrar()
{
    this->terminate();
}

