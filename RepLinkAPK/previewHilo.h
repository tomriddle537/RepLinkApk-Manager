#ifndef PREVIEWHILO_H
#define PREVIEWHILO_H

#include <QThread>
#include <QFile>
#include <QString>
#include <QSettings>
#include <QProcess>
#include <QListWidget>
#include <QListWidgetItem>
#include <QDomDocument>
#include <iostream>
#include <listaSE.h>
#include <cxmlrepo.h>
#include <newapkfile.h>
#include <apkpreview.h>
#include <caaptparser.h>

class CPreviewHilo: public QThread{
Q_OBJECT
private:

    QString repo;
    ListaSE<NewAPKFile> files;
    CBinOutputParser *bop;
    bool fromApk;


public:

    explicit CPreviewHilo(QObject *parent=0);

    void Cerrar();

    void SetParams(QString repo, ListaSE<NewAPKFile> files, bool fromApk=true);

protected:
    void run();


signals:
    void Preview(ApkPreview);
    void Porciento(int);
};


#endif // PREVIEWHILO_H
