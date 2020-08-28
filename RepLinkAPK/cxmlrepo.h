#ifndef CXMLREPO
#define CXMLREPO

#include <QString>
#include <QStringList>
#include <QDomDocument>
#include <QDomText>


class CXMLRepo {

private:
    QString name;
    QString url;
    QString desc;
    QString icon;
    QString pubkey;
    QString version;
    QString timestamp;
    QDomDocument domDocument;


public:

    CXMLRepo(){
        this->name="";
        this->url="";
        this->desc="";
        this->icon="repo-icon.png";
        this->pubkey="";
        this->version="17";
        this->timestamp="20121231115959";
    }


    CXMLRepo(QString name,QString url,QString desc,QString icon,QString pubkey,QString version){
        this->name=name;
        this->url=url;
        this->desc=desc;
        this->icon=icon;
        this->version=version;
        this->pubkey=pubkey;
        this->timestamp="20121231115959";
    }

    QString ObtenerName()
    {
        return this->name;
    }

    QString ObtenerUrl()
    {
        return this->url;
    }

    QString ObtenerDescription()
    {
        return this->desc;
    }

    QString ObtenerIcon()
    {
        return this->icon;
    }

    QString ObtenerPubkey()
    {
        return this->pubkey;
    }

    QString ObtenerVersion()
    {
        return this->version;
    }


    QString ObtenerTimeStamp()
    {
        return this->timestamp;
    }

    void SetTimeStamp(QString timestamp)
    {
        this->timestamp=timestamp;
    }

    QDomElement toXML(){
        QDomText text;

        QDomElement xdesc=domDocument.createElement("description");
        text=domDocument.createTextNode(ObtenerDescription());
        xdesc.appendChild(text);

        QDomElement xrepo=domDocument.createElement("repo");
        xrepo.setAttribute("name", ObtenerName());
        xrepo.setAttribute("pubkey", ObtenerPubkey());
        xrepo.setAttribute("url", ObtenerUrl());
        xrepo.setAttribute("timestamp", ObtenerTimeStamp());
        xrepo.setAttribute("version", ObtenerVersion());
        xrepo.setAttribute("icon", ObtenerIcon());
        xrepo.setAttribute("maxage", "14");
        xrepo.appendChild(xdesc);

        return xrepo;
    }



};

Q_DECLARE_METATYPE(CXMLRepo);

#endif // CXMLREPO

