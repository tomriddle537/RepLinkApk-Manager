#ifndef JSONHILO_H
#define JSONHILO_H

#include <QThread>
#include <QFile>
#include <QString>
#include <QDomDocument>
#include <QDateTime>
#include <appinfo.h>
#include <iostream>
#include <listaSE.h>
#include <pkginfo.h>
#include <cxmlmethods.h>
#include <QDebug>
#include <QDir>
#include <QFileInfoList>
#include <QtMath>

class CJSONHilo: public QThread{
Q_OBJECT
private:

    QString repo;
    QString xmlFilePath;
    QString jsonFilePath;
    CXMLMethods *xmlM;

public:

    explicit CJSONHilo(QObject *parent=0);

    void Cerrar();

    void SetParams(CXMLMethods *xmlM, QString repo);

protected:
    void run();


signals:
    void Porciento(int);
};


#endif // JSONHILO_H
