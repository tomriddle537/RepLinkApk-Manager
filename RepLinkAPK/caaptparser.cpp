#include "caaptparser.h"


CBinOutputParser::CBinOutputParser(QObject *parent): QObject(parent)
{
    Q_UNUSED(parent);
}


CXMLAplication CBinOutputParser::binOutputParser(QString repository, QStringList lines, QString url, QFileInfo infoFile)
{


    //App
    QString id;
    QString added=infoFile.created().toString("yyyy-MM-dd");
    QString lastudated=added;
    QString name;
    QString summary="Unknown";
    QString icon;
    QString icondir;
    QString desc="Unknown";
    QString licence="Free";
    QString categorYies="Unknown";
    QString web=" ";
    QString source=" ";
    QString tracker=" ";
    QString marketversion;
    QString marketvercode;
    QString requierements;
    ListaSE<CXMLPackage> packages;

    //Package
    QString version="1.0";
    QString versioncode;

    QString apkname;
    QStringList partes = url.split( "/" );
    apkname = partes.last();

    QString srcname=" ";

    QCryptographicHash::Algorithm hashAlgorithm=QCryptographicHash::Sha256;
    QString hash=" ";

    QByteArray checksum=fileChecksum(url,hashAlgorithm);
    hash=checksum.toHex();

    QString sig=" ";
    QString size=QString::number(infoFile.size());
    QString sdkver;
    QString targetSDKver;
    QString addedpkg=added;
    QStringList permissions;
    QStringList features;

    CXMLPackage pack;
    CXMLAplication app;

    QString result="";
    QStringList parsedLine;
    QStringList subParsedLine;
    QStringList aux;
    QStringList densities;
    QStringList imgsDensities;
    for (int i = 0; i < lines.length(); ++i) {
        parsedLine=lines.at(i).split(":");

        if(parsedLine.length()==3){
            parsedLine=lines.at(i).split(":'");
            if(parsedLine.length()==2){
                QString auxPart=parsedLine.at(1);
                auxPart.insert(0,"'").replace(":","");

                parsedLine.replace(1,auxPart);
            }else{
                parsedLine=lines.at(i).split(": label");
                if(parsedLine.length()==2){
                    QString auxPart=parsedLine.at(1);
                    auxPart.insert(0,"'").replace(":","");
                    parsedLine.replace(1,auxPart);
                }
            }
        }


        if(parsedLine.length()==2){

            if(isIn(parsedLine.at(0))){
                if(parsedLine.at(0)=="sdkVersion"){
                    sdkver=parsedLine.at(1).trimmed().split("'").at(1);
                }else if(parsedLine.at(0)=="targetSdkVersion"){
                    targetSDKver=parsedLine.at(1).trimmed().split("'").at(1);
                } else if((parsedLine.at(0)=="application-label" ) || parsedLine.at(0)=="application-label-en"){
                    name=parsedLine.at(1).trimmed().split("'").at(1);

                    name.replace("Á","A").replace("É","E").replace("Í","I").replace("Ó","O").replace("Ú","U");
                    name.replace("á","a").replace("é","e").replace("í","i").replace("ú","u").replace("ó","o");
                    name.replace(",","");
                    name.replace(":","");
                    name.replace("?","");
                    name.replace("¿","");
                    name.replace("!","");
                    name.replace("¡","");
                    name.replace("&","");
                    name.replace("©","");
                    name.replace("®","");
                    name.replace("~","");
                    name.replace("ñ","n");
                    name.replace("__","_");
                    name.replace("/","-");


                } else if(parsedLine.at(0)=="application"){
                    subParsedLine=parsedLine.at(1).trimmed().split(" ");
                    for (int j = 0; j < subParsedLine.length(); ++j) {
                        aux=subParsedLine.at(j).split("=");
                        if(aux.at(0)=="icon"){
                            icondir=aux.at(1).trimmed().split("'").at(1);
                        }
                    }
                } else if(parsedLine.at(0)=="package"){
                    subParsedLine=parsedLine.at(1).trimmed().split(" ");
                    for (int j = 0; j < subParsedLine.length(); ++j) {
                        aux=subParsedLine.at(j).split("=");
                        if(aux.at(0)=="name"){
                            id=aux.at(1).trimmed().split("'").at(1);
                        } else if(aux.at(0)=="versionName"){
                            version=aux.at(1).trimmed().split("'").at(1);
                            marketversion=version;
                        } else if(aux.at(0)=="versionCode"){
                            versioncode=aux.at(1).trimmed().split("'").at(1);
                            marketvercode=versioncode;
                        }
                    }
                }else if(parsedLine.at(0)=="densities"){
                    aux=parsedLine.at(1).trimmed().split(" ");
                    for (int k = 0; k < aux.length(); ++k) {
                        densities.append(aux.at(k).trimmed().split("'").at(1));
                    }

                } else if(parsedLine.at(0)=="uses-permission"){
                    if(QString(parsedLine.at(1)).indexOf("HANDOVER_STATUS")==-1 &&  QString(parsedLine.at(1)).indexOf("PROVIDE_BACKGROUND")==-1 && QString(parsedLine.at(1)).indexOf("C2D_MESSAGE")==-1 ){
                        QString tempPerm=QString(parsedLine.at(1)).replace("android.permission.","");
                        permissions.append(tempPerm.trimmed().split("'").at(1));
                    }else{
                        permissions.append(parsedLine.at(1).trimmed().split("'").at(1));
                    }
                    result+=parsedLine.at(0)+":"+parsedLine.at(1).trimmed()+"\r\n";
                }else if(parsedLine.at(0)=="uses-feature"){
                    if(parsedLine.at(1).trimmed().split("'").at(1)!="android.hardware.camera.autofocus"){
                        features.append(parsedLine.at(1).trimmed().split("'").at(1));
                        result+=parsedLine.at(0)+":"+parsedLine.at(1).trimmed()+"\r\n";
                    }
                }
            }



        }
    }

    for (int i = 0; i < densities.length(); ++i) {
        for (int j = 0; j <lines.length(); ++j) {
            parsedLine=lines.at(j).split(":");
            if(parsedLine.length()==2){
                if(parsedLine.at(0)=="application-icon-"+densities.at(i)){
                    imgsDensities.append(parsedLine.at(1).trimmed().split("'").at(1));
                    result+="icons-"+densities.at(i)+":"+imgsDensities.at(imgsDensities.length()-1)+"\r\n";
                }
            }
        }
    }

    if(!id.isEmpty()){
        sig=parseGetSig(infoFile);

        if(version.trimmed()==""){
            version="1.0";
        }


        QString ext = "png";
        if(icondir!=""){
           ext= icondir.split(".").last();
        }
        icon=id+"."+ext;


        pack=CXMLPackage(version,versioncode,apkname,hash,size,sdkver,targetSDKver,addedpkg,permissions,srcname,features,sig);
        packages.AdicionarCola(pack);
        app=CXMLAplication(id,added,lastudated,name,summary,icon,desc,categorYies,marketversion,marketvercode,packages, requierements,licence,web,source,tracker);
        int v=CXMLMethods().GetAppLatestVersion(repository+"/replink.xml",id);
        iconProcess(repository, url, id,icondir,densities,imgsDensities, v <= versioncode.toInt() && v != -1);
    }else{
        //std::cout<< "Corrupt App or Not well renamed "<< url.toStdString() << std::endl;
    }

    return app;
}

