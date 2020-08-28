#include "csecurity.h"

CSecurity::CSecurity(QObject *parent): QObject(parent)
{
    Q_UNUSED(parent);
}

QByteArray CSecurity::strChecksum(const QString &data, QCryptographicHash::Algorithm hashAlgorithm)
{
    QCryptographicHash hash(hashAlgorithm);
    hash.addData(data.toLocal8Bit());
    return hash.result();
}

CSecurity::~CSecurity()
{

}

QString CSecurity::getLastMessage()
{
    return lastMsg;
}

QString CSecurity::getMD5(QString text)
{
    QCryptographicHash::Algorithm hashAlgorithm=QCryptographicHash::Md5;
    QByteArray checksum=strChecksum(text,hashAlgorithm);
    return checksum.toHex();

}
