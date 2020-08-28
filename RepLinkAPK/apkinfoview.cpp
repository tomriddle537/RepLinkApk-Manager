#include "apkinfoview.h"
#include "ui_apkinfoview.h"

ApkInfoView::ApkInfoView(QString repository, CXMLMethods *xmlM, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ApkInfoView)
{
    ui->setupUi(this);
    this->xmlM=xmlM;
    this->repository=repository;

    ui->lCopiando->setVisible(false);

    QFileDialog abrir;
    QString manejador= abrir.getOpenFileName(this, tr("Abrir APK"), repository==""?DatAccess().SelectRecent(1):this->repository, tr("Android Application (*.apk)"));

    url=manejador.toUtf8();

    url.replace("\\","/");

    QStringList partes= url.split("/");
    file=partes.last();
    partes.removeAt(partes.length()-1);
    QString aux=partes.join("/");

    url=aux;
    if(repository==""){
        DatAccess().UpdateRecent(url,1);
    }

    ui->tbNext->setVisible(false);
    ui->tbPrev->setVisible(false);

    direct=false;
    inXML=false;





    ApkView();

}

ApkInfoView::ApkInfoView(QString url, QString repository,  CXMLMethods *xmlM, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ApkInfoView)
{
    ui->setupUi(this);
    this->xmlM=xmlM;
    this->repository=repository;

    ui->lCopiando->setVisible(false);

    url.replace("\\","/");
    QStringList partes= url.split("/");

    file=partes.last();

    partes.removeAt(partes.length()-1);
    this->url=partes.join("/");
    ui->bCopyToRepo->setVisible(false);
    ui->bSendProcess->setVisible(false);
    ui->tbLog->setVisible(false);
    ui->tbNext->setVisible(false);
    ui->tbPrev->setVisible(false);

    direct=false;
    inXML=false;

    ApkView();

}

ApkInfoView::ApkInfoView(QString url, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ApkInfoView)
{
    ui->setupUi(this);

    ui->lCopiando->setVisible(false);


    url.replace("\\","/");
    QStringList partes= url.split("/");

    file=partes.last();

    partes.removeAt(partes.length()-1);

    this->url=partes.join("/");

    //    if(file.indexOf(" ")!=-1){
    //        QFile f(this->url+"/"+file);
    //        QString newName=file.replace(" ","_");
    //        f.rename(this->url+"/"+newName);
    //        file=newName;
    //    }

    ui->bCopyToRepo->setVisible(false);
    ui->bSendProcess->setVisible(false);
    ui->tbLog->setVisible(false);
    direct=true;
    inXML=false;

    ApkView();
}

ApkInfoView::ApkInfoView()
{

}

ApkInfoView::~ApkInfoView()
{
    delete ui;
}

void ApkInfoView::slot_ImagenAct(QPixmap pixmap)
{

    ico.addPixmap(pixmap);
    ui->lIcon->setPixmap(pixmap);
    if(pixmap.width()> 110 || pixmap.height()> 110 ){
        ico.addPixmap(pixmap.scaled(110,110,Qt::KeepAspectRatioByExpanding));
        ui->lIcon->setPixmap(ico.pixmap(110,110));
    }

    this->setWindowIcon(ico);
}

//void ApkInfoView::slot_Porc(qint64 bytes)
//{
//    qint64 total= QFileInfo(xfile->fileName()).size();
//    int porc= (int)(100*bytes)/total;
//    if(porc!=100){
//        ui->lCopiando->setVisible(true);
//        ui->lCopiando->setText("Copiando... ("+QString::number(porc)+"%)");
//    }else{
//        ui->lCopiando->setVisible(false);
//    }
//}


