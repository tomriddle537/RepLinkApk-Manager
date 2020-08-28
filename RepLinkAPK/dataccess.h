#ifndef DATACCESS
#define DATACCESS

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlResult>
#include <QSettings>
#include <QDir>
#include <QDebug>
#include <iostream>
#include <repoinfo.h>
#include <listaSE.h>
#include <QTableWidget>

class DatAccess{
private:
    QSqlQuery ConsultarDB(QString consulta)
    {

        QString appDir="";

        QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                           "RepLink APK Manager", "RepLinkAPK");

        appDir+=settings.value("AppDirs/path").toString()+"/";

        QSqlDatabase db;
        if(!QSqlDatabase::database("myDefault").isValid()){
            db=QSqlDatabase::addDatabase("QSQLITE", "myDefault");
            db.setDatabaseName(appDir+"config.sqlitedb");
            db.open();
        }else{
            db=QSqlDatabase::database("myDefault");
            db.open();
        }



        if(!db.open()){
            return QSqlQuery("ERROR");
        }else{
            QSqlQuery consultar(QSqlDatabase::database("myDefault"));
            consultar.exec(consulta);

            return consultar;
        }
    }






public:
    DatAccess(){

    }



    ListaSE<RepoInfo> SelectRepoFromId(int idRepo){
        QSqlQuery consulta;
        ListaSE<RepoInfo> repos;
        RepoInfo repo;

        QString name;
        QString dir;
        QString url;
        QString backup;
        int id;


        if( idRepo!=-1){
            consulta=ConsultarDB("SELECT * FROM repo WHERE id='"+QString::number(idRepo)+"' ORDER BY name ASC");
        }else{
            consulta=ConsultarDB("SELECT * FROM repo ORDER BY name ASC");
        }

        int campoName = consulta.record().indexOf("name");
        int campoDir = consulta.record().indexOf("dir");
        int campoUrl = consulta.record().indexOf("url");
        int campoId = consulta.record().indexOf("id");
        int campoBackup = consulta.record().indexOf("backup");

        while (consulta.next()) {
            name = consulta.value(campoName).toString();
            dir = consulta.value(campoDir).toString();
            url = consulta.value(campoUrl).toString();
            id = consulta.value(campoId).toInt();
            backup = consulta.value(campoBackup).toString();

            repo= RepoInfo(id, name,dir ,url, backup);
            repos.AdicionarCola(repo);
        }

        return repos;
    }

    QString SelectAndroidVerFromSdk(int idSdk, bool large){
        QSqlQuery consulta;
        ListaSE<RepoInfo> repos;
        RepoInfo repo;

        QString version;
        QString descripcion;


        if( idSdk!=-1){
            consulta=ConsultarDB("SELECT * FROM sdk WHERE id='"+QString::number(idSdk)+"' ORDER BY id ASC");
        }else{
            consulta=ConsultarDB("SELECT * FROM sdk ORDER BY name ASC");
        }

        int campoVersion = consulta.record().indexOf("version");
        int campoDescripcion = consulta.record().indexOf("descripcion");

        while (consulta.next()) {
            version = consulta.value(campoVersion).toString();
            descripcion = consulta.value(campoDescripcion).toString();
        }

        return large?descripcion:version;
    }

    int ObtenerIdRepo(QString name)
    {
        QSqlQuery result=ConsultarDB(QString("select id from repo where nombre=\"%1\" ").arg(name));
        result.next();
        if(result.lastError().text()==" "){
            return result.value(0).toInt();
        }else{
            return -1;
        }
    }

    bool UpdateRepoInfo(RepoInfo repo)
    {
        QSqlQuery result=ConsultarDB(QString("update repo set name='%2',dir='%3', url='%4', backup='%5' where id=%1").arg(
                                         QString::number(repo.ObtenerId()), repo.ObtenerName(), repo.ObtenerDir(), repo.ObtenerUrl(), repo.ObtenerBackUpDir()));


        if(result.lastError().text()==" "){
            return true;
        }else{
            return false;
        }
    }

    bool UpdateRecent(QString recent, int id)
    {
        if(SelectRecent(id)=="Error"){
            AddRecent(recent, id);
        }

        QString sql=QString("update recent set dir='%1' where id='%2'").arg(
                    recent, QString::number(id));
        QSqlQuery result=ConsultarDB(sql);


        if(result.lastError().text()==" "){
            return true;
        }else{
            return false;
        }
    }

    bool AddRecent(QString recent, int id)
    {
        QString sql=QString("insert into recent(id,dir) values(\"%1\",\"%2\") ").arg(
                    QString::number(id),recent);
        QSqlQuery result=ConsultarDB(sql);


        if(result.lastError().text()==" "){
            return true;
        }else{
            return false;
        }
    }

    QString SelectRecent(int id){
        QString sql=QString("select dir from recent where id='%1'").arg(QString::number(id));
        QSqlQuery result=ConsultarDB(sql);
        if(result.next() && result.lastError().text()==" "){
            return result.value(0).toString();
        }else{
            return "Error";
        }
    }

    bool DeleteRepo(int id)
    {
        QSqlQuery result=ConsultarDB(QString("delete from repo where id=%1").arg(QString::number(id)));
        if(result.lastError().text()==" "){
            return true;
        }else{
            return false;
        }

    }

    bool InsertRepo(QString name, QString dir, QString url)
    {

        QSqlQuery result=ConsultarDB(QString("insert into repo(name,dir,url) values(\"%1\",\"%2\",\"%3\")").arg(
                                         name,
                                         dir,
                                         url));


        if(result.lastError().text()==" "){
            return true;
        } else{
            return false;
        }
    }

    QString ObtenerSize(double size )
    {
        if(size<1024){
            return QString::number(size)+" Bytes";
        }else {
            double kilos=size/1024;
            if(kilos<1024){
                return QString::number((int)kilos)+" KB";
            }else {
                double megas=kilos/1024;
                if(megas<1024){
                    return QString::number((int)megas)+"."+QString::number((int)kilos%1024)+" MB";
                }else {
                    return QString::number((int)(megas/1024))+"."+QString::number((int)megas%1024)+" GB";
                }
            }
        }
    }

};



#endif // DATACCESS

