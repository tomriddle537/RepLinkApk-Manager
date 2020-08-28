#ifndef CXMLPACKAGE
#define CXMLPACKAGE
#include <QString>
#include <QStringList>
#include <QDomDocument>
#include <QDomText>


class CXMLPackage {

private:
    QString version;
    QString versioncode;
    QString apkname;
    QString srcname;
    QString hash;
    QString sig;
    QString size;
    QString sdkver;
    QString targetSdkVer;
    QString addedpkg;
    QString obbMainFile;
    QString obbMainFileSha256;
    QString obbPatchFile;
    QString obbPatchFileSha256;
    QStringList permissions;
    QStringList features;
    QDomDocument domDocument;

public:

    CXMLPackage(){
        this->version="";
        this->versioncode="";
        this->apkname="";
        this->srcname="";
        this->hash="";
        this->sig="";
        this->size="";
        this->sdkver="";
        this->targetSdkVer="";
        this->addedpkg="";
        this->obbMainFile="";
        this->obbMainFileSha256="";
        this->obbPatchFile="";
        this->obbPatchFileSha256="";
    }


    CXMLPackage(QString version, QString versioncode, QString apkname, QString hash, QString size,
                QString sdkver, QString targetSdkVer, QString addedpkg, QStringList permissions,
                QString srcname="",   QStringList features=QStringList(), QString sig="",
                QString obbMainFile="", QString obbMainFileSha256="", QString obbPatchFile="", QString obbPatchFileSha256=""){
        this->version=version;
        this->versioncode=versioncode;
        this->apkname=apkname;
        this->srcname=srcname;
        this->hash=hash;
        this->sig=sig;
        this->size=size;
        this->sdkver=sdkver;
        this->targetSdkVer=targetSdkVer;
        this->addedpkg=addedpkg;
        this->obbMainFile=obbMainFile;
        this->obbMainFileSha256=obbMainFileSha256;
        this->obbPatchFile=obbPatchFile;
        this->obbPatchFileSha256=obbPatchFileSha256;
        this->permissions=permissions;
        this->features=features;
    }

    QString ObtenerVersion()
    {
        return this->version;
    }

    QString ObtenerVersionCode()
    {
        return this->versioncode;
    }

    QString ObtenerApkName()
    {
        return this->apkname;
    }

    QString ObtenerSRCName()
    {
        return this->srcname;
    }

    QString ObtenerHash()
    {
        return this->hash;
    }

    QString ObtenerSig()
    {
        return this->sig;
    }

    QString ObtenerSize()
    {
        return this->size;
    }

    QString ObtenerSDKVer()
    {
        return this->sdkver;
    }

    QString ObtenerTargetSdkVer()
    {
        return this->targetSdkVer;
    }

    QString ObtenerAddedDate()
    {
        return this->addedpkg;
    }

    QStringList ObtenerPermisions()
    {
        return this->permissions;
    }

    QStringList ObtenerFeatures()
    {
        return this->features;
    }

    QString ObtenerMainObb()
    {
        return this->obbMainFile;
    }

    QString ObtenerMainObbSHA()
    {
        return this->obbMainFileSha256;
    }

    QString ObtenerPatchObb()
    {
        return this->obbPatchFile;
    }

    QString ObtenerPatchObbSHA()
    {
        return this->obbPatchFileSha256;
    }