AppInfo CBinOutputParser::binOutputParser(QStringList lines, QString url, QFileInfo infoFile)
{
    ListaSE<PkgInfo> packages;
    //App
    QString id;
    QString name;
    QString summary="Unknown";
    QString icon;
    QString icondir;
    QString desc="Unknown";
    QString licence="Free";
    QString category="Unknown";
    QString categories=category;
    QString web=" ";
    QString marketversion;
    QString marketvercode;

    //Package
    QString version;
    QString versioncode;

    QString apkname;
    QStringList partes = url.split( "/" );
    apkname = partes.value(partes.length()-1);

    double size=infoFile.size();
    QString sdkver;
    QString targetSDKver;
    QStringList permissions;
    QStringList features;

    AppInfo app;

    QString result="";
    QStringList parsedLine;
    QStringList subParsedLine;
    QStringList aux;
    QStringList densities;
    QStringList screens;
    QStringList imgsDensities;

    for (int i = 0; i < lines.length(); ++i) {
        parsedLine=lines.at(i).split(":");


        if(parsedLine.length()==3){
            parsedLine=lines.at(i).split(":'");
            if(parsedLine.length()==2){
                QString auxPart=parsedLine.at(1);
                auxPart.insert(0,"'").replace(":","");

                parsedLine.replace(1,auxPart);
            }else{
                parsedLine=lines.at(i).split(": label");
                if(parsedLine.length()==2){
                    QString auxPart=parsedLine.at(1);
                    auxPart.insert(0,"'").replace(":","");
                    parsedLine.replace(1,auxPart);
                }
            }

        }

        if(parsedLine.length()==2){


            if(isIn(parsedLine.at(0))){
                if(parsedLine.at(0)=="sdkVersion"){
                    sdkver=parsedLine.at(1).trimmed().split("'").at(1);
                }else if(parsedLine.at(0)=="targetSdkVersion"){
                    targetSDKver=parsedLine.at(1).trimmed().split("'").at(1);
                } else if((parsedLine.at(0)=="application-label" ) || parsedLine.at(0)=="application-label-en-GB"){
                    name=parsedLine.at(1).trimmed().split("'").at(1);

                } else if(parsedLine.at(0)=="application"){
                    subParsedLine=parsedLine.at(1).trimmed().split(" ");
                    for (int j = 0; j < subParsedLine.length(); ++j) {
                        aux=subParsedLine.at(j).split("=");
                        if(aux.at(0)=="icon"){
                            icondir=aux.at(1).trimmed().split("'").at(1);
                        }
                    }
                } else if(parsedLine.at(0)=="package"){
                    subParsedLine=parsedLine.at(1).trimmed().split(" ");
                    for (int j = 0; j < subParsedLine.length(); ++j) {
                        aux=subParsedLine.at(j).split("=");
                        if(aux.at(0)=="name"){
                            id=aux.at(1).trimmed().split("'").at(1);
                            icon=id+".png";
                        } else if(aux.at(0)=="versionName"){
                            version=aux.at(1).trimmed().split("'").at(1);
                            marketversion=version;
                        } else if(aux.at(0)=="versionCode"){
                            versioncode=aux.at(1).trimmed().split("'").at(1);
                            marketvercode=versioncode;
                        }
                    }
                }else if(parsedLine.at(0)=="densities"){
                    aux=parsedLine.at(1).trimmed().split(" ");
                    for (int k = 0; k < aux.length(); ++k) {
                        densities.append(aux.at(k).trimmed().split("'").at(1));
                    }

                } else if(parsedLine.at(0)=="supports-screens"){
                    aux=parsedLine.at(1).trimmed().split(" ");
                    for (int k = 0; k < aux.length(); ++k) {
                        screens.append(aux.at(k).trimmed().split("'").at(1));
                    }

                } else if(parsedLine.at(0)=="uses-permission"){
                    if(QString(parsedLine.at(1)).indexOf("HANDOVER_STATUS")==-1 &&  QString(parsedLine.at(1)).indexOf("PROVIDE_BACKGROUND")==-1 && QString(parsedLine.at(1)).indexOf("C2D_MESSAGE")==-1 ){
                        QString tempPerm=QString(parsedLine.at(1)).replace("android.permission.","");
                        permissions.append(tempPerm.trimmed().split("'").at(1));
                    }else{
                        permissions.append(parsedLine.at(1).trimmed().split("'").at(1));
                    }
                    result+=parsedLine.at(0)+":"+parsedLine.at(1).trimmed()+"\r\n";
                }else if(parsedLine.at(0)=="uses-feature"){
                    features.append(parsedLine.at(1).trimmed().split("'").at(1));
                    result+=parsedLine.at(0)+":"+parsedLine.at(1).trimmed()+"\r\n";
                }
            }



        }
    }

	
    for (int i = 0; i < densities.length(); ++i) {
        for (int j = 0; j <lines.length(); ++j) {
            parsedLine=lines.at(j).split(":");
            if(parsedLine.length()==2){
                if(parsedLine.at(0)=="application-icon-"+densities.at(i)){
                    imgsDensities.append(parsedLine.at(1).trimmed().split("'").at(1));
                    result+="icons-"+densities.at(i)+":"+imgsDensities.at(imgsDensities.length()-1)+"\r\n";
                }
            }
        }
    }

    if(!id.isEmpty()){

        if(version.trimmed()==""){
            version="1.0";
        }

        packages.AdicionarCola(PkgInfo());
        app=AppInfo(id,name,summary,desc,category,categories,licence,web,version,packages, "",versioncode, sdkver,targetSDKver);
        app.SetDensities(densities);
        app.SetFeatures(features);
        app.SetPermissions(permissions);
        app.SetScreens(screens);
        app.SetSize(size);
        extractIcon(id+versioncode, url, icondir);
    }else{
        //std::cout<< "Corrupt App or Not well renamed "<< url.toStdString() << std::endl;
    }

    return app;
}

