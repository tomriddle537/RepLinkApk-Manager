#include "appinfo.h"

AppInfo::AppInfo(){

}

AppInfo::AppInfo(const QString id, const QString name, const QString summary, const QString desc,
                 const QString category, const QString categories, const QString license, const QString source, const QString version, const ListaSE<PkgInfo> packages,const QString requirements, const QString versionCode, const QString sdkVer, const QString targetSdkVer ){
    this->id=id;
    this->name=name;
    this->summary=summary;
    this->desc=desc;
    this->license=license;
    this->category=category;
    this->categories=categories;
    this->source=source;
    this->version=version;
    this->packages=packages;
    this->sdkVer=sdkVer;
    this->targetSdkVer=targetSdkVer;
    this->versionCode=versionCode;
    this->requirements=requirements;
    this->versionCodeObb="";
    this->obbMainFile="";
    this->obbPatchFile="";
}

QString AppInfo::ObtenerId() const
{
    return this->id;
}


QString AppInfo::ObtenerName() const
{
    return this->name;
}

QString AppInfo::ObtenerSummary() const
{
    return this->summary;
}


QString AppInfo::ObtenerDescription() const
{
    return this->desc;
}

QString AppInfo::ObtenerLicense() const
{
    return this->license;
}

QString AppInfo::ObtenerCategory() const
{
    return this->category;
}

QString AppInfo::ObtenerCategories() const
{
    return this->categories;
}

QString AppInfo::ObtenerWeb() const
{
    return this->web;
}

QString AppInfo::ObtenerLatestVersion() const
{
    return this->version;
}

QString AppInfo::ObtenerVersionCode() const
{
    return this->versionCode;
}

QString AppInfo::ObtenerSdkVersion() const
{
    return this->sdkVer;
}

QString AppInfo::ObtenerTargetSdkVersion() const
{
    return this->targetSdkVer;
}

QString AppInfo::ObtenerNoPackages()
{
    int cant=this->packages.Longitud();
    return QString::number(cant);
}

QString AppInfo::ObtenerRequirements()
{
    return requirements;
}

ListaSE<PkgInfo> AppInfo::ObtenerPackages()
{
    return packages;
}

QString AppInfo::ObtenerSize() const
{
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

QString AppInfo::ObtenerSource() const
{
    return this->source;
}

QStringList AppInfo::ObtenerFeatures() const
{
    return this->features;
}

QStringList AppInfo::ObtenerPermisions() const
{
    return this->permissions;
}

QStringList AppInfo::ObtenerDensities() const
{
    return this->densities;
}

QStringList AppInfo::ObtenerScreens() const
{
    return this->screens;
}

QString AppInfo::ObtenerVersionCodeToObb()
{
    return this->versionCodeObb;
}

QString AppInfo::ObtenerMainObb()
{
    return this->obbMainFile;
}

QString AppInfo::ObtenerPatchObb()
{
    return this->obbPatchFile;
}

void AppInfo::SetPackages(ListaSE<PkgInfo> packages)
{
    this->packages=packages;
}


void AppInfo::SetName(const QString name)
{
    this->name=name;
}

void AppInfo::SetDescription(const QString desc)
{
    this->desc=desc;
}

void AppInfo::SetSummary(const QString summary)
{
    this->summary=summary;
}

void AppInfo::SetCategory(const QString category)
{
    this->category=category;
}

void AppInfo::SetCategories(const QString categories)
{
    this->categories=categories;
}

void AppInfo::SetWeb(const QString web)
{
    this->web=web;
}

void AppInfo::SetRequirements(const QString requirements)
{
    this->requirements=requirements;
}

void AppInfo::SetSize(const double size)
{
    this->size=size;
}

void AppInfo::SetLicense(const QString license)
{
    this->license=license;
}

void AppInfo::SetSource(const QString source)
{
    this->source=source;
}

void AppInfo::SetFeatures(const QStringList features)
{
    this->features=features;
}

void AppInfo::SetPermissions(const QStringList permissions)
{
    this->permissions=permissions;
}

void AppInfo::SetDensities(const QStringList densities)
{
    this->densities=densities;
}

void AppInfo::SetScreens(const QStringList screens)
{
    this->screens=screens;
}

void AppInfo::SetVersionCodeToObb(QString versionCodeObb)
{
    this->versionCodeObb = versionCodeObb;
}

void AppInfo::SetMainObb(QString mainObb)
{
    this->obbMainFile=mainObb;
}

void AppInfo::SetPatchObb(QString patchObb)
{
    this->obbPatchFile=patchObb;
}



QString AppInfo::toString(){

    int cant=packages.Longitud();
    QString android=cant>0?packages.ObtenerPorPos(1).AndroidVersion(packages.ObtenerPorPos(1).ObtenerSDKVer(),false):"None";
    QString s=source.trimmed().isEmpty()?"":"\nSource: "+source;

    QString latest= cant>0?("Latest Version: "+version+"\nLatest Min Android: "+android+"\n"):"";
    return "Id: "+id+"\n"
                     "Summary: "+summary+"\n"
                                         "Category: "+category+"\n"
                                                               "Categories: "+categories+"\n"
                                                                                         "License: "+license+"\n"+latest+ "No. Packages: "+QString::number(cant)+s
            +(requirements=="root"?"\nNeed [ROOT]":"");
}

AppInfo::~AppInfo()
{
}
