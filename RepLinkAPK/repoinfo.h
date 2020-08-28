#ifndef REPOINFO
#define REPOINFO

#include <QString>
#include <QStringList>
#include <QMetaType>

class RepoInfo
{
public:
    RepoInfo(){}

    RepoInfo(const int id, const QString name, const QString dir, const QString url, const QString backupDir){
        this->id=id;
        this->name=name;
        this->dir=dir;
        this->url=url;
        this->backupDir=backupDir;
    }

    int ObtenerId() const{
        return id;
    }
    QString ObtenerName() const{
        return name;
    }
    QString ObtenerDir() const{
        return dir;
    }
    QString ObtenerBackUpDir() const{
        return backupDir;
    }
    QString ObtenerUrl() const{
        return url;
    }
    QString ObtenerDescription() const{
        return description;
    }
    void SetDescription(QString description){
        this->description=description;
    }
    QString toString(){
        return //"Id: "+QString::number(id)+"\n"
                         "Dir: "+dir+""+ ( url.length()>0?QString("\nUrl: "+url+""):"")
                       + ( backupDir.length()>0?QString("\nBackup Dir: "+backupDir+""):"");
    }

private:
    int id;
    QString name;
    QString dir;
    QString url;
    QString description;
    QString backupDir;
};

Q_DECLARE_METATYPE(RepoInfo);


#endif // REPOINFO



