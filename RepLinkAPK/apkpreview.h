#ifndef APKPREVIEW
#define APKPREVIEW

#include <QString>
#include <QStringList>
#include <QPixmap>
#include <pkginfo.h>
#include <listaSE.h>
#include <QMetaType>

class ApkPreview{

private:
    QString id;
    QString name;
    QString ver;
    QString apkname;
    QString vercode;
    QPixmap icon;
    double size;

public:

    ApkPreview(){}
    ~ApkPreview(){}
    ApkPreview(QString apkname, QString id, QString name, QPixmap icon, double size, QString ver, QString vercode ){
        this->id=id;
        this->name=name;
        this->icon=icon;
        this->size=size;
        this->ver=ver;
        this->vercode=vercode;
        this->apkname=apkname;
    }

    QString ObtenerId(){
        return id;
    }
    QString ObtenerName(){
        return name;
    }
    QString ObtenerVersion(){
        return ver;
    }
    QString ObtenerVersionCode(){
        return vercode;
    }
    QPixmap ObtenerPixmap(){
        return icon;
    }
    QString ObtenerApkName(){
        return apkname;
    }
    QString ObtenerSize(){
        if(size<1024){
            return QString::number(size)+" Bytes";
        }else {
            double kilos=size/1024;
            if(kilos<1024){
                return QString::number((int)kilos)+" KB";
            }else {
                double megas=kilos/1024;
                if(megas<1024){
                    return QString::number((int)megas)+"."+QString::number((int)kilos%1024)+" MB";
                }else {
                    return QString::number((int)(megas/1024))+"."+QString::number((int)megas%1024)+" GB";
                }
            }
        }
    }

    void SetID(QString id){
        this->id=id;
    }
    void SetName(QString name){
        this->name=name;
    }
    void SetPixmap(QPixmap icon){
        this->icon=icon;
    }
    void SetSize(double size){
        this->size=size;
    }
    void SetVersion(QString ver){
        this->ver=ver;
    }
    void SetVersionCode(QString vercode){
        this->vercode=vercode;
    }
    void SetAPKName(QString apkname){
        this->apkname=apkname;
    }



    QString toString(){
        QString rec=RecomendedName();

        return "Id: "+id+"\n"
                         "Name: "+name+"\n"
                                       "Version: "+ver+"\n"
                                                       "Version Code: "+vercode+"\n"
                                                                     "Size: "+ObtenerSize()+""
                +(rec!=apkname?("\nRecomended: "+RecomendedName()):"");
    }

    QString RecomendedName()
    {
        QString xname= name+"_"+ver+"."+vercode+".apk";
        xname.replace("Á","A").replace("É","E").replace("Í","I").replace("Ó","O").replace("Ú","U");
        xname.replace("á","a").replace("é","e").replace("í","i").replace("ú","u").replace("ó","o");
        xname.replace(",","");
        xname.replace(":","");
        xname.replace("?","");
        xname.replace("¿","");
        xname.replace("!","");
        xname.replace("¡","");
        xname.replace("&","");
        xname.replace("©","");
        xname.replace("®","");
        xname.replace("~","");
        xname.replace("ñ","nn");
        xname.replace(" ","_");
        xname.replace("__","_");
        xname.replace("/","-");


        return xname;
    }


};


Q_DECLARE_METATYPE(ApkPreview);

#endif // APKPREVIEW

