#ifndef NEWAPKFILE
#define NEWAPKFILE


#include <QString>

class NewAPKFile{

private:
    QString apkname;
    QString category;

public:

    NewAPKFile(){}
    ~NewAPKFile(){}
    NewAPKFile( QString apkname, QString category="Uncategorized"){
        this->apkname=apkname;
        this->category=category;
    }

    QString ObtenerAPKName() {
        return apkname;
    }
    QString ObtenerCategory() {
        return category;
    }

    void SetAPKName(QString apkname){
        this->apkname=apkname;
    }
    void SetCategory(QString category){
        this->category=category;
    }

    QString toString(){
        return "Category: "+category;
    }


};


//Q_DECLARE_METATYPE(APKFile);



#endif // NEWAPKFILE

