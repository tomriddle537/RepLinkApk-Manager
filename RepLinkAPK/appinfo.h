#ifndef APPINFO_H
#define APPINFO_H

#include <QString>
#include <QStringList>
#include <QPixmap>
#include <pkginfo.h>
#include <listaSE.h>
#include <QMetaType>



class AppInfo{

private:
    QString id;
    QString name;
    QString summary;
    QString desc;
    QString license;
    QString category;
    QString categories;
    QString web;
    QString version;
    QString versionCode;

    QString sdkVer;
    QString targetSdkVer;
    QString requirements;
    ListaSE<PkgInfo> packages;
    double size;
    QString versionCodeObb;
    QString obbMainFile;
    QString obbPatchFile;
    QString source;
    QStringList features;
    QStringList permissions;
    QStringList densities;
    QStringList screens;



public:

    AppInfo();
    ~AppInfo();
    AppInfo(const QString id, const QString name, const QString summary, const QString desc,
            const QString category, const QString categories, const QString license, const QString source, const QString version, ListaSE<PkgInfo> packages, const QString requirements, const QString versionCode="", const QString sdkVer="", const QString targetSdkVer="" );

    QString ObtenerId() const;
    QString ObtenerName() const;
    QString ObtenerSummary() const;
    QString ObtenerDescription() const;
    QString ObtenerLicense() const;
    QString ObtenerCategory() const;
    QString ObtenerCategories() const;
    QString ObtenerWeb() const;
    QString ObtenerLatestVersion() const;
    QString ObtenerVersionCode() const;
    QString ObtenerSdkVersion() const;
    QString ObtenerTargetSdkVersion() const;
    QString ObtenerNoPackages();
    QString ObtenerRequirements();
    ListaSE<PkgInfo> ObtenerPackages();
    QString ObtenerSize() const;
    QString ObtenerSource() const;
    QStringList ObtenerFeatures() const;
    QStringList ObtenerPermisions() const;
    QStringList ObtenerDensities() const;
    QStringList ObtenerScreens() const;

    QString ObtenerVersionCodeToObb();
    QString ObtenerMainObb();
    QString ObtenerPatchObb();


    void SetPackages(ListaSE<PkgInfo> packages);
    void SetName(const QString name);
    void SetDescription(const QString desc);
    void SetSummary(const QString summary);
    void SetCategory(const QString category);
    void SetCategories(const QString categories);
    void SetWeb(const QString source);
    void SetRequirements(const QString requirements);
    void SetSize(const double size);
    void SetLicense(const QString license);
    void SetSource(const QString source);
    void SetFeatures(const QStringList features);
    void SetPermissions(const QStringList permissions);
    void SetDensities(const QStringList densities);
    void SetScreens(const QStringList screens);
    void SetVersionCodeToObb(QString versionCodeObb);
    void SetMainObb(QString mainObb);
    void SetPatchObb(QString patchObb);


    QString toString();


};


Q_DECLARE_METATYPE(AppInfo);

#endif // APPINFO_H