ApkPreview CBinOutputParser::binOutputParser(QStringList lines, QString url)
{
    //App
    QString id;
    QString name;
    QString ver="1.0";
    QString vercode;
    QPixmap icon;
    double size=QFileInfo(url).size();

    ApkPreview app;

    QString icondir;
    QStringList parsedLine;
    QStringList subParsedLine;
    QStringList aux;



    for (int i = 0; i < lines.length(); ++i) {
        parsedLine=lines.at(i).split(":");
        if(parsedLine.length()==3){
            parsedLine=lines.at(i).split(":'");
            if(parsedLine.length()==2){
                QString auxPart=parsedLine.at(1);
                auxPart.insert(0,"'").replace(":","");
                parsedLine.replace(1,auxPart);
            }else{
                parsedLine=lines.at(i).split(": label");
                if(parsedLine.length()==2){
                    QString auxPart=parsedLine.at(1);
                    auxPart.insert(0,"'").replace(":","");
                    parsedLine.replace(1,auxPart);
                }
            }
        }
        if(parsedLine.length()==2){
            if(isIn(parsedLine.at(0))){
                if((parsedLine.at(0)=="application-label" ) || parsedLine.at(0)=="application-label-en-GB"){
                    name=parsedLine.at(1).trimmed().split("'").at(1);
                } else if(parsedLine.at(0)=="application"){
                    subParsedLine=parsedLine.at(1).trimmed().split(" ");
                    for (int j = 0; j < subParsedLine.length(); ++j) {
                        aux=subParsedLine.at(j).split("=");
                        if(aux.at(0)=="icon"){
                            icondir=aux.at(1).trimmed().split("'").at(1);
                            break;
                        }
                    }
                    break;
                } else if(parsedLine.at(0)=="package"){
                    subParsedLine=parsedLine.at(1).trimmed().split(" ");
                    for (int j = 0; j < subParsedLine.length(); ++j) {
                        aux=subParsedLine.at(j).split("=");
                        if(aux.at(0)=="name"){
                            id=aux.at(1).trimmed().split("'").at(1);
                        }else if(aux.at(0)=="versionName"){
                            ver=aux.at(1).trimmed().split("'").at(1);
                            break;
                        } else if(aux.at(0)=="versionCode"){
                            vercode=aux.at(1).trimmed().split("'").at(1);
                        }
                    }
                }
            }
        }
    }

    if(!id.isEmpty()){
        icon=extractIcon(id+vercode, url, icondir);

        if(ver.trimmed()==""){
            ver="1.0";
        }
        app=ApkPreview(url.split("/").last(), id,name,icon,size, ver, vercode);

    }else{
        //std::cout<< "Corrupt App or Not well renamed "<< url.toStdString() << std::endl;
    }

    return app;
}

