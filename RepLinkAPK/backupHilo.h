#ifndef CBACKUPHILO_H
#define CBACKUPHILO_H

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
#include <QDate>
#include <QSettings>

class CBackupHilo: public QThread{
Q_OBJECT
private:

    QString repo;
    QString xmlFilePath;
    QString backupDir;

public:

    explicit CBackupHilo(QObject *parent=0);

    void Cerrar();

    void SetParams(QString repo, QString backupDir);

protected:
    void run();


signals:
    void Porciento(int, QString, QString);
};


#endif // CBACKUPHILO_H