void ApkInfoView::ApkView()
{

    if(url.isEmpty()){

        this->setEnabled(false);
        this->close();

    }else{

        bop = new CBinOutputParser();
        connect(bop, SIGNAL(ImgActual(QPixmap)), this, SLOT(slot_ImagenAct(QPixmap)));

        AppInfo appaux;
        QFileInfo infoFile;
        infoFile=QFileInfo(url+"/"+file);

        if(infoFile.exists()){
            QProcess cmd;
            QString command;
            QString appDir="\"";

            QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                               "RepLink APK Manager", "RepLinkAPK");

            appDir+=settings.value("AppDirs/path").toString()+"/";

            command=appDir+"bins/aapt.exe\" dump badging \""+url+"/"+file+"\"";

            cmd.start(command.replace("/","\\") );
            //qDebug()<< command.replace("/","\\");
            cmd.waitForFinished(-1);

            QByteArray respose=cmd.readAllStandardOutput();
            QString result=QString().fromStdString(respose.toStdString());
            this->log=result;
            QStringList lines=result.split("\r\n");

            appaux=bop->binOutputParser(lines,url+"/"+file, infoFile);

            if(!appaux.ObtenerId().isEmpty()){
                this->show();
                ui->eDensities->setText(appaux.ObtenerDensities().join(" "));
                ui->eID->setText(appaux.ObtenerId());
                ui->eName->setText(appaux.ObtenerName());
                ui->eScreen->setText(appaux.ObtenerScreens().join(" "));
                ui->eVersion->setText(appaux.ObtenerLatestVersion());
                ui->eMinSDK->setText(appaux.ObtenerSdkVersion());
                ui->eMinAndroid->setText(AndroidVersion(appaux.ObtenerSdkVersion()));
                ui->eTargetSDK->setText(appaux.ObtenerTargetSdkVersion());
                ui->eTargetAndroid->setText(AndroidVersion(appaux.ObtenerTargetSdkVersion()));
                ui->eVersionCode->setText(appaux.ObtenerVersionCode());
                ui->tPermisions->setPlainText(appaux.ObtenerPermisions().join("\r\n"));
                ui->tFeatures->setPlainText(appaux.ObtenerFeatures().join("\r\n"));
                ui->eCurrent->setText(file);
                ui->lSize->setText("Size: "+appaux.ObtenerSize());


                QString newName=appaux.ObtenerName().replace(" ","_")+"_"+appaux.ObtenerLatestVersion()+"."+appaux.ObtenerVersionCode()+".apk";
                newName.replace("Á","A").replace("É","E").replace("Í","I").replace("Ó","O").replace("Ú","U");
                newName.replace("á","a").replace("é","e").replace("í","i").replace("ú","u").replace("ó","o");
                newName.replace(",","");
                newName.replace(":","");
                newName.replace("?","");
                newName.replace("¿","");
                newName.replace("!","");
                newName.replace("¡","");
                newName.replace("&","");
                newName.replace("©","");
                newName.replace("®","");
                newName.replace("~","");
                newName.replace("ñ","nn");
                newName.replace("__","_");
                newName.replace("/","-");



                ui->eNewname->setText(newName);

                UpdateButtons(newName);


                bop->disconnect();
                delete bop;

            }else{
                QMessageBox::warning(this, "Error", "Corrupt App or Not well renamed \""+ url+"/"+file+"\"", QMessageBox::Ok);
                ui->lIcon->setEnabled(false);
            }

        }
    }

}

QString ApkInfoView::AndroidVersion(QString sdk)
{
    int num=sdk.toInt();
    return DatAccess().SelectAndroidVerFromSdk(num,true);


}

void ApkInfoView::UpdateButtons(QString newName)
{
    url.replace("\\","/");
    QFileInfo infoFile;
    if(!direct){
        repository.replace("\\","/");


        infoFile= QFileInfo(repository+"/"+file);

        if(url!=repository && !infoFile.exists() && repository!=""){
            ui->bCopyToRepo->setEnabled(true);
        }else{
            ui->bCopyToRepo->setEnabled(false);
        }



        if(url==repository && repository!=""){
            QStringList used= xmlM->UsedAPKs(repository+"/replink.xml");
            if(used.indexOf(file)!=-1){
                inXML=true;
                ui->bSendProcess->setEnabled(false);
            }else{
                inXML=false;
                ui->bSendProcess->setEnabled(true);
            }
        }
    }else{
        QDir currentDir = QDir(url);
        QString  fileName = "*.apk";
        QStringList temp= currentDir.entryList(QStringList(fileName), QDir::Files | QDir::NoSymLinks, QDir::NoSort);
        int index= temp.indexOf(file);

        ui->label->setText(QString::number(index+1)+"/"+QString::number(temp.length()));

        if(index==0 && temp.length()==1){
            ui->tbNext->setEnabled(false);
            ui->tbNext->setToolTip("");
            ui->tbPrev->setEnabled(false);
            ui->tbPrev->setToolTip("");
        }else if(index==0 && temp.length()>0){
            ui->tbNext->setEnabled(true);
            ui->tbNext->setToolTip(temp.at(0));
            ui->tbPrev->setEnabled(false);
            ui->tbPrev->setToolTip("");
        }else if(index==temp.length()-1 && temp.length()>0){
            ui->tbNext->setEnabled(false);
            ui->tbNext->setToolTip("");
            ui->tbPrev->setEnabled(true);
            ui->tbPrev->setToolTip(temp.at(temp.length()-2));
        }else{
            ui->tbNext->setEnabled(true);
            ui->tbNext->setToolTip(temp.at(index+1));
            ui->tbPrev->setEnabled(true);
            ui->tbPrev->setToolTip(temp.at(index-1));
        }
    }

    infoFile= QFileInfo(url+"/"+newName);
    if(file!=newName && !infoFile.exists()){
        ui->bRename->setEnabled(true);
    }else{
        ui->bRename->setEnabled(false);
    }

    //qDebug() << inXML;
}