    QDomElement toXML(){
        QDomText text;

        QDomElement xversion=domDocument.createElement("version");
        text=domDocument.createTextNode(ObtenerVersion());
        xversion.appendChild(text);
        QDomElement xversioncode=domDocument.createElement("versioncode");
        text=domDocument.createTextNode(ObtenerVersionCode());
        xversioncode.appendChild(text);
        QDomElement xapkname=domDocument.createElement("apkname");
        text=domDocument.createTextNode(ObtenerApkName());
        xapkname.appendChild(text);
        QDomElement xsrcname=domDocument.createElement("srcname");
        text=domDocument.createTextNode(ObtenerSRCName());
        xsrcname.appendChild(text);
        QDomElement xhash=domDocument.createElement("hash");
        text=domDocument.createTextNode(ObtenerHash());
        xhash.appendChild(text);
        xhash.setAttribute("type", "sha256");
        QDomElement xsig=domDocument.createElement("sig");
        text=domDocument.createTextNode(ObtenerSig());
        xsig.appendChild(text);
        QDomElement xsize=domDocument.createElement("size");
        text=domDocument.createTextNode(ObtenerSize());
        xsize.appendChild(text);
        QDomElement xsdkver=domDocument.createElement("sdkver");
        text=domDocument.createTextNode(ObtenerSDKVer());
        xsdkver.appendChild(text);
        QDomElement xtargetSdkVer=domDocument.createElement("targetSdkVersion");
        text=domDocument.createTextNode(ObtenerTargetSdkVer());
        xtargetSdkVer.appendChild(text);

        QDomElement xaddedpkg=domDocument.createElement("added");
        text=domDocument.createTextNode(ObtenerAddedDate());
        xaddedpkg.appendChild(text);

        QDomElement xmainobb=domDocument.createElement("obbMainFile");
        QDomElement xmainobbsha=domDocument.createElement("obbMainFileSha256");
        if(this->obbMainFile!=""){

            text=domDocument.createTextNode(ObtenerMainObb());
            xmainobb.appendChild(text);

            text=domDocument.createTextNode(ObtenerMainObbSHA());
            xmainobbsha.appendChild(text);
        }
        QDomElement xpatchobb=domDocument.createElement("obbPatchFile");
        QDomElement xpatchobbsha=domDocument.createElement("obbPatchFileSha256");
        if(this->obbPatchFile!=""){

            text=domDocument.createTextNode(ObtenerPatchObb());
            xpatchobb.appendChild(text);

            text=domDocument.createTextNode(ObtenerPatchObbSHA());
            xpatchobbsha.appendChild(text);
        }

        QDomElement xpermissions=domDocument.createElement("permissions");
        QString perms="";
        for (int i = 0; i < ObtenerPermisions().length(); ++i) {
              perms+=ObtenerPermisions().at(i);
              if(i != ObtenerPermisions().length()-1){
                  perms+=",";
              }
        }

        //QDomElement xfeatures=domDocument.createElement("features");
        QString feat=" ";
        for (int i = 0; i < ObtenerFeatures().length(); ++i) {
            if(ObtenerFeatures().at(i)=="android.hardware.screen.portrait"){
                feat+=ObtenerFeatures().at(i);
                //if(i != ObtenerFeatures().length()-1){
                feat+=",";
                //}
            }
        }
        //text=domDocument.createTextNode(feat);
        //xfeatures.appendChild(text);

        if(feat!=" "){
            perms=feat+""+perms;
        }
        text=domDocument.createTextNode(perms);
        xpermissions.appendChild(text);

        QDomElement xpackage=domDocument.createElement("package");
        xpackage.appendChild(xversion);
        xpackage.appendChild(xversioncode);
        xpackage.appendChild(xapkname);
        xpackage.appendChild(xsrcname);
        xpackage.appendChild(xhash);
        xpackage.appendChild(xsig);
        xpackage.appendChild(xsize);
        xpackage.appendChild(xsdkver);
        xpackage.appendChild(xtargetSdkVer);
        if(this->obbMainFile!=""){
            xpackage.appendChild(xmainobb);
            xpackage.appendChild(xmainobbsha);
        }

        if(this->obbPatchFile!=""){
            xpackage.appendChild(xpatchobb);
            xpackage.appendChild(xpatchobbsha);
        }

        xpackage.appendChild(xaddedpkg);
        xpackage.appendChild(xpermissions);
        //xpackage.appendChild(xfeatures);



        return xpackage;


    }



};

#endif // CXMLPACKAGE

