#ifndef REPORTHILO_H
#define REPORTHILO_H

#include <QThread>
#include <QFile>
#include <QString>
#include <QDomDocument>
#include <iostream>
#include <listaSE.h>
#include <cxmlrepo.h>
#include <listaSE.h>
#include <QFileInfo>
#include <QDir>

class CReportHilo: public QThread{
Q_OBJECT
private:

    QString repo;
    QString xmlFilePath;
    QStringList existFiles;

    ListaSE<QStringList> ExactSizeApps();
public:

    explicit CReportHilo(QObject *parent=0);

    void Cerrar();

    void SetParams(QString repo, QStringList existFiles);

protected:
    void run();


signals:
    void Report(QString,QStringList,QStringList);
    void Porciento(int);
};


#endif // REPORTHILO_H
