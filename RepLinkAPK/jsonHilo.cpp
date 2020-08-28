#include <jsonHilo.h>


CJSONHilo::CJSONHilo(QObject *parent): QThread(parent){
    Q_UNUSED(parent);

}

void CJSONHilo::SetParams(CXMLMethods *xmlM, QString repo){
    this->repo=repo;
    this->xmlFilePath=repo+"/replink.xml";
    this->jsonFilePath=repo+"/rootxml/index-v1.json";
    this->xmlM=xmlM;
}

void CJSONHilo::run()
{
    QDomDocument domDocument;
    QString jsonIndexV1="";

    QString error="";
    QFile* file = new QFile(xmlFilePath);

    if (!file->open(QFile::ReadOnly | QFile::Text)) {
        error=QString("Can't read file %1:\n%2.").arg(xmlFilePath).arg(file->errorString());
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
                QString rdesc="Error";
                QDomNodeList repo=fdroid.elementsByTagName("repo");
                rdesc=repo.at(0).toElement().elementsByTagName("description").at(0).firstChild().nodeValue();
                QString url=repo.at(0).attributes().namedItem("url").nodeValue();
                QString nameR=repo.at(0).attributes().namedItem("name").nodeValue();
                //QString pubkey=repo.at(0).attributes().namedItem("pubkey").nodeValue();
                //QString version=repo.at(0).attributes().namedItem("version").nodeValue();
                QString iconR=repo.at(0).attributes().namedItem("icon").nodeValue();
                QString timestamp=repo.at(0).attributes().namedItem("timestamp").nodeValue();



                QString jsonIndexV1repo="{\"repo\": {\"timestamp\": "+timestamp+", \"version\": 19, \"maxage\": 14, \"name\": \""+nameR+"\", \"icon\": \""+iconR+"\", "+
                        "\"address\": \""+url+"\", \"description\": \""+rdesc+"\"}, \"requests\": {\"install\": [], \"uninstall\": []}, ";

                QString jsonIndexV1apps="\"apps\": [";

                QString jsonIndexV1pkgs="], \"packages\": {";

                QString jsonIndexV1end="}}";


                QString tempApp;
                QString id;
                QString name;
                QString summary;
                QString desc;
                QString license;
                QString category;
                QString categories;
                QString source;
                QString version;
                QString web;
                QString icon;
                QString marketversion;
                QString marketvercode;
                QString addedApp;
                QString update;
                QString localized;
                qint64 v1size;

                QDomNodeList listaApps = fdroid.elementsByTagName("application");

                for (int i = 0; i < listaApps.length(); ++i) {

                    id= listaApps.at(i).toElement().elementsByTagName("id").at(0).firstChild().nodeValue();
                    name= listaApps.at(i).toElement().elementsByTagName("name").at(0).firstChild().nodeValue();
                    summary= listaApps.at(i).toElement().elementsByTagName("summary").at(0).firstChild().nodeValue();

                    desc=listaApps.at(i).toElement().elementsByTagName("desc").at(0).firstChild().nodeValue();

                    license= listaApps.at(i).toElement().elementsByTagName("license").at(0).firstChild().nodeValue();
                    category= listaApps.at(i).toElement().elementsByTagName("category").at(0).firstChild().nodeValue();
                    categories= listaApps.at(i).toElement().elementsByTagName("categories").at(0).firstChild().nodeValue();
                    marketversion= listaApps.at(i).toElement().elementsByTagName("marketversion").at(0).firstChild().nodeValue();
                    marketvercode= listaApps.at(i).toElement().elementsByTagName("marketvercode").at(0).firstChild().nodeValue();
                    source= listaApps.at(i).toElement().elementsByTagName("source").at(0).firstChild().nodeValue();
                    web= listaApps.at(i).toElement().elementsByTagName("web").at(0).firstChild().nodeValue();
                    icon= listaApps.at(i).toElement().elementsByTagName("icon").at(0).firstChild().nodeValue();
                    addedApp= listaApps.at(i).toElement().elementsByTagName("added").at(0).firstChild().nodeValue();
                    update= listaApps.at(i).toElement().elementsByTagName("lastupdated").at(0).firstChild().nodeValue();
                    version= listaApps.at(i).toElement().elementsByTagName("package").at(0).toElement().elementsByTagName("version").at(0).firstChild().nodeValue();
                    v1size= listaApps.at(i).toElement().elementsByTagName("package").at(0).toElement().elementsByTagName("size").at(0).firstChild().nodeValue().toLong();

                    QDateTime addedAppDT= QDateTime();
                    addedAppDT.setDate(QDate(addedApp.mid(0,4).toInt(),addedApp.mid(5,2).toInt(),addedApp.mid(8,2).toInt()));
                    QString addedAppTS=QString::number(addedAppDT.toMSecsSinceEpoch());

                    QDateTime updateDT= QDateTime();
                    updateDT.setDate(QDate(update.mid(0,4).toInt(),update.mid(5,2).toInt(),update.mid(8,2).toInt()));
                    QString updateTS=QString::number(updateDT.toMSecsSinceEpoch());


                    QStringList catList = categories.split(",");


                    QString tempCats="";
                    for (int p = 0; p < catList.length(); ++p) {
                        tempCats+="\""+catList.at(p)+"\"";
                        if(p!=catList.length()-1){
                            tempCats+=", ";
                        }
                    }

                    localized="";
                    QString phoneScreenShots="";
                    QString featureGraphic ="";
                    QString iconLocalized ="";

                    if(QDir(this->repo+"/"+id+"/").exists()){
                        QFileInfo fG=QFileInfo(this->repo+"/"+id+"/es/featureGraphic.png");
                        if(fG.exists()){
                            featureGraphic ="\"featureGraphic\": \"featureGraphic.png\",";
                        }

                        QFileInfo iL=QFileInfo(this->repo+"/"+id+"/es/icon.png");
                        QFileInfo iLJpg=QFileInfo(this->repo+"/"+id+"/es/icon.jpg");

                        if(iL.exists()){
                            QImage imageIconLocalized= QImage(this->repo+"/"+id+"/es/icon.png");
                            if(imageIconLocalized.width()+imageIconLocalized.height() >= 512 ){
                                iconLocalized ="\"icon\": \"icon.png\",";
                            }
                        }else if(iLJpg.exists()){
                            QImage imageIconLocalized= QImage(this->repo+"/"+id+"/es/icon.jpg");
                            if(imageIconLocalized.width()+imageIconLocalized.height() >= 512 ){
                                iconLocalized ="\"icon\": \"icon.png\",";
                            }
                        }

                        QString screenshotsText="";

                        QStringList filters;
                        filters << "*.png" << "*.jpg" ;

                        QFileInfoList imgs= QDir(this->repo+"/"+id+"/es/phoneScreenshots/").entryInfoList(filters);

                        if(imgs.length()>0){
                            for (int s=0;s<imgs.length();s++) {
                                if(s==0){
                                    screenshotsText+="\""+imgs.at(s).fileName()+"\"";
                                }else{
                                    screenshotsText+=", \""+imgs.at(s).fileName()+"\"";
                                }
                            }

                            phoneScreenShots ="\"phoneScreenshots\": ["+screenshotsText+"], ";
                        }

                    }
                    QString whatsNew=", \"whatsNew\": \"* Categoria(s): "+categories+" \\n* Peso: "+(QString::number(qFloor(v1size/1024/1024)))+" Mb\"";
                    localized+= ", \"localized\": { \"es\": { "+featureGraphic+iconLocalized+phoneScreenShots+" \"description\": \""+(desc.replace("&lt;","<")).replace("&gt;",">")+"\", \"name\": \""+name+"\",  \"summary\":\" "+summary+"\" "+whatsNew+" }, \"en-US\": { "+featureGraphic+iconLocalized+phoneScreenShots+" \"description\": \""+(desc.replace("&lt;","<")).replace("&gt;",">")+"\", \"name\": \""+name+"\",  \"summary\":\" "+summary+"\" "+whatsNew+" }}";


                    tempApp="{\"categories\": ["+tempCats+"], \"suggestedVersionName\": \""+marketvercode+"\", \"suggestedVersionCode\": \""+marketvercode+"\","+
                            "\"license\": \""+license+"\", \"name\": \""+name+"\", \"sourceCode\": \""+source+"\", \"summary\": \""+summary+"\", \"webSite\": \""+web+"\", "+
                            "\"added\": "+addedAppTS+", \"icon\": \""+icon+"\", \"packageName\": \""+id+"\", \"lastUpdated\": "+updateTS+" "+localized+"}";



                    jsonIndexV1apps+=tempApp;
                    if(i!=listaApps.length()-1){
                        jsonIndexV1apps+=", ";
                    }


                    QDomNodeList pkgs=listaApps.at(i).toElement().elementsByTagName("package");

                    QString tempPkg="\""+id+"\": [";
                    QString tempPkgUnit;
                    QString v;
                    QString vc;
                    QString apkname;
                    QString srcname;
                    QString sdkver;
                    QString tsdkver;
                    QString hash;
                    QString sig;
                    QString size;
                    QString added;
                    QString perms;
                    QString obbMainFile;
                    QString obbMainFileSha256;
                    QString obbPatchFile;
                    QString obbPatchFileSha256;


                    for (int j = 0; j < pkgs.count(); ++j) {
                        v=pkgs.at(j).toElement().elementsByTagName("version").at(0).firstChild().nodeValue();
                        vc=pkgs.at(j).toElement().elementsByTagName("versioncode").at(0).firstChild().nodeValue();
                        apkname=pkgs.at(j).toElement().elementsByTagName("apkname").at(0).firstChild().nodeValue();
                        srcname=pkgs.at(j).toElement().elementsByTagName("srcname").at(0).firstChild().nodeValue();
                        sdkver=pkgs.at(j).toElement().elementsByTagName("sdkver").at(0).firstChild().nodeValue();
                        tsdkver=pkgs.at(j).toElement().elementsByTagName("targetSdkVersion").at(0).firstChild().nodeValue();

                        QString obbText="";
                        if(pkgs.at(j).toElement().elementsByTagName("obbMainFile").count()>0){
                            obbMainFile= pkgs.at(j).toElement().elementsByTagName("obbMainFile").at(0).firstChild().nodeValue();
                            obbMainFileSha256 = pkgs.at(j).toElement().elementsByTagName("obbMainFileSha256").at(0).firstChild().nodeValue();
                            obbText+="\"obbMainFile\": \""+obbMainFile+"\", \"obbMainFileSha256\": \""+obbMainFileSha256+"\",";
                        }

                        if(pkgs.at(j).toElement().elementsByTagName("obbPatchFile").count()>0){
                            obbPatchFile = pkgs.at(j).toElement().elementsByTagName("obbPatchFile").at(0).firstChild().nodeValue();
                            obbPatchFileSha256 = pkgs.at(j).toElement().elementsByTagName("obbPatchFileSha256").at(0).firstChild().nodeValue();
                            obbText+="\"obbPatchFile\": \""+obbPatchFile+"\", \"obbPatchFileSha256\": \""+obbPatchFileSha256+"\",";
                        }




                        hash=pkgs.at(j).toElement().elementsByTagName("hash").at(0).firstChild().nodeValue();
                        sig=pkgs.at(j).toElement().elementsByTagName("sig").at(0).firstChild().nodeValue();
                        size=pkgs.at(j).toElement().elementsByTagName("size").at(0).firstChild().nodeValue();
                        added=pkgs.at(j).toElement().elementsByTagName("added").at(0).firstChild().nodeValue();
                        QDateTime addedDT= QDateTime();
                        addedDT.setDate(QDate(added.mid(0,4).toInt(),added.mid(5,2).toInt(),added.mid(8,2).toInt()));
                        QString addedTS=QString::number(addedDT.toMSecsSinceEpoch());
                        perms=pkgs.at(j).toElement().elementsByTagName("permissions").at(0).firstChild().nodeValue();
                        QStringList permisions = perms.split(",");


                        QString tempPerms="";
                        for (int p = 0; p < permisions.length(); ++p) {
                            tempPerms+="[\""+permisions.at(p)+"\", null]";
                            if(p!=permisions.length()-1){
                                tempPerms+=", ";
                            }
                        }

                        tempPkgUnit ="{\"added\": "+addedTS+", \"apkName\": \""+apkname+"\", \"hash\": \""+hash+"\", \"hashType\": \"sha256\", \"minSdkVersion\": \""+sdkver+"\", "+
                                "\"packageName\": \""+id+"\", \"sig\": \""+sig+"\", \"uses-permission\": ["+tempPerms+ "], "+
                                "\"size\": "+size+", \"srcname\": \""+srcname+"\", \"targetSdkVersion\": \""+tsdkver+"\", "+obbText+" \"versionCode\": "+vc+", \"versionName\": \""+v+"\"}";

                        tempPkg+=tempPkgUnit;
                        if(j!=pkgs.length()-1){
                            tempPkg+=", ";
                        }
                    }
                    tempPkg+="]";
                    jsonIndexV1pkgs+=tempPkg;
                    if(i!=listaApps.length()-1){
                        jsonIndexV1pkgs+=", ";
                    }


                    emit Porciento(i*100/listaApps.length());

                    //this->msleep(15+ (i==0? 150:0));

                    jsonIndexV1= jsonIndexV1repo+jsonIndexV1apps+jsonIndexV1pkgs+jsonIndexV1end;

                    //qDebug()<< jsonIndexV1;
                }
                emit Porciento(100);
                xmlM->XMLWriteJSON(this->repo, jsonFilePath,jsonIndexV1);

            }
        }
    }
}


void CJSONHilo::Cerrar()
{
    this->terminate();
}

