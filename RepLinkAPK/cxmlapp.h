#ifndef CXMLAPP
#define CXMLAPP
#include <QString>
#include <QDomDocument>
#include <QDomText>
#include <listaSE.h>
#include <cxmlpackage.h>

class CXMLAplication {

private:
    QString id;
    QString added;
    QString lastupated;
    QString name;
    QString summary;
    QString icon;
    QString desc;
    QString license;
    QString categorYies;
    QString web;
    QString source;
    QString tracker;
    QString marketversion;
    QString marketvercode;
    QString requirements;
    ListaSE<CXMLPackage> packages;
    QDomDocument domDocument;


public:

    CXMLAplication(){
        this->id="";
        this->added="";
        this->lastupated="";
        this->name="";
        this->summary="";
        this->icon="";
        this->desc="";
        this->license="";
        this->categorYies="";
        this->web="";
        this->source="";
        this->tracker="";
        this->marketversion="";
        this->marketvercode="";
    }


    CXMLAplication(QString id, QString added, QString lastupated, QString name, QString summary, QString icon, QString desc,
                   QString categorYies, QString marketversion, QString marketvercode, ListaSE<CXMLPackage> package, QString requirements,
                   QString license="", QString web="", QString source="", QString tracker=""){
        this->id=id;
        this->added=added;
        this->lastupated=lastupated;
        this->name=name;
        this->summary=summary;
        this->icon=icon;
        this->desc=desc;
        this->license=license;
        this->categorYies=categorYies;
        this->web=web;
        this->source=source;
        this->tracker=tracker;
        this->marketversion=marketversion;
        this->marketvercode=marketvercode;
        this->packages=package;
        this->requirements=requirements;
    }

    QString ObtenerId()
    {
        return this->id;
    }

    QString ObtenerMarketVersion()
    {
        return this->marketversion;
    }

    QString ObtenerMarketVersionCode()
    {
        return this->marketvercode;
    }

    QString ObtenerLastUpdated()
    {
        return this->lastupated;
    }

    QString ObtenerName()
    {
        return this->name;
    }

    QString ObtenerSummary()
    {
        return this->summary;
    }

    QString ObtenerIcon()
    {
        return this->icon;
    }

    QString ObtenerDescription()
    {
        return this->desc;
    }

    QString ObtenerLicense()
    {
        return this->license;
    }

    QString ObtenerAddedDate()
    {
        return this->added;
    }

    QString ObtenerCategorYies()
    {
        return this->categorYies;
    }

    QString ObtenerWeb()
    {
        return this->web;
    }

    QString ObtenerSource()
    {
        return this->source;
    }

    QString ObtenerTracker()
    {
        return this->tracker;
    }

    QString ObtenerRequirements()
    {
        return this->requirements;
    }

    ListaSE<CXMLPackage> ObtenerPackages()
    {
        return this->packages;
    }

    void SetDescription(QString desc)
    {
        this->desc=desc;
    }

    void SetSummary(QString summary)
    {
        this->summary=summary;
    }

    void SetCategorYies(QString categorYies)
    {
        this->categorYies=categorYies;
    }

    void SetWeb(QString web)
    {
        this->web=web;
    }

    void SetSource(QString source)
    {
        this->source=source;
    }

    void SetLicense(QString license)
    {
        this->license=license;
    }

    QDomElement toXML(){
        QDomText text;

        QDomElement xid=domDocument.createElement("id");
        text=domDocument.createTextNode(ObtenerId());
        xid.appendChild(text);
        QDomElement xadded=domDocument.createElement("added");
        text=domDocument.createTextNode(ObtenerAddedDate());
        xadded.appendChild(text);
        QDomElement xlastupated=domDocument.createElement("lastupdated");
        text=domDocument.createTextNode(ObtenerLastUpdated());
        xlastupated.appendChild(text);
        QDomElement xname=domDocument.createElement("name");
        text=domDocument.createTextNode(ObtenerName());
        xname.appendChild(text);
        QDomElement xsummary=domDocument.createElement("summary");
        text=domDocument.createTextNode(ObtenerSummary());
        xsummary.appendChild(text);
        QDomElement xicon=domDocument.createElement("icon");
        text=domDocument.createTextNode(ObtenerIcon());
        xicon.appendChild(text);
        QDomElement xdesc=domDocument.createElement("desc");
        text=domDocument.createTextNode("&lt;p&gt;"+ObtenerDescription()+"&lt;/p&gt;");
        xdesc.appendChild(text);
        QDomElement xlicence=domDocument.createElement("license");
        text=domDocument.createTextNode(ObtenerLicense());
        xlicence.appendChild(text);
        QDomElement xcategories=domDocument.createElement("categories");
        text=domDocument.createTextNode(ObtenerCategorYies());
        xcategories.appendChild(text);
        QDomElement xcategory=domDocument.createElement("category");
        text=domDocument.createTextNode(ObtenerCategorYies());
        xcategory.appendChild(text);
        QDomElement xweb=domDocument.createElement("web");
        text=domDocument.createTextNode(ObtenerWeb());
        xweb.appendChild(text);
        QDomElement xsource=domDocument.createElement("source");
        text=domDocument.createTextNode(ObtenerSource());
        xsource.appendChild(text);
        QDomElement xtracker=domDocument.createElement("tracker");
        text=domDocument.createTextNode(ObtenerTracker());
        xtracker.appendChild(text);
        QDomElement xmarketversion=domDocument.createElement("marketversion");
        text=domDocument.createTextNode(ObtenerMarketVersion());
        xmarketversion.appendChild(text);
        QDomElement xmarketvercode=domDocument.createElement("marketvercode");
        text=domDocument.createTextNode(ObtenerMarketVersionCode());
        xmarketvercode.appendChild(text);
        QDomElement xrequirements=domDocument.createElement("requirements");
        text=domDocument.createTextNode(ObtenerRequirements());
        xrequirements.appendChild(text);


        QDomElement xapp=domDocument.createElement("application");
        xapp.setAttribute("id", ObtenerId());

        xapp.appendChild(xid);
        xapp.appendChild(xadded);
        xapp.appendChild(xlastupated);
        xapp.appendChild(xname);
        xapp.appendChild(xsummary);
        xapp.appendChild(xicon);
        xapp.appendChild(xdesc);
        xapp.appendChild(xlicence);
        xapp.appendChild(xcategories);
        xapp.appendChild(xcategory);
        xapp.appendChild(xweb);
        xapp.appendChild(xsource);
        xapp.appendChild(xtracker);
        xapp.appendChild(xmarketversion);
        xapp.appendChild(xmarketvercode);
        xapp.appendChild(xrequirements);
        for (int i = 1; i <= ObtenerPackages().Longitud(); ++i) {
                xapp.appendChild(ObtenerPackages().ObtenerPorPos(i).toXML());
        }

        return xapp;
    }

};




#endif // CXMLAPP

