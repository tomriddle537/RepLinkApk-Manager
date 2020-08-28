#ifndef CSECURITY_H
#define CSECURITY_H

#include <iostream>
#include <QString>
#include <QByteArray>
#include <QCryptographicHash>
#include <dataccess.h>

class CSecurity: public QObject{
    Q_OBJECT

public:
    explicit CSecurity(QObject *parent = 0);
    ~CSecurity();
    QString getLastMessage();
    QString getCurrentId();
    QString getMD5(QString text);
private:
    QString currentSB;
    QString lastMsg;

    QByteArray strChecksum(const QString &data, QCryptographicHash::Algorithm hashAlgorithm);
};

#endif // CSECURITY_H
