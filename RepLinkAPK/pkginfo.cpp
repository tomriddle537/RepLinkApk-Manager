#include "pkginfo.h"

PkgInfo::PkgInfo()
{

}

PkgInfo::~PkgInfo()
{

}

PkgInfo::PkgInfo(const QString id, const QString version, const QString versionCode, const QString apkname, const QString sdkver, const QString size, const QString mainObb, const QString patchObb)
{
    this->id=id;
    this->version=version;
    this->versionCode=versionCode;
    this->apkname=apkname;
    this->sdkver=sdkver;
    this->size=size;
    this->obbMainFile=mainObb;
    this->obbPatchFile=patchObb;
}

QString PkgInfo::ObtenerId() const
{
    return this->id;
}

QString PkgInfo::ObtenerVersion() const
{
    return this->version;
}

QString PkgInfo::ObtenerVersionCode() const
{
    return this->versionCode;
}

QString PkgInfo::ObtenerSize() const
{
    double xsize=size.toDouble();
    if(xsize<1024){
        return QString::number(xsize)+" Bytes";
    }else {
        double kilos=xsize/1024;
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

QString PkgInfo::ObtenerApkName() const
{
    return this->apkname;
}

QString PkgInfo::ObtenerSDKVer() const
{
    return this->sdkver;
}

QString PkgInfo::ObtenerMainObb() const
{
    return this->obbMainFile;
}

QString PkgInfo::ObtenerPatchObb() const
{
    return this->obbPatchFile;
}

void PkgInfo::SetApkName(const QString apkname)
{
    this->apkname=apkname;
}

void PkgInfo::SetMainObb(const QString mainObb)
{
    this->obbMainFile=mainObb;
}

void PkgInfo::SetPatchObb(const QString patchObb)
{
    this->obbPatchFile=patchObb;
}

QString PkgInfo::toString()
{
    QString android=AndroidVersion(sdkver, true);
    QString mainObbTxt=obbMainFile!=""?"Main Obb: "+obbMainFile+"\n":"";
    QString patchObbTxt=obbPatchFile!=""?"Patch Obb: "+obbPatchFile+"\n":"";
    return "Version: "+version+"\n"
                               "Version Code: "+versionCode+"\n"
                                                            "Min Android: "+android+"\n"
                                                                                    "APK Name: "+apkname+"\n" + mainObbTxt + patchObbTxt +
                                                                                                         "Size: "+ObtenerSize();
}

QString PkgInfo::AndroidVersion(QString sdk, bool large)
{
    int num=sdk.toInt();

    return DatAccess().SelectAndroidVerFromSdk(num,large);

}