void ApkInfoView::Clear()
{
    ui->eDensities->setText("");
    ui->eID->setText("");
    ui->eName->setText("");
    ui->eScreen->setText("");
    ui->eVersion->setText("");
    ui->eMinSDK->setText("");
    ui->eMinAndroid->setText("");
    ui->eTargetSDK->setText("");
    ui->eTargetAndroid->setText("");
    ui->eVersionCode->setText("");
    ui->tPermisions->setPlainText("");
    ui->tFeatures->setPlainText("");
    ui->eCurrent->setText("");
    ui->lSize->setText("Size: ");
    ui->eNewname->setText("");
}


void ApkInfoView::on_bRename_clicked()
{
    if(inXML){
        xmlM->UpdateApkName(repository+"/replink.xml", file, ui->eNewname->text());
    }

    if(url!=repository){
        QFile f(QDir::toNativeSeparators(url +"/"+file));
        f.rename(QDir::toNativeSeparators(url +"/"+ui->eNewname->text()));
    }else{
        QFile f(QDir::toNativeSeparators(repository +"/"+file));
        f.rename(QDir::toNativeSeparators(repository +"/"+ui->eNewname->text()));
    }

    file=ui->eNewname->text();
    ui->eCurrent->setText(file);

    QString size= QString::number(QFile(url +"/"+file).size());


    emit UpdatePkgInfo(PkgInfo(ui->eID->text(),ui->eVersion->text(),ui->eVersionCode->text(), ui->eCurrent->text(), ui->eMinSDK->text(), size, "",""));
    UpdateButtons(ui->eNewname->text());

}

void ApkInfoView::on_bCopyToRepo_clicked()
{
    xfile= new QFile(url+"/"+file);
    ui->lCopiando->setVisible(true);
    ui->lCopiando->repaint();

    xfile->copy(repository+"/"+file);


    url=repository;

    UpdateButtons(ui->eNewname->text());
    ui->lCopiando->setVisible(false);
}

void ApkInfoView::on_bSendProcess_clicked()
{
    QStringList files;
    files.append(file);

    emit Files(files);
    emit finished(0);
    this->close();
}

void ApkInfoView::on_tbNext_clicked()
{
    Clear();
    PrepareIndex(1);
    ApkView();
}

void ApkInfoView::on_tbPrev_clicked()
{
    Clear();
    PrepareIndex(-1);
    ApkView();
}


void ApkInfoView::PrepareIndex(int value){
    QDir currentDir = QDir(url);
    QString  fileName = "*.apk";
    QStringList temp= currentDir.entryList(QStringList(fileName), QDir::Files | QDir::NoSymLinks, QDir::NoSort);
    int index=temp.indexOf(file);
    index+=value;
    if(index>-1 && index<temp.length()){
        file=temp.at(index);
    }
}

void ApkInfoView::on_tbLog_clicked()
{
QString fileName=url+"/"+ ui->eID->text() +"_log.txt";
QFile file(QDir::toNativeSeparators(fileName));
if (!file.open(QFile::WriteOnly | QFile::Text)) {
    QMessageBox::warning(this, tr("Categories"),
                         tr("Cannot write file %1:\n%2.")
                         .arg(fileName)
                         .arg(file.errorString()));
}else {
    QTextStream out(&file);
    out << this->log;

    QString command="notepad \""+fileName+"\"";
    file.close();

    QProcess cmd;
    cmd.start(command.replace("/","\\") );
    //qDebug()<< command.replace("/","\\");
    cmd.waitForFinished(-1);
}


}