KeystoreInfo CBinOutputParser::binOutputParser()
{
    QString commonName;
    QString orgUnit;
    QString org;
    QString locality;
    QString state;
    QString country;

    KeystoreInfo kInfo;

    QStringList parsedLine;
    QStringList subParsedLine;
    QStringList aux;

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
    QString result="";


    if(QFile(settings.value("AppDirs/java").toString().trimmed()+"/keytool.exe").exists()){
        QFileInfo ks(keystore);
        if(ks.exists()){
            QString command= javaDir+"keytool.exe\" -list -v -alias "+key+" -keystore \""+ks.absoluteFilePath()+"\" -storepass "+storepass;
            cmd.start(command.replace("/","\\") );
            cmd.waitForFinished(-1);
            QByteArray respose=cmd.readAllStandardOutput();
            result=QString().fromStdString(respose.toStdString());

            QStringList lines=result.split("\n");

            for (int i = 0; i < lines.length(); ++i) {
                parsedLine=lines.at(i).split(":");
                if(parsedLine.length()==2){
                    if(parsedLine.at(0).indexOf("Prop")!=-1){
                        subParsedLine=QString(parsedLine.at(1)).replace(" ","").split(",");
                        for (int j = 0; j < subParsedLine.length(); ++j) {
                            aux=subParsedLine.at(j).split("=");
                            if(aux.at(0)=="CN"){
                                commonName=aux.at(1);
                            }else if(aux.at(0)=="OU"){
                                orgUnit=aux.at(1);
                            }else if(aux.at(0)=="O"){
                                org=aux.at(1);
                            }else if(aux.at(0)=="L"){
                                locality=aux.at(1);
                            }else if(aux.at(0)=="ST"){
                                state=aux.at(1);
                            }else if(aux.at(0)=="C"){
                                country=aux.at(1);
                            }
                        }
                        break;
                    }
                }
            }

            if(!commonName.isEmpty()){
                kInfo=KeystoreInfo(commonName,orgUnit,org,locality,state,country);
            }
        }else{
            error+="Error: Key Store not found! ";
            emit Notice(error,1);
        }


    }else{
        emit Notice("Error: Instale JDK v1.8.0",1);
    }



    return kInfo;
}


