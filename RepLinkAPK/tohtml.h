#ifndef CTOHTMLHILO_H
#define CTOHTMLHILO_H

#include <QThread>
#include <QFile>
#include <QString>
#include <QDomDocument>
#include <appinfo.h>
#include <iostream>
#include <listaSE.h>
#include <pkginfo.h>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>



class CToHtlmHilo: public QThread{
Q_OBJECT
private:

    QString repo;
    QString xmlFilePath;
    QString outputDir;
    bool latest;
    bool plain;

public:

    explicit CToHtlmHilo(QObject *parent=0);

    void Cerrar();

    void SetParams(QString repo, bool latest, bool plain, QString outputDir);

protected:
    void run();


signals:
    void Porciento(int, QString, QString);
};


#endif // CTOHTMLHILO_H
