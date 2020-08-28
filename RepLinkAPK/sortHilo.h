#ifndef SORTHILO_H
#define SORTHILO_H

#include <QThread>
#include <QFileInfo>
#include <QProcess>
#include <QDebug>
#include <QListWidget>
#include <appinfo.h>

class SortHilo: public QThread{
    Q_OBJECT

private:
    QListWidget *appsView;
    bool asc;
public:
    explicit SortHilo(QObject *parent=0);

    void Cerrar();

    void SetParametros(QListWidget *&appsView, bool asc);
signals:
    void Porciento(int);
protected:
    void run();

};

#endif // SORTHILO_H