bool CBinOutputParser::isIn(QString property)
{
    QStringList list;
    list << "Result" << "sdkVersion"<< "uses-feature" << "targetSdkVersion" << "uses-permission" << "supports-screens" << "densities"<<"application-label"<<"application-label-en-GB"<<"application"<<"package";
    return list.indexOf(property)!=-1;
}

QByteArray CBinOutputParser::fileChecksum(const QString &fileName, QCryptographicHash::Algorithm hashAlgorithm)
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

QString CBinOutputParser::parseGetSig(QFileInfo infoFile)
{
    QProcess cmd;

    QString result="";

    QString javaDir="\"";

    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       "RepLink APK Manager", "RepLinkAPK");
    QString appPath=settings.value("AppDirs/path").toString()+"/bins/";

    javaDir+=settings.value("AppDirs/java").toString()+"/";

    if(QFile(settings.value("AppDirs/java").toString().trimmed()+"/java.exe").exists()){

        QString command=javaDir+"java.exe\" -jar \""+appPath+"GetSig.jar\" \""+infoFile.filePath()+"\"";
        cmd.start(command.replace("/","\\") );
        cmd.waitForFinished(-1);
        QByteArray respose=cmd.readAllStandardOutput();
        result=QString().fromStdString(respose.toStdString());

        QStringList lines=result.split("\r\n");

        result=" ";
        QStringList parsedLine;

        for (int i = 0; i < lines.length(); ++i) {
            parsedLine=lines.at(i).split(":");

            if(parsedLine.length()==2){
                if(isIn(parsedLine.at(0))){
                    if(parsedLine.at(0)=="Result"){
                        result=parsedLine.at(1).trimmed();
                        break;
                    }
                }
            }
        }
    }else{
        emit Notice("Error: Instale JDK v1.8.0",1);
    }

    return result;
}




