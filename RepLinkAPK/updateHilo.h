#ifndef UPDATEHILO_H
#define UPDATEHILO_H

#include <QThread>
#include <QFile>
#include <QString>
#include <QDomDocument>
#include <iostream>
#include <listaSE.h>
#include <cxmlrepo.h>

class CUpdateHilo: public QThread{
Q_OBJECT
private:

    QString repo;
    QString xmlFilePath;

public:

    explicit CUpdateHilo(QObject *parent=0);

    void Cerrar();

    void SetParams(QString repo);

protected:
    void run();


signals:
    void UpdateRepoInfo(QString,QString,CXMLRepo);
    void Porciento(int);
};


#endif // UPDATEHILO_H
