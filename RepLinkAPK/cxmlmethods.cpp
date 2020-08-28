#include "cxmlmethods.h"

CXMLMethods::CXMLMethods(QObject *parent): QObject(parent)
{
    Q_UNUSED(parent);
}

CXMLMethods::~CXMLMethods()
{

}


void CXMLMethods::XMLParse(QDomDocument &domDocument, CXMLAplication &app, QFileInfo infoFile, bool remoteList)
{

    //std::cout<<("Archivo cargado")<<std::endl;

    QDomElement fdroid = domDocument.documentElement();
    QString error="";

    if (fdroid.tagName() != "fdroid") {
        error=QString("El archivo no posee la estructura RepLink.");
        emit Notice(error,1);
    }else {
        QDomNodeList repo=fdroid.elementsByTagName("repo");
        double fileTS=infoFile.created().toString("yyyyMMddhhmmss").toDouble();
        double repoTS=repo.at(0).attributes().namedItem("timestamp").nodeValue().toDouble();
        int pos=0;

        if(fileTS>repoTS || remoteList){
            QDomNodeList listApp=domDocument.elementsByTagName("application");
            bool flag=false;
            for (int i = 0; i < listApp.length(); ++i) {
                QString idpkg = listApp.at(i).attributes().namedItem("id").nodeValue();
                if(idpkg==app.ObtenerId()){
                    flag=true;
                    pos=i;
                    break;
                }
            }

            if(!flag){
                domDocument.elementsByTagName("fdroid").at(0).appendChild(app.toXML());
            }else{
                QDomElement appElement=listApp.at(pos).toElement();
                QDomNodeList listPkg = appElement.elementsByTagName("package");
                int versionCode=0;
                int cant=listPkg.length();
                bool flag=true;
                for (int j = 0; j < cant; ++j) {
                    versionCode=listPkg.at(j).childNodes().at(1).firstChild().nodeValue().toInt();
                    if(app.ObtenerPackages().ObtenerPorPos(1).ObtenerVersionCode().toInt()> versionCode){
                        domDocument.elementsByTagName("application").at(pos).insertBefore(app.ObtenerPackages().ObtenerPorPos(1).toXML(),listPkg.at(j));
                        flag=false;
                        break;
                    } else if(app.ObtenerPackages().ObtenerPorPos(1).ObtenerVersionCode().toInt()== versionCode){
                        flag=false;
                        break;
                    }
                }
                if(flag && listPkg.length()>0){
                    domDocument.elementsByTagName("application").at(pos).insertAfter(app.ObtenerPackages().ObtenerPorPos(1).toXML(),listPkg.at(cant-1));
                }else if(flag && listPkg.length()==0){
                    domDocument.elementsByTagName("application").at(pos).appendChild(app.ObtenerPackages().ObtenerPorPos(1).toXML());
                }

                QString lastupdated=domDocument.elementsByTagName("application").at(pos).toElement().elementsByTagName("package").at(0).toElement().elementsByTagName("added").at(0).firstChild().nodeValue();
                QString marketversion=domDocument.elementsByTagName("application").at(pos).toElement().elementsByTagName("package").at(0).toElement().elementsByTagName("version").at(0).firstChild().nodeValue();
                QString marketvercode=domDocument.elementsByTagName("application").at(pos).toElement().elementsByTagName("package").at(0).toElement().elementsByTagName("versioncode").at(0).firstChild().nodeValue();

                domDocument.elementsByTagName("application").at(pos).toElement().elementsByTagName("lastupdated").at(0).firstChild().setNodeValue(lastupdated);
                domDocument.elementsByTagName("application").at(pos).toElement().elementsByTagName("marketversion").at(0).firstChild().setNodeValue(marketversion);
                domDocument.elementsByTagName("application").at(pos).toElement().elementsByTagName("marketvercode").at(0).firstChild().setNodeValue(marketvercode);
            }
        }
        emit Notice("Document Parsed for "+ app.ObtenerName() +  " "+app.ObtenerMarketVersion(),1);
    }





}

QString CXMLMethods::XMLWrite(QString xmlFilePath, QDomDocument &domDocument, bool updateTS)
{

    QString error="ok";
    if(updateTS){
        error=UpdateTimeStamp(domDocument);
    }

    if(error=="ok"){
        QFile *file = new QFile(xmlFilePath);
        if (!file->open(QFile::WriteOnly | QFile::Text)) {
            error=QString("Can\'t read file %1:\n%2.").arg(xmlFilePath).arg(file->errorString());
            delete file;
            return error;
        }else {
            if(updateTS){emit Notice("Building the index file...",1);}

            //const int IndentSize = 4;
            QTextStream out(file);
            //domDocument.save(out, IndentSize);

            QString document=domDocument.toString();
            document.replace("&amp;lt;p&amp;gt;","&lt;p&gt;");
            document.replace("&amp;lt;/p&amp;gt;","&lt;/p&gt;");
            document.replace("&lt;p>","&lt;p&gt;");
            document.replace("&lt;/p>","&lt;/p&gt;");
            //domDocument.setContent(document);
            out << document;

            file->close();
            delete file;

            if(updateTS){emit Notice("Signing the repo...",1);}

            CreateSignedIndexJar(xmlFilePath);

            if(updateTS){emit Notice("Done.",0);}

            return "File succefully writed";
        }
    } else {
        return error;
    }
}

QString CXMLMethods::XMLWriteJSON(QString repo, QString jsonFilePath, QString json)
{
    QString error="ok";


    if(error=="ok"){
        QFile *file = new QFile(jsonFilePath);
        if (!file->open(QFile::WriteOnly | QFile::Text)) {
            error=QString("Can\'t read file %1:\n%2.").arg(jsonFilePath).arg(file->errorString());
            delete file;
            return error;
        }else {
            emit Notice("Building the json file...",1);

            QTextStream out(file);
            out << json;

            file->close();
            delete file;

            emit Notice("Signing the json repo...",1);

            CreateSignedIndexV1Jar(repo, jsonFilePath);

            emit Notice("Done.",0);

            return "JSON File succefully writed";
        }
    } else {
        return error;
    }
}

