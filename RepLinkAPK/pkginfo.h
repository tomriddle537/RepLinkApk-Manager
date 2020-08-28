#ifndef PKGINFO_H
#define PKGINFO_H

#include <QString>
#include <QStringList>
#include <QPixmap>
#include <QMetaType>
#include <dataccess.h>

class PkgInfo
{
public:
    PkgInfo();
    ~PkgInfo();
   PkgInfo(const QString id, const QString version, const QString versionCode, const QString pkgname, const QString sdkver, const QString size, const QString mainObb, const QString patchObb);

    QString ObtenerId() const;
    QString ObtenerVersion() const;
    QString ObtenerVersionCode() const;
    QString ObtenerSize() const;
    QString ObtenerApkName() const;
    QString ObtenerSDKVer() const;
    QString ObtenerMainObb() const;
    QString ObtenerPatchObb() const;
    QString toString();


    void SetApkName(const QString apkname);
    void SetMainObb(const QString mainObb);
    void SetPatchObb(const QString patchObb);
    QString AndroidVersion(QString sdk, bool large);

private:
    QString id;
    QString version;
    QString versionCode;
    QString apkname;
    QString sdkver;
    QString size;
    QString obbMainFile;
    QString obbPatchFile;


};

Q_DECLARE_METATYPE(PkgInfo);

#endif // PKGINFO_H
