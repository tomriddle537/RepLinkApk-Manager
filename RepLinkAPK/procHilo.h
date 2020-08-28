#ifndef PROCHILO_H
#define PROCHILO_H

#include <QThread>
#include <caaptparser.h>
#include <QFileInfo>
#include <QProcess>
#include <QDebug>
#include <QByteArray>
#include <QPixmap>
#include <cxmlapp.h>
#include <cxmlmethods.h>
#include <newapkfile.h>

class CProcHilo: public QThread{
Q_OBJECT
private:
    CBinOutputParser *bop;
    CXMLMethods *xmlM;
    QString repository;
    QString catDefault;
    ListaSE<NewAPKFile> files;
    QDomDocument domDocument;
    bool bDefault;
    bool remoteFiles;

    QString desc;
    QString summary;
    QString category;
    QString license;
    QString web;
    QString source;


public:

    explicit CProcHilo(QObject *parent=0);

    void Cerrar();

    void SetParametros(CXMLMethods *xmlM, QString repository, ListaSE<NewAPKFile> files, QDomDocument &domDocument, bool remoteFiles=false, bool bDefault=false,  QString catDefault="Unknown");


signals:
    void Mensaje(QString, int);
    void Imagen(QPixmap);
    void Porciento(int);

protected:
    void run();

private slots:
    void slot_Img(QPixmap img);
    void slot_Notice(QString msg, int type);

};


#endif // PROCHILO_H
