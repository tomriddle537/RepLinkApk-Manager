#ifndef COUNTHILO_H
#define COUNTHILO_H

#include <QThread>
#include <QFile>
#include <QString>
#include <QDomDocument>
#include <iostream>
#include <listaSE.h>
#include <cxmlrepo.h>

class CCountHilo: public QThread{
Q_OBJECT
private:

    QString repo;
    QString xmlFilePath;

public:

    explicit CCountHilo(QObject *parent=0);

    void Cerrar();

    void SetParams(QString repo);

protected:
    void run();


signals:
    void AppPerCat(QStringList);
    void Porciento(int);
};


#endif // COUNTHILO_H
