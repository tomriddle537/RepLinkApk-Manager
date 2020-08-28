#ifndef CAAPTPARSER_H
#define CAAPTPARSER_H

#include <QObject>
#include <QPixmap>
#include <cxmlapp.h>
#include <appinfo.h>
#include <cxmlpackage.h>
#include <cxmlmethods.h>
#include <keystoreinfo.h>
#include <QFile>
#include <QDate>
#include <QDebug>
#include <QFileInfo>
#include <QMessageBox>
#include <QSettings>
#include <QCryptographicHash>
#include <QProcess>
#include <QByteArray>
#include <listaSE.h>
#include <QDir>
#include <QMessageBox>
#include <apkpreview.h>
#include <csecurity.h>



class CBinOutputParser: public QObject{
    Q_OBJECT

public:
    explicit CBinOutputParser(QObject *parent = 0);

    CXMLAplication binOutputParser(QString repository, QStringList lines, QString url, QFileInfo infoFile);
    AppInfo binOutputParser(QStringList lines, QString url, QFileInfo infoFile);
    ApkPreview binOutputParser(QStringList lines, QString url);
    KeystoreInfo binOutputParser();





    bool isIn(QString property);
    QByteArray fileChecksum(const QString &fileName, QCryptographicHash::Algorithm hashAlgorithm);


signals:
    void ImgActual(QPixmap);
    void Notice(QString,int);

private:
    QString parseGetSig(QFileInfo infoFile);
    void iconProcess(QString repository, QString url, QString id, QString rutaicon, QStringList densities , QStringList iconDens, bool rewrite=false);
    QPixmap extractIcon(QString id, QString url , QString &rutaicon);



};

#endif // CAAPTPARSER_H
