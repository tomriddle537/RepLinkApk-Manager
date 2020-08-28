#ifndef CXMLMETHODS_H
#define CXMLMETHODS_H

#include <QDebug>
#include <QProcess>
#include <QByteArray>
#include <QDate>
#include <QSettings>
#include <QXmlStreamReader>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QDomDocument>
#include <QDomImplementation>
#include <QDomNodeList>
#include <QTextStream>
#include <QCryptographicHash>
#include <QByteArray>
#include <cxmlapp.h>
#include <cxmlpackage.h>
#include <appinfo.h>
#include <repoinfo.h>
#include <cxmlrepo.h>
#include <iostream>
#include <listaSE.h>
#include <QProcess>
#include <QSettings>



class CXMLMethods: public QObject{
    Q_OBJECT

signals:
    void Notice(QString,int);
public:
    explicit CXMLMethods(QObject *parent = 0);

    void XMLParse(QDomDocument &domDocument, CXMLAplication &app, QFileInfo infoFile, bool remoteFiles=false);
    QString XMLWrite(QString xmlFilePath, QDomDocument &domDocument, bool updateTS=true);
    QString XMLWriteJSON(QString repo, QString jsonFilePath, QString json);
    QString XMLCantApps(QString xmlFilePath);
    QString XMLCantPkg(QString xmlFilePath);
    QString XMLTimeStamp(QString xmlFilePath);
    QString XMLRepoIcon(QString xmlFilePath);
    CXMLRepo XMLRepoInfo(QString xmlFilePath);
    void CreateNewRepo(CXMLRepo rinfo, QString path);
    QString MergeRepo(QString xmlFilePath, QString xmlFilePathDos);
    QString SubstractRepo(QString xmlFilePath, QString xmlFilePathDos);
    QString UpdateAppInfo(QString xmlFilePath, AppInfo appInfo);
    QString UpdateRepoInfo(QString xmlFilePath, RepoInfo repoInfo);
    QString UpdateApkName(QString xmlFilePath, QString oldName, QString newName);
    QString DeleteApp(QString xmlFilePath, AppInfo appInfo);
    QString DeletePkg(QString xmlFilePath, PkgInfo pkgInfo);
    QString DeleteAllPkg(QString xmlFilePath);
    QString SummaryLikeCategory(QString xmlFilePath);
    //ListaSE<AppInfo> GetAppsInfo(QString xmlFilePath);
    QStringList UsedAPKs(QString xmlFilePath);
    void SignIndexXML(QString xmlFilePath);
    QString GetPubKey();
    int GetAppLatestVersion(QString xmlFilePath, QString id);
    QByteArray fileChecksum(const QString &fileName, QCryptographicHash::Algorithm hashAlgorithm);
    QString GetPkgSize(QString repo, QDomElement pkg);



    ~CXMLMethods();


private:
    QString UpdateTimeStamp(QDomDocument &domDocument);
    void CreateSignedIndexJar(QString xmlFilePath);
    void CreateSignedIndexV1Jar(QString repo, QString jsonFilePath);

};

#endif // CXMLMETHODS_H