QPixmap CBinOutputParser::extractIcon(QString id, QString url, QString &rutaicon)
{
    QPixmap pixmap;
    pixmap.load(":/img/apkfile.png");
    QString ext=rutaicon.split(".").last();

    QString md5= CSecurity().getMD5(id);
    QString temp=QDir::tempPath()+"\\APKIcons\\"+md5+"."+ext;
    QFile prev(temp);
    if(prev.exists()){
        pixmap.load(temp);
    }else{
        if(!rutaicon.isEmpty()){
            QString pngname;
            QStringList partes = rutaicon.split( "/" );
            pngname = partes.last();

            QString appDir="\"\"";

            QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                               "RepLink APK Manager", "RepLinkAPK");

            appDir+=settings.value("AppDirs/path").toString()+"/";

            QProcess cmd;
            QString command;
            command=appDir+"bins/unzip.exe\" -j -o \""+url+"\" \""+rutaicon+"\" -d \"%temp%\\APKIcons\"\"";

            cmd.start("cmd.exe /c " +command.replace("/","\\"));
            cmd.waitForFinished(-1);

            //qDebug()<< "cmd.exe /c "+command.replace("/","\\");
            QString temp2=QDir::tempPath()+"\\APKIcons\\"+pngname;
            QFile file(temp2);
            if(file.exists()){
                if(file.copy(temp)){
                    pixmap.load(temp);
                    file.remove();
                }else{
                    pixmap.load(temp2);
                }
            }
        }
    }
    emit ImgActual(pixmap);
    return pixmap;
}

void CBinOutputParser::iconProcess(QString repository, QString url, QString pkg, QString rutaicon, QStringList densities, QStringList imgDens, bool rewrite)
{

    if(!rutaicon.isEmpty()){
        QFileInfo fInfo;

        QString pngname;
        QStringList partes = rutaicon.split( "/" );
        pngname = partes.last();

        QProcess cmd;
        QString command;
        QString appDir="\"\"";
        QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                           "RepLink APK Manager", "RepLinkAPK");

        appDir+=settings.value("AppDirs/path").toString()+"/";

        QString ext=rutaicon.split(".").last();

        for (int i = 0; i < densities.length(); ++i) {
            QDir d(repository +"/icons-"+densities.at(i));
            if(!d.exists()){
                d.mkdir(repository +"/icons-"+densities.at(i));
            }

            fInfo=QFileInfo(repository +"/icons-"+densities.at(i)+"/"+pkg+"."+ext);

            if(!fInfo.exists() || rewrite){
                command=appDir+"bins/unzip.exe\" -j -o \""+url+"\" \""+imgDens.at(i)+"\" -d \"%temp%/APKIcons\"\"";
                cmd.start("cmd.exe /c "+command);
                cmd.waitForFinished(-1);

                QFile f(repository +"/icons-"+densities.at(i)+"/"+pkg+"."+ext);
                if(f.exists()){
                    f.remove();
                }
                f.copy(QDir::toNativeSeparators(QDir::tempPath()+"/APKIcons/"+pngname), QDir::toNativeSeparators(repository +"/icons-"+densities.at(i)+"/"+pkg+"."+ext));

                if(densities.at(i) == densities.last()){

                    QDir d(repository +"/icons");
                    if(!d.exists()){
                        d.mkdir(repository +"/icons");
                    }

                    QDir i(repository +"/"+pkg);
                    if(!i.exists()){
                        i.mkdir(repository +"/"+pkg);
                        qDebug()<< repository +"/"+pkg;
                        i.mkdir(repository +"/"+pkg+"/es");
                        i.mkdir(repository +"/"+pkg+"/es/phoneScreenshots");
                    }

                    QFile f(repository +"/icons/"+pkg+"."+ext);
                    if(f.exists()){
                        f.remove();
                    }

                    f.copy(QDir::toNativeSeparators(QDir::tempPath()+"/APKIcons/"+pngname), QDir::toNativeSeparators(repository +"/icons/"+pkg+"."+ext));
                    f.copy(QDir::toNativeSeparators(QDir::tempPath()+"/APKIcons/"+pngname), QDir::toNativeSeparators(repository +"/"+pkg+"/es/icon."+ext));

                }
            }
        }

        QString imgRuta=repository +"/icons/"+pkg+"."+ext;
        fInfo=QFileInfo(imgRuta);
        QPixmap pixmap;


        if(!fInfo.exists()){
            pixmap.load(":/img/apkfile.png");
        }else{
            pixmap.load(imgRuta);
        }


        emit ImgActual(pixmap);
    }
}


