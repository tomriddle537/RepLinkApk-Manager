#ifndef LISTHILO_H
#define LISTHILO_H

#include <QThread>
#include <QFile>
#include <QString>
#include <QDomDocument>
#include <appinfo.h>
#include <iostream>
#include <listaSE.h>
#include <pkginfo.h>

class CListHilo: public QThread{
Q_OBJECT
private:

    QString repo;
    QString xmlFilePath;

public:

    explicit CListHilo(QObject *parent=0);

    void Cerrar();

    void SetParams(QString repo);

protected:
    void run();


signals:
    void SendInfo(AppInfo);
    void Porciento(int);
};


#endif // LISTHILO_H