QString CXMLMethods::XMLCantPkg(QString xmlFilePath)
{
    QDomDocument domDocument;
    QString salida="Error";

    QString error="";
    QFile* file = new QFile(xmlFilePath);

    if (!file->open(QFile::ReadOnly | QFile::Text)) {
        error=QString("Can\'t read file %1:\n%2.").arg(xmlFilePath).arg(file->errorString());
        emit Notice(error,1);
        std::cout<< error.toStdString()<< std::endl;
        delete file;
        return error;
    }else{
        QString errorStr;
        int errorLine;
        int errorColumn;

        if (!domDocument.setContent(file, true, &errorStr, &errorLine, &errorColumn)) {
            error=QString("Error de Parseo en la linea %1, columna %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
            std::cout<< error.toStdString()<< std::endl;
            emit Notice(error,1);
            delete file;
            return error;
        }else{
            file->close();
            delete file;
            QDomElement fdroid = domDocument.documentElement();

            if (fdroid.tagName() != "fdroid") {
                error=QString("El archivo no posee la estructura RepLink.");
                emit Notice(error,1);
                return error;
            }else {
                salida=QString::number(fdroid.elementsByTagName("package").length());
                return salida;
            }
        }
    }
}

QString CXMLMethods::XMLCantApps(QString xmlFilePath)
{
    QDomDocument domDocument;
    QString salida="Error";

    QString error="";
    QFile* file = new QFile(xmlFilePath);

    if (!file->open(QFile::ReadOnly | QFile::Text)) {
        error=QString("Can\'t read file %1:\n%2.").arg(xmlFilePath).arg(file->errorString());
        std::cout<< error.toStdString()<< std::endl;
        emit Notice(error,1);
        delete file;
        return error;
    }else{
        QString errorStr;
        int errorLine;
        int errorColumn;

        if (!domDocument.setContent(file, true, &errorStr, &errorLine, &errorColumn)) {
            error=QString("Error de Parseo en la linea %1, columna %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
            std::cout<< error.toStdString()<< std::endl;
            emit Notice(error,1);
            delete file;
            return error;
        }else{
            file->close();
            delete file;
            QDomElement fdroid = domDocument.documentElement();

            if (fdroid.tagName() != "fdroid") {
                error=QString("El archivo no posee la estructura RepLink.");
                emit Notice(error,1);
                return error;
            }else {
                salida=QString::number(fdroid.elementsByTagName("application").length());
                return salida;
            }
        }
    }
}



QString CXMLMethods::XMLTimeStamp(QString xmlFilePath)
{
    QDomDocument domDocument;
    QString salida="Error";

    QString error="";
    QFile* file = new QFile(xmlFilePath);

    if (!file->open(QFile::ReadOnly | QFile::Text)) {
        error=QString("Can\'t read file %1:\n%2.").arg(xmlFilePath).arg(file->errorString());
        std::cout<< error.toStdString()<< std::endl;
        delete file;
        return error;
    }else{
        QString errorStr;
        int errorLine;
        int errorColumn;

        if (!domDocument.setContent(file, true, &errorStr, &errorLine, &errorColumn)) {
            error=QString("Error de Parseo en la linea %1, columna %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
            std::cout<< error.toStdString()<< std::endl;
            delete file;
            return error;
        }else{
            file->close();
            delete file;
            QDomElement fdroid = domDocument.documentElement();

            if (fdroid.tagName() != "fdroid") {
                error=QString("El archivo no posee la estructura RepLink.");
                return error;
            }else {
                QDomNodeList repo=fdroid.elementsByTagName("repo");
                salida=repo.at(0).attributes().namedItem("timestamp").nodeValue();
                return salida;
            }
        }
    }
}

QString CXMLMethods::XMLRepoIcon(QString xmlFilePath)
{
    QDomDocument domDocument;
    QString salida="Error";

    QString error="";
    QFile* file = new QFile(xmlFilePath);

    if (!file->open(QFile::ReadOnly | QFile::Text)) {
        error=QString("Can\'t read file %1:\n%2.").arg(xmlFilePath).arg(file->errorString());
        std::cout<< error.toStdString()<< std::endl;
        delete file;
        return error;
    }else{
        QString errorStr;
        int errorLine;
        int errorColumn;

        if (!domDocument.setContent(file, true, &errorStr, &errorLine, &errorColumn)) {
            error=QString("Error de Parseo en la linea %1, columna %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
            std::cout<< error.toStdString()<< std::endl;
            delete file;
            return error;
        }else{
            file->close();
            delete file;
            QDomElement fdroid = domDocument.documentElement();

            if (fdroid.tagName() != "fdroid") {
                error=QString("El archivo no posee la estructura RepLink.");
                return error;
            }else {
                QDomNodeList repo=fdroid.elementsByTagName("repo");
                salida=repo.at(0).attributes().namedItem("icon").nodeValue();
                return salida;
            }
        }
    }
}

CXMLRepo CXMLMethods::XMLRepoInfo(QString xmlFilePath)
{
    QDomDocument domDocument;
    QString desc="Error";

    QString error="";
    QFile* file = new QFile(xmlFilePath);

    if (!file->open(QFile::ReadOnly | QFile::Text)) {
        error=QString("Can\'t read file %1:\n%2.").arg(xmlFilePath).arg(file->errorString());
        std::cout<< error.toStdString()<< std::endl;
        delete file;
        return CXMLRepo();
    }else{
        QString errorStr;
        int errorLine;
        int errorColumn;

        if (!domDocument.setContent(file, true, &errorStr, &errorLine, &errorColumn)) {
            error=QString("Error de Parseo en la linea %1, columna %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
            std::cout<< error.toStdString()<< std::endl;
            delete file;
            return CXMLRepo();
        }else{
            file->close();
            delete file;
            QDomElement fdroid = domDocument.documentElement();

            if (fdroid.tagName() != "fdroid") {
                error=QString("El archivo no posee la estructura RepLink.");
                return CXMLRepo();
            }else {
                CXMLRepo xrepo;
                QDomNodeList repo=fdroid.elementsByTagName("repo");
                desc=repo.at(0).toElement().elementsByTagName("description").at(0).firstChild().nodeValue();
                QString url=repo.at(0).attributes().namedItem("url").nodeValue();
                QString name=repo.at(0).attributes().namedItem("name").nodeValue();
                QString pubkey=repo.at(0).attributes().namedItem("pubkey").nodeValue();
                QString version=repo.at(0).attributes().namedItem("version").nodeValue();
                QString icon=repo.at(0).attributes().namedItem("icon").nodeValue();

                xrepo=CXMLRepo(name,url,desc,icon,pubkey,version);

                return xrepo;
            }
        }
    }
}

void CXMLMethods::CreateNewRepo(CXMLRepo rinfo, QString path)
{
    QProcess cmd;
    QString command;

    QString c="cmd.exe /c ";

    QDir d(path);
    d.mkdir("icons");
    d.mkdir("icons-120");
    d.mkdir("icons-160");
    d.mkdir("icons-240");
    d.mkdir("icons-320");
    d.mkdir("icons-480");
    d.mkdir("icons-640");
    d.mkdir("logs");
    d.mkdir("repo");

    command="echo Empty > "+path+"/replink.xml";
    cmd.start(c+command.replace("/","\\"));
    cmd.waitForFinished(-1);
    command="echo Unknown > "+path+"/categories.txt";
    cmd.start(c+command.replace("/","\\"));
    cmd.waitForFinished(-1);

    QDomDocument domDocument;
    QDomProcessingInstruction header=domDocument.createProcessingInstruction("xml","version=\"1.0\"");
    domDocument.appendChild(header);
    QDomElement fdroid = domDocument.createElement("fdroid");
    fdroid.appendChild(rinfo.toXML());
    domDocument.appendChild(fdroid);


    XMLWrite(path+"/replink.xml",domDocument, false);
}

QString CXMLMethods::MergeRepo(QString xmlFilePath, QString xmlFilePathDos)
{
    QDomDocument domDocument;
    QDomDocument domDocumentDos;

    QString error="";
    QFile* file = new QFile(xmlFilePath);
    QFile* fileDos = new QFile(xmlFilePathDos);

    if (!file->open(QFile::ReadOnly | QFile::Text)) {
        error=QString("Can\'t read file %1:\n%2.").arg(xmlFilePath).arg(file->errorString());
        std::cout<< error.toStdString()<< std::endl;
        delete file;
        return error;
    }else if(!fileDos->open(QFile::ReadOnly | QFile::Text)){
        error=QString("Can\'t read file %1:\n%2.").arg(xmlFilePathDos).arg(file->errorString());
        std::cout<< error.toStdString()<< std::endl;
        delete file;
        return error;
    }else{
        QString errorStr;
        int errorLine;
        int errorColumn;

        if (!domDocument.setContent(file, true, &errorStr, &errorLine, &errorColumn)) {
            error=QString("Error de Parseo en la linea %1, columna %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
            std::cout<< error.toStdString()<< std::endl;
            delete file;
            return error;
        }else if(!domDocumentDos.setContent(fileDos, true, &errorStr, &errorLine, &errorColumn)){
            error=QString("Error de Parseo en la linea %1, columna %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
            std::cout<< error.toStdString()<< std::endl;
            delete file;
            return error;
        }else{
            file->close();
            fileDos->close();
            delete file;
            delete fileDos;
            QDomElement fdroid = domDocument.documentElement();
            QDomElement fdroidDos = domDocumentDos.documentElement();

            if (fdroid.tagName() != "fdroid") {
                error=QString("El archivo no posee la estructura RepLink.");
                std::cout<< error.toStdString()<< std::endl;
                return error;
            }else if(fdroidDos.tagName() != "fdroid"){
                error=QString("El archivo a unir no posee la estructura RepLink.");
                std::cout<< error.toStdString()<< std::endl;
                return error;
            }else {
                QString id;
                QString idDos;

                QDomNodeList listaApps = fdroid.elementsByTagName("application");
                QDomNodeList listaAppsDos = fdroidDos.elementsByTagName("application");

                bool flag=false;
                int cantActual=listaApps.length();

                for (int i = listaAppsDos.length(); i >= 0 ; --i) {
                    idDos= listaAppsDos.at(i).toElement().elementsByTagName("id").at(0).firstChild().nodeValue();

                    for (int j = 0; j < cantActual; ++j) {
                        id= listaApps.at(j).toElement().elementsByTagName("id").at(0).firstChild().nodeValue();

                        if(id==idDos){
                            flag=true;
                            break;
                        }
                    }

                    if(!flag){
                        domDocument.documentElement().appendChild(listaAppsDos.at(i));

                    }else{
                        flag=false;
                    }

                    id="";
                    idDos="";
                }

                return XMLWrite(xmlFilePath,domDocument);
            }
        }
    }
}

QString CXMLMethods::SubstractRepo(QString xmlFilePath, QString xmlFilePathDos)
{
    QDomDocument domDocument;
    QDomDocument domDocumentDos;

    QString error="";
    QFile* file = new QFile(xmlFilePath);
    QFile* fileDos = new QFile(xmlFilePathDos);

    if (!file->open(QFile::ReadOnly | QFile::Text)) {
        error=QString("Can\'t read file %1:\n%2.").arg(xmlFilePath).arg(file->errorString());
        std::cout<< error.toStdString()<< std::endl;
        delete file;
        return error;
    }else if(!fileDos->open(QFile::ReadOnly | QFile::Text)){
        error=QString("Can\'t read file %1:\n%2.").arg(xmlFilePathDos).arg(file->errorString());
        std::cout<< error.toStdString()<< std::endl;
        delete file;
        return error;
    }else{
        QString errorStr;
        int errorLine;
        int errorColumn;

        if (!domDocument.setContent(file, true, &errorStr, &errorLine, &errorColumn)) {
            error=QString("Error de Parseo en la linea %1, columna %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
            std::cout<< error.toStdString()<< std::endl;
            delete file;
            return error;
        }else if(!domDocumentDos.setContent(fileDos, true, &errorStr, &errorLine, &errorColumn)){
            error=QString("Error de Parseo en la linea %1, columna %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
            std::cout<< error.toStdString()<< std::endl;
            delete file;
            return error;
        }else{
            file->close();
            fileDos->close();
            delete file;
            delete fileDos;
            QDomElement fdroid = domDocument.documentElement();
            QDomElement fdroidDos = domDocumentDos.documentElement();

            if (fdroid.tagName() != "fdroid") {
                error=QString("El archivo no posee la estructura RepLink.");
                std::cout<< error.toStdString()<< std::endl;
                return error;
            }else if(fdroidDos.tagName() != "fdroid"){
                error=QString("El archivo a unir no posee la estructura RepLink.");
                std::cout<< error.toStdString()<< std::endl;
                return error;
            }else {
                QString id;
                QString idDos;

                QDomNodeList listaApps = fdroid.elementsByTagName("application");
                QDomNodeList listaAppsDos = fdroidDos.elementsByTagName("application");

                int cantApps=listaApps.length();

                for (int i =0 ; i <listaAppsDos.length() ; i++) {
                    idDos= listaAppsDos.at(i).toElement().elementsByTagName("id").at(0).firstChild().nodeValue();

                    for (int j = 0; j < cantApps; j++) {
                        id= listaApps.at(j).toElement().elementsByTagName("id").at(0).firstChild().nodeValue();

                        if(id==idDos){
                            //                            if(id=="com.cmcm.locker"){
                            //                               j+=1;
                            //                               j-=1;
                            //                            }

                            QDomNode pkg=listaApps.at(j).toElement().elementsByTagName("package").at(0);
                            long vercode= pkg.toElement().elementsByTagName("versioncode").at(0).firstChild().nodeValue().toLongLong();
                            QDomNode pkg2=listaAppsDos.at(i).toElement().elementsByTagName("package").at(0);
                            QString vercodeDos=pkg2.toElement().elementsByTagName("versioncode").at(0).firstChild().nodeValue();

                            if(vercodeDos.toLongLong()>=vercode){
                                domDocument.documentElement().removeChild(listaApps.at(j));
                                cantApps--;
                                break;
                            }
                        }
                    }

                    id="";
                    idDos="";
                }

                return XMLWrite(xmlFilePath,domDocument);
            }
        }
    }
}

QString CXMLMethods::UpdateAppInfo(QString xmlFilePath, AppInfo appInfo)
{
    QDomDocument domDocument;

    QString error="";
    QFile* file = new QFile(xmlFilePath);

    if (!file->open(QFile::ReadOnly | QFile::Text)) {
        error=QString("Can\'t read file %1:\n%2.").arg(xmlFilePath).arg(file->errorString());
        std::cout<< error.toStdString()<< std::endl;
        delete file;
        return error;
    }else{
        QString errorStr;
        int errorLine;
        int errorColumn;

        if (!domDocument.setContent(file, true, &errorStr, &errorLine, &errorColumn)) {
            error=QString("Error de Parseo en la linea %1, columna %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
            std::cout<< error.toStdString()<< std::endl;
            delete file;
            return error;
        }else{
            file->close();
            delete file;
            QDomElement fdroid = domDocument.documentElement();

            if (fdroid.tagName() != "fdroid") {
                error=QString("El archivo no posee la estructura RepLink.");
                std::cout<< error.toStdString()<< std::endl;
                return error;
            }else {
                QString id;

                QDomNodeList listaApps = fdroid.elementsByTagName("application");

                for (int i = 0; i < listaApps.length(); ++i) {
                    id= listaApps.at(i).toElement().elementsByTagName("id").at(0).firstChild().nodeValue();
                    if(id==appInfo.ObtenerId()){

                        QDomText name;
                        QDomElement tempName=domDocument.createElement("name");
                        QString n=appInfo.ObtenerName();

                        name=domDocument.createTextNode(n);
                        tempName.appendChild(name);

                        domDocument.documentElement().elementsByTagName("application").at(i).toElement().insertAfter(tempName,domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("name").at(0));
                        domDocument.documentElement().elementsByTagName("application").at(i).removeChild(domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("name").at(0));


                        domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("summary").at(0).firstChild().setNodeValue(appInfo.ObtenerSummary());

                        QDomText descrip;
                        QDomElement tempDesc=domDocument.createElement("desc");
                        QString text=appInfo.ObtenerDescription();
                        //text.replace("<p>","&lt;p&gt;");
                        //text.replace("</p>","&lt;/p&gt;");

                        descrip=domDocument.createTextNode(text.toHtmlEscaped());
                        tempDesc.appendChild(descrip);

                        domDocument.documentElement().elementsByTagName("application").at(i).toElement().insertAfter(tempDesc,domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("desc").at(0));
                        if(domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("desc").count()>1){
                            domDocument.documentElement().elementsByTagName("application").at(i).removeChild(domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("desc").at(0));
                        }

                        domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("license").at(0).firstChild().setNodeValue(appInfo.ObtenerLicense());
                        domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("category").at(0).firstChild().setNodeValue(appInfo.ObtenerCategory());
                        domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("categories").at(0).firstChild().setNodeValue(appInfo.ObtenerCategories());
                        //Source
                        QDomText source;
                        QDomElement tempSource=domDocument.createElement("source");
                        QString s=appInfo.ObtenerSource();

                        source=domDocument.createTextNode(s);
                        tempSource.appendChild(source);

                        domDocument.documentElement().elementsByTagName("application").at(i).toElement().insertAfter(tempSource,domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("source").at(0));
                        if(domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("source").count()>1){
                            domDocument.documentElement().elementsByTagName("application").at(i).removeChild(domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("source").at(0));
                        }
                        //Requerimientos
                        QDomText req;
                        QDomElement tempReq=domDocument.createElement("requirements");
                        QString r=appInfo.ObtenerRequirements();

                        req=domDocument.createTextNode(r);
                        tempReq.appendChild(req);


                        domDocument.documentElement().elementsByTagName("application").at(i).toElement().insertAfter(tempReq,domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("requirements").at(0));
                        if(domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("requirements").count()>1){
                            domDocument.documentElement().elementsByTagName("application").at(i).removeChild(domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("requirements").at(0));
                        }


                        if(appInfo.ObtenerVersionCodeToObb()!=""){
                            QString vcObb;
                            QDomNodeList listaPkgs = fdroid.elementsByTagName("application").at(i).toElement().elementsByTagName("package");

                            for (int j = 0; j < listaPkgs.length(); ++j) {
                                vcObb= listaPkgs.at(j).toElement().elementsByTagName("versioncode").at(0).firstChild().nodeValue();
                                if(vcObb==appInfo.ObtenerVersionCodeToObb()){
                                    QStringList repo = xmlFilePath.split( "/" );
                                    repo.removeLast();
                                    if(appInfo.ObtenerMainObb()!=""){

                                        QCryptographicHash::Algorithm hashAlgorithm=QCryptographicHash::Sha256;
                                        QString obbRuta=repo.join("//")+"//"+appInfo.ObtenerMainObb();
                                        QByteArray checksumMO=fileChecksum(obbRuta,hashAlgorithm);

                                        QDomText mainObb;
                                        QDomText mainObbSha;
                                        QDomElement tempMainObb=domDocument.createElement("obbMainFile");
                                        QDomElement tempMainObbSha=domDocument.createElement("obbMainFileSha256");
                                        QString m=appInfo.ObtenerMainObb();
                                        QString ms=checksumMO.toHex();

                                        mainObb=domDocument.createTextNode(m);
                                        mainObbSha=domDocument.createTextNode(ms);
                                        tempMainObb.appendChild(mainObb);
                                        tempMainObbSha.appendChild(mainObbSha);
                                        if(listaPkgs.at(j).toElement().elementsByTagName("obbMainFile").count()>0){
                                            domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().insertAfter(tempMainObb,domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().elementsByTagName("obbMainFile").at(0));
                                            domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().insertAfter(tempMainObbSha,domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().elementsByTagName("obbMainFileSha256").at(0));

                                            if(listaPkgs.at(j).toElement().elementsByTagName("obbMainFile").count()>1){
                                                domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().removeChild(domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().elementsByTagName("obbMainFile").at(0));
                                                domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().removeChild(domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().elementsByTagName("obbMainFileSha256").at(0));

                                            }
                                        }else{
                                            domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().insertAfter(tempMainObb,domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().elementsByTagName("targetSdkVersion").at(0));
                                            domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().insertAfter(tempMainObbSha,domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().elementsByTagName("obbMainFile").at(0));
                                        }




                                        if(appInfo.ObtenerPatchObb()!=""){

                                            obbRuta=repo.join("//")+"//"+appInfo.ObtenerPatchObb();
                                            QByteArray checksumPO=fileChecksum(obbRuta,hashAlgorithm);

                                            QDomText patchObb;
                                            QDomText patchObbSha;
                                            QDomElement tempPatchObb=domDocument.createElement("obbPatchFile");
                                            QDomElement tempPatchObbSha=domDocument.createElement("obbPatchFileSha256");
                                            QString p=appInfo.ObtenerPatchObb();
                                            QString ps=checksumPO.toHex();

                                            patchObb=domDocument.createTextNode(p);
                                            patchObbSha=domDocument.createTextNode(ps);
                                            tempPatchObb.appendChild(patchObb);
                                            tempPatchObbSha.appendChild(patchObbSha);
                                            if(listaPkgs.at(j).toElement().elementsByTagName("obbPatchFile").count()>0){
                                                domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().insertAfter(tempPatchObb,domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().elementsByTagName("obbPatchFile").at(0));
                                                domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().insertAfter(tempPatchObbSha,domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().elementsByTagName("obbPatchFileSha256").at(0));

                                                if(listaPkgs.at(j).toElement().elementsByTagName("obbPatchFile").count()>1){
                                                    domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().removeChild(domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().elementsByTagName("obbPatchFile").at(0));
                                                    domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().removeChild(domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().elementsByTagName("obbPatchFileSha256").at(0));

                                                }
                                            }else{
                                                domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().insertAfter(tempPatchObb,domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().elementsByTagName("obbMainFileSha256").at(0));
                                                domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().insertAfter(tempPatchObbSha,domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().elementsByTagName("obbPatchFile").at(0));
                                            }

                                        }else{
                                            if(listaPkgs.at(j).toElement().elementsByTagName("obbPatchFile").count()>0){
                                                domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().removeChild(domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().elementsByTagName("obbPatchFile").at(0));
                                                domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().removeChild(domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().elementsByTagName("obbPatchFileSha256").at(0));
                                            }
                                        }

                                    }else{
                                        if(listaPkgs.at(j).toElement().elementsByTagName("obbMainFile").count()>0){
                                            domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().removeChild(domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().elementsByTagName("obbMainFile").at(0));
                                            domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().removeChild(domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().elementsByTagName("obbMainFileSha256").at(0));
                                        }

                                        if(listaPkgs.at(j).toElement().elementsByTagName("obbPatchFile").count()>0){
                                            domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().removeChild(domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().elementsByTagName("obbPatchFile").at(0));
                                            domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().removeChild(domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().elementsByTagName("obbPatchFileSha256").at(0));
                                        }
                                    }


                                    //Source
                                    QDomText size;
                                    QDomElement tempSize=domDocument.createElement("size");
                                    QString sz= GetPkgSize(repo.join("//"),domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement());
;

                                    size=domDocument.createTextNode(sz);
                                    tempSize.appendChild(size);

                                    domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().insertAfter(tempSize,domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().elementsByTagName("size").at(0));

                                    if(listaPkgs.at(j).toElement().elementsByTagName("size").count()>1){
                                        domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().removeChild(domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package").at(j).toElement().elementsByTagName("size").at(0));
                                    }

                                }


                            }
                        }





                    }
                }




                return XMLWrite(xmlFilePath,domDocument);
            }
        }
    }
}

QString CXMLMethods::UpdateRepoInfo(QString xmlFilePath, RepoInfo repoInfo)
{
    QDomDocument domDocument;

    QString error="";
    QFile* file = new QFile(xmlFilePath);

    if (!file->open(QFile::ReadOnly | QFile::Text)) {
        error=QString("Can\'t read file %1:\n%2.").arg(xmlFilePath).arg(file->errorString());
        std::cout<< error.toStdString()<< std::endl;
        delete file;
        return error;
    }else{
        QString errorStr;
        int errorLine;
        int errorColumn;

        if (!domDocument.setContent(file, true, &errorStr, &errorLine, &errorColumn)) {
            error=QString("Error de Parseo en la linea %1, columna %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
            std::cout<< error.toStdString()<< std::endl;
            delete file;
            return error;
        }else{
            file->close();
            delete file;
            QDomElement fdroid = domDocument.documentElement();

            if (fdroid.tagName() != "fdroid") {
                error=QString("El archivo no posee la estructura RepLink.");
                std::cout<< error.toStdString()<< std::endl;
                return error;
            }else {
                QDomText descrip;
                QDomElement tempDesc=domDocument.createElement("description");
                QString text=repoInfo.ObtenerDescription();
                descrip=domDocument.createTextNode(text);
                tempDesc.appendChild(descrip);

                domDocument.documentElement().elementsByTagName("repo").at(0).toElement().insertAfter(tempDesc,domDocument.documentElement().elementsByTagName("repo").at(0).toElement().elementsByTagName("description").at(0));
                domDocument.documentElement().elementsByTagName("repo").at(0).removeChild(domDocument.documentElement().elementsByTagName("repo").at(0).toElement().elementsByTagName("description").at(0));

                domDocument.documentElement().elementsByTagName("repo").at(0).attributes().namedItem("url").setNodeValue(repoInfo.ObtenerUrl());
                domDocument.documentElement().elementsByTagName("repo").at(0).attributes().namedItem("name").setNodeValue(repoInfo.ObtenerName());

                return XMLWrite(xmlFilePath,domDocument,false);
            }
        }
    }
}

QString CXMLMethods::UpdateApkName(QString xmlFilePath, QString oldName, QString newName)
{
    QDomDocument domDocument;

    QString error="";
    QFile* file = new QFile(xmlFilePath);

    if (!file->open(QFile::ReadOnly | QFile::Text)) {
        error=QString("Can\'t read file %1:\n%2.").arg(xmlFilePath).arg(file->errorString());
        std::cout<< error.toStdString()<< std::endl;
        delete file;
        return error;
    }else{
        QString errorStr;
        int errorLine;
        int errorColumn;

        if (!domDocument.setContent(file, true, &errorStr, &errorLine, &errorColumn)) {
            error=QString("Error de Parseo en la linea %1, columna %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
            std::cout<< error.toStdString()<< std::endl;
            delete file;
            return error;
        }else{
            file->close();
            delete file;
            QDomElement fdroid = domDocument.documentElement();

            if (fdroid.tagName() != "fdroid") {
                error=QString("El archivo no posee la estructura RepLink.");
                std::cout<< error.toStdString()<< std::endl;
                return error;
            }else {
                QString document=domDocument.toString().replace(oldName,newName);
                domDocument.setContent(document);

                return XMLWrite(xmlFilePath,domDocument);
            }
        }
    }
}

QString CXMLMethods::DeleteApp(QString xmlFilePath, AppInfo appInfo)
{
    QDomDocument domDocument;

    QString error="";
    QFile* file = new QFile(xmlFilePath);

    if (!file->open(QFile::ReadOnly | QFile::Text)) {
        error=QString("Can\'t read file %1:\n%2.").arg(xmlFilePath).arg(file->errorString());
        std::cout<< error.toStdString()<< std::endl;
        delete file;
        return error;
    }else{
        QString errorStr;
        int errorLine;
        int errorColumn;

        if (!domDocument.setContent(file, true, &errorStr, &errorLine, &errorColumn)) {
            error=QString("Error de Parseo en la linea %1, columna %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
            std::cout<< error.toStdString()<< std::endl;
            delete file;
            return error;
        }else{
            file->close();
            delete file;
            QDomElement fdroid = domDocument.documentElement();

            if (fdroid.tagName() != "fdroid") {
                error=QString("El archivo no posee la estructura RepLink.");
                std::cout<< error.toStdString()<< std::endl;
                return error;
            }else {
                QString id;

                QDomNodeList listaApps = fdroid.elementsByTagName("application");

                for (int i = 0; i < listaApps.length(); ++i) {
                    id= listaApps.at(i).toElement().elementsByTagName("id").at(0).firstChild().nodeValue();
                    if(id==appInfo.ObtenerId()){
                        QDomNode app= domDocument.documentElement().elementsByTagName("application").at(i);

                        domDocument.documentElement().removeChild(app);
                        break;
                    }
                }

                return XMLWrite(xmlFilePath,domDocument);
            }
        }
    }
}

QString CXMLMethods::DeletePkg(QString xmlFilePath, PkgInfo pkgInfo)
{
    QDomDocument domDocument;

    QString error="";
    QFile* file = new QFile(xmlFilePath);

    if (!file->open(QFile::ReadOnly | QFile::Text)) {
        error=QString("Can\'t read file %1:\n%2.").arg(xmlFilePath).arg(file->errorString());
        std::cout<< error.toStdString()<< std::endl;
        delete file;
        return error;
    }else{
        QString errorStr;
        int errorLine;
        int errorColumn;

        if (!domDocument.setContent(file, true, &errorStr, &errorLine, &errorColumn)) {
            error=QString("Error de Parseo en la linea %1, columna %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
            std::cout<< error.toStdString()<< std::endl;
            delete file;
            return error;
        }else{
            file->close();
            delete file;
            QDomElement fdroid = domDocument.documentElement();

            if (fdroid.tagName() != "fdroid") {
                error=QString("El archivo no posee la estructura RepLink.");
                std::cout<< error.toStdString()<< std::endl;
                return error;
            }else {


                QDomNodeList listaApps = fdroid.elementsByTagName("application");
                QDomNodeList pkgs;
                QString id;

                for (int i = 0; i < listaApps.length(); ++i) {
                    id= listaApps.at(i).toElement().elementsByTagName("id").at(0).firstChild().nodeValue();
                    if(id==pkgInfo.ObtenerId()){
                        pkgs= domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package");

                        QDomNode pkg;
                        for (int j = 0; j < pkgs.length(); ++j) {
                            pkg=pkgs.at(j);

                            QString version= pkg.toElement().elementsByTagName("version").at(0).firstChild().nodeValue();

                            if(version==pkgInfo.ObtenerVersion()){

                                domDocument.documentElement().elementsByTagName("application").at(i).removeChild(pkg);
                                break;
                            }
                        }


                        break;

                    }
                }

                return XMLWrite(xmlFilePath,domDocument);
            }
        }
    }
}


QString CXMLMethods::DeleteAllPkg(QString xmlFilePath)
{
    QDomDocument domDocument;

    QString error="";
    QFile* file = new QFile(xmlFilePath);

    if (!file->open(QFile::ReadOnly | QFile::Text)) {
        error=QString("Can\'t read file %1:\n%2.").arg(xmlFilePath).arg(file->errorString());
        std::cout<< error.toStdString()<< std::endl;
        delete file;
        return error;
    }else{
        QString errorStr;
        int errorLine;
        int errorColumn;

        if (!domDocument.setContent(file, true, &errorStr, &errorLine, &errorColumn)) {
            error=QString("Error de Parseo en la linea %1, columna %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
            std::cout<< error.toStdString()<< std::endl;
            delete file;
            return error;
        }else{
            file->close();
            delete file;
            QDomElement fdroid = domDocument.documentElement();

            if (fdroid.tagName() != "fdroid") {
                error=QString("El archivo no posee la estructura RepLink.");
                std::cout<< error.toStdString()<< std::endl;
                return error;
            }else {


                QDomNodeList listaApps = fdroid.elementsByTagName("application");
                QDomNodeList pkgs;

                for (int i = 0; i < listaApps.length(); ++i) {
                    pkgs= domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("package");

                    QDomNode pkg;
                    for (int j = pkgs.length()-1; j > -1; --j) {
                        pkg=pkgs.at(j);
                        domDocument.documentElement().elementsByTagName("application").at(i).removeChild(pkg);
                    }
                }
                return XMLWrite(xmlFilePath,domDocument);
            }
        }
    }
}

QString CXMLMethods::SummaryLikeCategory(QString xmlFilePath)
{
    QDomDocument domDocument;

    QString error="";
    QFile* file = new QFile(xmlFilePath);

    if (!file->open(QFile::ReadOnly | QFile::Text)) {
        error=QString("Can\'t read file %1:\n%2.").arg(xmlFilePath).arg(file->errorString());
        std::cout<< error.toStdString()<< std::endl;
        delete file;
        return error;
    }else{
        QString errorStr;
        int errorLine;
        int errorColumn;

        if (!domDocument.setContent(file, true, &errorStr, &errorLine, &errorColumn)) {
            error=QString("Error de Parseo en la linea %1, columna %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
            std::cout<< error.toStdString()<< std::endl;
            delete file;
            return error;
        }else{
            file->close();
            delete file;
            QDomElement fdroid = domDocument.documentElement();

            if (fdroid.tagName() != "fdroid") {
                error=QString("El archivo no posee la estructura RepLink.");
                std::cout<< error.toStdString()<< std::endl;
                return error;
            }else {


                QDomNodeList listaApps = fdroid.elementsByTagName("application");
                QString summaryAct= "Unknown";
                QString categoryAct= "No Category";

                for (int i = 0; i < listaApps.length(); ++i) {
                    summaryAct= listaApps.at(i).toElement().elementsByTagName("summary").at(0).firstChild().nodeValue();
                    categoryAct= listaApps.at(i).toElement().elementsByTagName("category").at(0).firstChild().nodeValue();

                    if(summaryAct=="Unknown"){
                        domDocument.documentElement().elementsByTagName("application").at(i).toElement().elementsByTagName("summary").at(0).firstChild().setNodeValue(categoryAct);
                    }

                    summaryAct= "Unknown";
                    categoryAct= "No Category";

                }
                return XMLWrite(xmlFilePath,domDocument);
            }
        }
    }
}


QStringList CXMLMethods::UsedAPKs(QString xmlFilePath)
{
    QDomDocument domDocument;
    QStringList usedApks;

    QString error="";
    QFile* file = new QFile(xmlFilePath);

    if (!file->open(QFile::ReadOnly | QFile::Text)) {
        error=QString("Can\'t read file %1:\n%2.").arg(xmlFilePath).arg(file->errorString());
        std::cout<< error.toStdString()<< std::endl;
        emit Notice(error,1);
        delete file;
    }else{
        QString errorStr;
        int errorLine;
        int errorColumn;

        if (!domDocument.setContent(file, true, &errorStr, &errorLine, &errorColumn)) {
            error=QString("Error de Parseo en la linea %1, columna %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
            std::cout<< error.toStdString()<< std::endl;
            emit Notice(error,1);
            delete file;
        }else{
            file->close();
            delete file;
            QDomElement fdroid = domDocument.documentElement();

            if (fdroid.tagName() != "fdroid") {
                error=QString("El archivo no posee la estructura RepLink.");
                emit Notice(error,1);
            }else {
                QDomNodeList apks=fdroid.elementsByTagName("apkname");
                for (int k = 0; k < apks.length(); ++k) {
                    usedApks << apks.at(k).firstChild().nodeValue();
                }
            }
        }
    }
    return usedApks;
}

QString CXMLMethods::UpdateTimeStamp(QDomDocument &domDocument)
{
    QDomElement fdroid = domDocument.documentElement();
    QString error="";

    if (fdroid.tagName() != "fdroid") {
        error=QString("El archivo no posee la estructura RepLink.");
        return error;
    }else {
        QDomNodeList repo=fdroid.elementsByTagName("repo");
        QString currentTS=QDateTime().currentDateTime().toString("yyyyMMddhhmmss");
        repo.at(0).attributes().namedItem("timestamp").setNodeValue(currentTS);
        return "ok";
    }
}

void CXMLMethods::CreateSignedIndexJar(QString xmlFilePath)
{
    QFileInfo info = QFileInfo(xmlFilePath);

    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       "RepLink APK Manager", "RepLinkAPK");

    QString appPath=settings.value("AppDirs/path").toString()+"/keystores/";

    QString keystore="\""+appPath+settings.value("KeyStoreConf/keystore").toString()+"\"";
    QString storepass=settings.value("KeyStoreConf/storepass").toString();
    QString key=settings.value("KeyStoreConf/key").toString();
    QString keypass=settings.value("KeyStoreConf/keypass").toString();
    QString dir=info.absolutePath();

    QProcess cmd;

    QString javaDir="\"";


    javaDir+=settings.value("AppDirs/java").toString()+"/";


    if(QFile(settings.value("AppDirs/java").toString().trimmed()+"/jarsigner.exe").exists()&&
            QFile(settings.value("AppDirs/java").toString().trimmed()+"/jar.exe").exists()){
        QDir d(dir+"/rootxml");
        QFile f(info.absoluteFilePath());
        if(d.exists()){
            QFile index(dir+"/rootxml/index.xml");
            if(index.exists()){
                if(index.remove()){
                    f.copy(QDir::toNativeSeparators(dir+"/rootxml/index.xml"));
                }else{
                    emit Notice("Error: Can't copy de index file!",1);
                }
            }else{
                f.copy(QDir::toNativeSeparators(dir+"/rootxml/index.xml"));
            }

        }else{
            d.mkdir(QDir::toNativeSeparators(dir+"/rootxml"));
            f.copy(QDir::toNativeSeparators(dir+"/rootxml/index.xml"));
        }

        QString command= javaDir+"jar\" -cf \""+dir+"/index.jar\" -C \""+dir+"/rootxml\" index.xml";
      //  qDebug()<<command;
        cmd.start(command.replace("/","\\") );
        cmd.waitForFinished(-1);

        command=javaDir+"jarsigner\" -keystore "+keystore+" -storepass "+storepass+" -digestalg SHA1 -sigalg SHA1withRSA \""+dir+"/index.jar\" "+key+" -keypass "+keypass+ "";
       // qDebug()<<command;
        cmd.start(command.replace("/","\\") );
        cmd.waitForFinished(-1);
    }else{
        emit Notice("Error: Instale JDK v1.8.0",1);
    }
}

void CXMLMethods::CreateSignedIndexV1Jar(QString repo,QString jsonFilePath)
{
    QFileInfo info = QFileInfo(jsonFilePath);
    QDir dirRepo= QDir(repo);

    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       "RepLink APK Manager", "RepLinkAPK");

    QString appPath=settings.value("AppDirs/path").toString()+"/keystores/";

    QString keystore="\""+appPath+settings.value("KeyStoreConf/keystore").toString()+"\"";
    QString storepass=settings.value("KeyStoreConf/storepass").toString();
    QString key=settings.value("KeyStoreConf/key").toString();
    QString keypass=settings.value("KeyStoreConf/keypass").toString();
    QString dir=info.absolutePath();

    QProcess cmd;

    QString javaDir="\"";


    javaDir+=settings.value("AppDirs/java").toString()+"/";


    if(QFile(settings.value("AppDirs/java").toString().trimmed()+"/jarsigner.exe").exists()&&
            QFile(settings.value("AppDirs/java").toString().trimmed()+"/jar.exe").exists()){
        QFile index(dir+"/index-v1.json");
        if(index.exists()){
            QString command= javaDir+"jar\" -cf \""+dirRepo.absolutePath()+"/index-v1.jar\" -C \""+dirRepo.absolutePath()+"/rootxml\" index-v1.json";
            cmd.start(command.replace("/","\\") );
            cmd.waitForFinished(-1);

            command=javaDir+"jarsigner\" -keystore "+keystore+" -storepass "+storepass+" -digestalg SHA1 -sigalg SHA1withRSA \""+dirRepo.absolutePath()+"/index-v1.jar\" "+key+" -keypass "+keypass+ "";
            cmd.start(command.replace("/","\\") );
            cmd.waitForFinished(-1);
        }else{
            emit Notice("Error: No existe el archivo JSON",1);
        }



    }else{
        emit Notice("Error: Instale JDK v1.8.0",1);
    }
}

void CXMLMethods::SignIndexXML(QString xmlFilePath)
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

                QDomNodeList repo=fdroid.elementsByTagName("repo");
                QString pubkey=GetPubKey();
                repo.at(0).toElement().setAttribute("pubkey",pubkey);

                XMLWrite(xmlFilePath,domDocument,false);
            }
        }
    }
}

QString CXMLMethods::GetPubKey()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       "RepLink APK Manager", "RepLinkAPK");

    QString appPath=settings.value("AppDirs/path").toString()+"/keystores/";


    QString keystore=appPath+settings.value("KeyStoreConf/keystore").toString();
    QString storepass=settings.value("KeyStoreConf/storepass").toString();
    QString key=settings.value("KeyStoreConf/key").toString();

    QProcess cmd;
    QString javaDir="\"";

    javaDir+=settings.value("AppDirs/java").toString()+"/";

    QString error="";

    QString pubkey="";

    if(QFile(settings.value("AppDirs/java").toString().trimmed()+"/keytool.exe").exists()){
        QFileInfo ks(keystore);
        if(ks.exists()){
            QString command= javaDir+"keytool.exe\" -exportcert -alias "+key+" -keystore \""+ks.absoluteFilePath()+"\" -storepass "+storepass;

            //std::cout << command.toStdString() << std::endl;
            cmd.start(command.replace("/","\\"));
            cmd.waitForFinished(-1);
            QByteArray respose=cmd.readAllStandardOutput();
            pubkey=QString().fromStdString(respose.toHex().toStdString());
        }else{
            error+="Error: Key Store not found! ";
            emit Notice(error,1);
        }
    }else{
        error+="Error: Instale JDK v1.8.0 ";
        emit Notice(error,1);
    }

    if(pubkey==""){
        emit Notice(error+"Error: Couldn't get the pub key! ",1);
    }
    return pubkey;
}

int CXMLMethods::GetAppLatestVersion(QString xmlFilePath, QString id)
{
    QDomDocument domDocument;

    QString error="";
    QFile* file = new QFile(xmlFilePath);

    if (!file->open(QFile::ReadOnly | QFile::Text)) {
        error=QString("Can\'t read file %1:\n%2.").arg(xmlFilePath).arg(file->errorString());
        std::cout<< error.toStdString()<< std::endl;
        delete file;
        return -1;
    }else{
        QString errorStr;
        int errorLine;
        int errorColumn;

        if (!domDocument.setContent(file, true, &errorStr, &errorLine, &errorColumn)) {
            error=QString("Error de Parseo en la linea %1, columna %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
            std::cout<< error.toStdString()<< std::endl;
            delete file;
            return -1;
        }else{
            file->close();
            delete file;
            QDomElement fdroid = domDocument.documentElement();

            if (fdroid.tagName() != "fdroid") {
                error=QString("El archivo no posee la estructura RepLink.");
                return -1;
            }else {
                QString vercode="-1";

                QDomNodeList listaApps = fdroid.elementsByTagName("application");



                for (int i = 0; i < listaApps.length(); ++i) {
                    QString idx= listaApps.at(i).toElement().elementsByTagName("id").at(0).firstChild().nodeValue();
                    if(id==idx){
                        vercode= listaApps.at(i).toElement().elementsByTagName("package").at(0).toElement().elementsByTagName("versioncode").at(0).firstChild().nodeValue();
                        break;
                    }
                }

                return vercode.toInt();
            }
        }
    }
    //return -1;
}

QByteArray CXMLMethods::fileChecksum(const QString &fileName, QCryptographicHash::Algorithm hashAlgorithm)
{
    QFile f(fileName);
    if (f.open(QFile::ReadOnly)) {
        QCryptographicHash hash(hashAlgorithm);
        if (hash.addData(&f)) {
            return hash.result();
        }
    }
    return QByteArray();
}

QString CXMLMethods::GetPkgSize(QString repo, QDomElement pkg)
{
    qint64 apkSize = 0;
    qint64 mainObbSize = 0;
    qint64 patchObbSize = 0;
    qint64 totalSize = 0;

    if(pkg.elementsByTagName("obbMainFile").count()>0){
        QString rutaMainObb = repo+"//" + pkg.elementsByTagName("obbMainFile").at(0).firstChild().nodeValue();
        mainObbSize = QFileInfo(rutaMainObb).size();

        if(pkg.elementsByTagName("obbPatchFile").count()>0){
            QString rutaPatchObb = repo+"//" + pkg.elementsByTagName("obbPatchFile").at(0).firstChild().nodeValue();
            patchObbSize = QFileInfo(rutaPatchObb).size();
        }
    }

    QString rutaApk = repo+"//" + pkg.elementsByTagName("apkname").at(0).firstChild().nodeValue();
    apkSize= QFileInfo(rutaApk).size();

    totalSize = apkSize + mainObbSize + patchObbSize;

    return QString::number(totalSize);
}
