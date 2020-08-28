#ifndef KEYSTOREINFO
#define KEYSTOREINFO

#include <QString>

class KeystoreInfo
{
public:
    KeystoreInfo(){
        this->commonName="";
        this->orgUnit="";
        this->org="";
        this->locality="";
        this->state="";
        this->country="";
    }

    KeystoreInfo(const QString commonName,const QString orgUnit,const QString org,
                 const QString locality,const QString state,const QString country){
        this->commonName=commonName;
        this->orgUnit=orgUnit;
        this->org=org;
        this->locality=locality;
        this->state=state;
        this->country=country;
    }

    QString ObtenerCN() const{
        return commonName;
    }
    QString ObtenerON() const{
        return orgUnit;
    }
    QString ObtenerO() const{
        return org;
    }
    QString ObtenerL() const{
        return locality;
    }
    QString ObtenerS() const{
        return state;
    }
    QString ObtenerC() const{
        return country;
    }

    bool Check(){
        QString str = commonName + orgUnit + org + locality + state + country;
        return str!="";
    }

    QString toString(){
        return "Signature: \n"
               "Common Name(CN): "+commonName+"\n"
               "Organizational Unit(OU): "+orgUnit+",  Organization(O): "+org+"\n"
               "Locality(L): "+locality+",  State(ST): "+state+",  Country(C): "+country+"";
    }

    //Common Name (CN), Organizational Unit (OU), Oranization(O), Locality (L), State (ST) and Country (C)
private:
    QString commonName;
    QString orgUnit;
    QString org;
    QString locality;
    QString state;
    QString country;
};

#endif // KEYSTOREINFO

