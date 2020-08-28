#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    countHilo= new CCountHilo();
    connect(countHilo, SIGNAL(AppPerCat(QStringList)), this, SLOT(slot_AppPerCat(QStringList)));



    updateHilo= new CUpdateHilo();
    connect(updateHilo, SIGNAL(UpdateRepoInfo(QString,QString,CXMLRepo)), this, SLOT(slot_UpdateRepoInfo(QString,QString,CXMLRepo)));
    previewHilo=new CPreviewHilo();
    connect(previewHilo, SIGNAL(Preview(ApkPreview)), this, SLOT(slot_Preview(ApkPreview)));

    backupHilo= new CBackupHilo();


    ui->mainToolBar->setVisible(false);
    ui->actionCancel_BackUp->setVisible(false);

    xmlM= NULL;
    repository="";
    salida="";
    remoteFilesList=false;

    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       "RepLink APK Manager", "RepLinkAPK");
    settings.value("Options/rename").toString()=="1"?ui->actionAuto_Rename_APKs->setChecked(true):ui->actionAuto_Rename_APKs->setChecked(false);
    settings.value("Options/update").toString()=="1"?ui->actionAuto_Update->setChecked(true):ui->actionAuto_Update->setChecked(false);
    settings.value("Options/preview").toString()=="1"?ui->actionPreviews->setChecked(true):ui->actionPreviews->setChecked(false);
    settings.value("Options/apkbackup").toString()=="1"?ui->actionBackUp_APKs_Too->setChecked(true):ui->actionBackUp_APKs_Too->setChecked(false);


    CheckSig();

    progIco.addFile(":/replinkAPKico.png");
    this->setWindowIcon(progIco);

    procStatus= new processing(this);
    procStatus->hide();

    if(QSysInfo::windowsVersion()>QSysInfo::WV_VISTA){
        taskbarButton = new QWinTaskbarButton(this);
        taskbarButton->setWindow(this->windowHandle());


        taskbarProgress = taskbarButton->progress();
        // taskbarButton->setOverlayIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        // taskbarProgress->show();

        // taskbarProgress->setValue(75);
    }

    movie = new QMovie(":/img/win-loading.gif");
    movie->setScaledSize(ui->lLoading->size());
    ui->lLoading->setMovie(movie);

    ui->lLoading->hide();
    ui->lWarning->hide();
    ui->lCount->hide();

    connect(this, SIGNAL(Progress(int)), procStatus, SLOT(slot_UpdatePorc(int)));
    connect(this, SIGNAL(Image(QPixmap)), procStatus, SLOT(slot_UpdateInfo(QPixmap)));
    procStatus->setWindowFlags(Qt::FramelessWindowHint);
    procStatus->setAttribute(Qt::WA_TranslucentBackground);

    ui->lwNewApps->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->lwNewApps, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(PopUp(QPoint)));

    qRegisterMetaType<CXMLAplication>("CXMLAplication");
    qRegisterMetaType<CXMLRepo>("CXMLRepo");
    qRegisterMetaType<ApkPreview>("ApkPreview");


    LoadMYRepos();

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::slot_ImagenAct(QPixmap pixmap)
{
    emit Image(pixmap);
}

void MainWindow::slot_Buzon(QString msg, int type)
{
    if(type==0){
        ui->statusBar->showMessage(ui->statusBar->currentMessage() + "  " + msg,3000);
    }else if(type==1){
        if(ui->statusBar->currentMessage().indexOf("JDK")!=-1 && msg.indexOf("Document Parsed for")!=-1){
            ui->statusBar->showMessage("Error: Instale JDK v1.8.0 <- "+msg);
        }else{
            ui->statusBar->showMessage(msg);
        }
    }
}

void MainWindow::slot_NewCat(QString cat)
{
    int index=ui->lwNewApps->currentRow();
    files.ObtenerPorPosMem(index+1)->SetCategory(cat);
    QIcon check;
    check.addFile(":/img/check");

    ui->lwNewApps->currentItem()->setToolTip("Category: "+cat);
    ui->lwNewApps->currentItem()->setIcon(check);
    //delete cc;
}

void MainWindow::slot_RemotesNewCat(QString cat)
{
    CatRemoteFiles(remotes, cat);
    UpdateAppsList();
}

void MainWindow::slot_Recent()
{
    for (int i = 1; i <= repos.Longitud(); ++i) {
        if(repos.ObtenerPorPos(i).ObtenerName()==hoveredAct){
            remoteFilesList=false;
            repository=repos.ObtenerPorPos(i).ObtenerDir();
            OpenRepository();
            break;
        }
    }
}

void MainWindow::slot_SaveHovered()
{
    hoveredAct= ui->menuOpen_recent_repo->activeAction()->text();
}

void MainWindow::slot_RepoManager(int)
{
    LoadMYRepos();
    repoM->disconnect();
    delete repoM;
}

void MainWindow::slot_RemoteFiles(QStringList files)
{
    remoteFilesList=true;
    this->files= ListaSE<NewAPKFile>();
    for (int i = 0; i < files.length(); ++i) {
        this->files.AdicionarCabeza(NewAPKFile(files.at(i)));
    }
}


void MainWindow::slot_UpdateRepoInfo(QString cantApps, QString cantPkg, CXMLRepo xmlRepo)
{
    QString ver=xmlRepo.ObtenerVersion();
    QString sig=xmlRepo.ObtenerPubkey()!=""? "Yes": "No";
    ui->lRepoIcon->setToolTip(xmlRepo.ObtenerName());
    QString timeS=xmlRepo.ObtenerTimeStamp();


    QString tooltip= sig=="No"? "The repo it's not signed!":"";

    ui->labelSigned->setText(sig);
    ui->labelVer->setText(ver);

    if(timeS.length()==14){
        QString date=timeS.mid(6,2)+"/"+timeS.mid(4,2)+"/"+timeS.mid(0,4)+"  At  "+timeS.mid(8,2)+":"+timeS.mid(10,2)+":"+timeS.mid(12,2);
        ui->labelTime->setText(date);
    }else{
        ui->labelTime->setText(timeS);
        tooltip+=QString(tooltip!=""?"\n":"")+"This repository it wasn't created by RepLink APK Manager or was manually modified.";
    }

    double cantA;
    double cantP;
    QString cApk=QString::number(cantAPK);

    if(qIsNaN(cantApps.toDouble()) || qIsNaN(cantPkg.toDouble())){
        cantApps="Error";
        cantPkg="Error";
    }else{
        cantA=cantApps.toDouble();
        cantP=cantPkg.toDouble();

        if(cantAPK!=cantP){
            tooltip+=QString(tooltip!=""?"\n":"")+"The amount of APKs difiers of No. Packages.";
        }
        if(cantA>cantP){
            tooltip+=QString(tooltip!=""?"\n":"")+"The are missing Packages!";
        }
    }

    if(tooltip!=""){
        ui->lWarning->setToolTip(tooltip);
        ui->lWarning->show();
    }else{
        ui->lWarning->hide();
    }

    ui->labelApp->setText(cantApps);
    ui->labelPkg->setText(cantPkg);
    ui->labelAkp->setText(cApk);

    if(countHilo->isRunning()){
        ui->statusBar->showMessage(ui->statusBar->currentMessage().replace("Updating repository info...",""));
    }else{
        ui->lLoading->hide();
        ui->statusBar->showMessage("");
    }
}

void MainWindow::slot_AppPerCat(QStringList appPerCat)
{
    if(updateHilo->isRunning()){
        ui->statusBar->showMessage(ui->statusBar->currentMessage().replace("Counting...",""));
    }else{
        ui->lLoading->hide();
        ui->statusBar->showMessage("");
    }

    QString msg="";
    int rest;

    for (int i = 0; i < appPerCat.length(); ++i) {
        rest=i+1;

        if(rest%3==0){
            msg+="["+appPerCat.at(i)+"]\n";
        }else if(rest!= appPerCat.length()){
            msg+="["+appPerCat.at(i)+"]  ";
        }else{
            msg+="["+appPerCat.at(i)+"]";
        }
    }

    QMessageBox::information(this,"Count", msg);

}

void MainWindow::slot_Preview(ApkPreview prev)
{
    for (int i = 0; i < ui->lwNewApps->count(); ++i) {
        if(ui->lwNewApps->item(i)->text()==prev.ObtenerApkName()){
            QIcon check=QIcon();
            check.addPixmap(prev.ObtenerPixmap());
            ui->lwNewApps->item(i)->setIcon(check);
            ui->lwNewApps->setIconSize(QSize(28,28));
            ui->lwNewApps->item(i)->setToolTip(prev.toString());
            if(prev.toString().indexOf("Recomended: ")!=-1){
                ui->lwNewApps->item(i)->setBackground(QColor(150,50,0,50));
                ui->lwNewApps->item(i)->setData(6,QVariant(prev.RecomendedName()));
            }
            break;
        }
    }
}


void MainWindow::slot_Porc(int porc)
{
    ui->statusBar->showMessage("Loading... "+QString::number(porc)+"%");
}

void MainWindow::slot_BackupPorc(int porc, QString category, QString apkname)
{
    ui->statusBar->showMessage("Backing Up... "+QString::number(porc)+"%  [ "+category+" -> "+apkname+" ]");
}

void MainWindow::slot_GenerateJsonPorc(int porc)
{
    ui->statusBar->showMessage("Generating JSON Index v1... "+QString::number(porc)+"%");
}

void MainWindow::slot_ToHtmlPorc(int porc, QString app, QString apkname)
{
    ui->statusBar->showMessage("Making Web Links... "+QString::number(porc)+"%  [ "+app+" -> "+apkname+" ]");
}


void MainWindow::PopUp(const QPoint &pos)
{
    if(ui->lwNewApps->selectedItems().count()>0 && ui->lwNewApps->indexAt(pos).isValid()){
        QPoint globalPos= ui->lwNewApps->mapToGlobal(pos);
        QMenu popupMenu;
        if(ui->lwNewApps->indexAt(pos).data(6).toString()!=""){
            popupMenu.addAction(ui->actionRename);
        }
        popupMenu.addAction(ui->actionRemove);
        popupMenu.exec(globalPos);
    }
}

void MainWindow::CatRemoteFiles(QStringList files, QString cat)
{
    remoteFilesList=true;
    for (int i = 0; i < files.length(); ++i) {
        this->files.AdicionarCabeza(NewAPKFile(files.at(i),cat));
    }
}

void MainWindow::DestroyView(int result)
{
    Q_UNUSED(result);
    if(ui->actionAuto_Update->isChecked()){
        UpdateAppsList();
    }
    appv->disconnect();
    //delete appv;
}

void MainWindow::DestroyFilesView(int result)
{
    Q_UNUSED(result);
    if(ui->actionAuto_Update->isChecked()){
        UpdateAppsList();
    }
    apkFilesV->disconnect();
}



void MainWindow::DestroyReport(int result)
{
    Q_UNUSED(result);
    if(ui->actionAuto_Update->isChecked()){
        UpdateAppsList();
    }
    apkr->disconnect();
    //delete apkr;
}

void MainWindow::DestroyViewInfo(int result)
{
    Q_UNUSED(result);
    if(ui->actionAuto_Update->isChecked()){
        UpdateAppsList();
    }
    apkiv->disconnect();
    //delete apkiv;

}

void MainWindow::DestroyConfig(int result)
{
    Q_UNUSED(result);
    if(ui->actionAuto_Update->isChecked()){
        CheckSig();
    }
    //delete apkiv;
}

void MainWindow::DestroyHilo()
{
    emit Progress(100);

    salida=xmlM->XMLWrite(xmlFilePath, domDocument);
    ui->statusBar->showMessage(salida, 5000);
    UpdateAppsList();

    emit Image(QPixmap());

    procStatus->hide();
    ui->menuBar->setEnabled(true);
    ui->lblock->move(this->width(),0);


    procHilo->disconnect();
    procHilo->Cerrar();
    delete procHilo;


}



void MainWindow::DestroyHiloBackup()
{
    ui->actionBackUp_Repo->setEnabled(true);
    ui->actionBackUp_Repo->setVisible(true);
    ui->actionCancel_BackUp->setVisible(false);
    ui->statusBar->showMessage("");
    QMessageBox::information(this,"BackUp Finish", "The repository backup has been finished.");
}

void MainWindow::DestroyHiloGenerateJson()
{
    ui->actionGenerate_JSON->setEnabled(true);
    ui->statusBar->showMessage("");
    QMessageBox::information(this,"Generate JSON Finish", "The Generation of JSON Index v1 has been finished.");
}

void MainWindow::DestroyHiloToHtml()
{
    ui->actionTo_Web_Links->setEnabled(true);
    ui->statusBar->showMessage("");
    QMessageBox::information(this,"To Web Links Finish", "The repository has been exported into a web!");
}



void MainWindow::UpdateAppsList()
{
    if(repository!=""){
        movie->start();
        ui->lLoading->show();

        if(countHilo->isRunning()){
            ui->statusBar->showMessage(ui->statusBar->currentMessage() +" Updating repository info...");
        }else{
            ui->statusBar->showMessage("Updating repository info...");
            ui->lLoading->repaint();
        }

        ui->rbDefault->setChecked(false);

        QDir currentDir = QDir(repository);
        QFileInfo infoFile;

        QString  fileName = "*.apk";
        QStringList temp= currentDir.entryList(QStringList(fileName), QDir::Files | QDir::NoSymLinks, QDir::Reversed);

        if(!remoteFilesList){
            files= ListaSE<NewAPKFile>();
            for (int i = 0; i < temp.length(); ++i) {
                files.AdicionarCola(NewAPKFile(temp.at(i)));
            }
            ui->bClearSendedApps->setEnabled(false);
        }else{
            ui->bClearSendedApps->setEnabled(true);
        }

        cantAPK=temp.length();

        ListaSE<NewAPKFile> newFiles;
        QString ruta="";

        QString timeStamp=xmlM->XMLTimeStamp(xmlFilePath);
        double repoTS=qIsNaN(timeStamp.toDouble())?qInf():timeStamp.toDouble();
        double fileTS;
        if(!qIsInf(repoTS)){

            //////////////////

            ui->actionApps_in_repository->setEnabled(true);
            ui->actionApps_per_Categoy_count->setEnabled(true);
            ui->actionOpen_APK_files->setEnabled(true);
            ui->actionClose_repo->setEnabled(true);
            ui->actionBackUp_Repo->setEnabled(true);
            ui->actionTo_Web_Links->setEnabled(true);
            ui->actionSummary_like_Category->setEnabled(true);
            ui->actionApk_report->setEnabled(true);
            ui->actionMerge_repo->setEnabled(true);
            ui->actionSubstract_repo->setEnabled(true);
            ui->actionSign_Repo->setEnabled(true);
            ui->actionGenerate_JSON->setEnabled(true);
            ui->actionAll_Packages->setEnabled(true);
            ui->lwNewApps->clear();

            QListWidgetItem *newItem;

            if(!remoteFilesList){

                bool flag=false;
                for (int var = 1; var <= files.Longitud(); ++var) {
                    ruta=repository+"/"+files.ObtenerPorPos(var).ObtenerAPKName();
                    infoFile=QFileInfo(ruta);
                    fileTS=infoFile.created().toString("yyyyMMddhhmmss").toDouble();
                    if(fileTS>repoTS){
                        newItem = new QListWidgetItem();

                        newFiles.AdicionarCola(files.ObtenerPorPos(var));
                        newItem->setSizeHint(QSize(200, 32));


                        newItem->setText(files.ObtenerPorPos(var).ObtenerAPKName());
                        newItem->setToolTip(files.ObtenerPorPos(var).toString());
                        ui->lwNewApps->addItem(newItem);


                        flag=true;
                    }
                }

                if(flag){
                    ui->gbNew->setEnabled(true);
                    ui->lwNewApps->setEnabled(true);
                    ui->cbCategories->setEnabled(true);
                    flag=false;
                }else{
                    ui->cbCategories->setEnabled(false);
                    ui->gbNew->setEnabled(false);
                    ui->lwNewApps->setEnabled(false);
                }

                files=newFiles;
            }else{
                for (int j = 1; j <= files.Longitud(); ++j) {
                    newItem = new QListWidgetItem();

                    newItem->setToolTip(files.ObtenerPorPos(j).toString());
                    newItem->setText(files.ObtenerPorPos(j).ObtenerAPKName());

                    if(files.ObtenerPorPos(j).ObtenerCategory()!="Uncategorized"){
                        QIcon check=QIcon();
                        check.addFile(":/img/check");

                        newItem->setToolTip("Category: "+files.ObtenerPorPos(j).ObtenerCategory());

                        newItem->setIcon(check);

                    }
                    newItem->setSizeHint(QSize(200, 32));
                    ui->lwNewApps->setIconSize(QSize(28,28));
                    ui->lwNewApps->addItem(newItem);
                }

                ui->gbNew->setEnabled(true);
                ui->lwNewApps->setEnabled(true);
                ui->cbCategories->setEnabled(true);
            }

            if(ui->actionAuto_Rename_APKs->isChecked()){

                QString newName;
                for (int x = 1; x <= files.Longitud(); ++x) {
                    QString actualName=files.ObtenerPorPos(x).ObtenerAPKName();
                    newName=CheckName(actualName);
                    if(actualName!=newName){
                        QFile f(QDir::toNativeSeparators(repository +"/"+files.ObtenerPorPos(x).ObtenerAPKName()));
                        if(f.rename(QDir::toNativeSeparators(repository +"/"+newName))){
                            ui->lwNewApps->item(x-1)->setText(newName);
                        }
                        files.ObtenerPorPosMem(x)->SetAPKName(newName);
                    }
                }
            }

            if(ui->lwNewApps->count()){
                ui->gbAppProcess->setEnabled(true);
                ui->bProcess->setEnabled(true);
                ui->lCount->show();
                ui->lCount->setText(QString::number(ui->lwNewApps->count()));
            }else{
                ui->gbAppProcess->setEnabled(false);
                ui->lCount->hide();
            }

            updateHilo->SetParams(repository);
            updateHilo->start();
            if(ui->actionPreviews->isChecked()){
                previewHilo->SetParams(repository, files);
                previewHilo->start();
            }
        }else {
            CloseRepo();
        }

    }
}

void MainWindow::CloseRepo()
{
    on_actionCancel_BackUp_triggered();
    repository="";
    salida="";
    remoteFilesList=false;
    files= ListaSE<NewAPKFile>();
    cantAPK=0;
    ui->repourl->setText("");
    ui->gbAppProcess->setEnabled(false);
    ui->labelAkp->setText("");
    ui->labelApp->setText("");
    ui->labelPkg->setText("");
    ui->labelSigned->setText("");
    ui->labelVer->setText("");
    ui->labelTime->setText("");
    ui->lRepoIcon->setToolTip("");
    ui->cbCategories->clear();
    ui->lwNewApps->clear();
    ui->gbInfo->setEnabled(false);
    ui->gbNew->setEnabled(false);
    ui->lwNewApps->setEnabled(false);


    ui->actionApps_in_repository->setEnabled(false);
    ui->actionApps_per_Categoy_count->setEnabled(false);
    ui->actionOpen_APK_files->setEnabled(false);
    ui->actionClose_repo->setEnabled(false);
    ui->actionApk_report->setEnabled(false);
    ui->actionSign_Repo->setEnabled(false);
    ui->actionGenerate_JSON->setEnabled(false);
    ui->actionMerge_repo->setEnabled(false);
    ui->actionSubstract_repo->setEnabled(false);
    ui->actionAll_Packages->setEnabled(false);
    ui->actionBackUp_Repo->setEnabled(false);
    ui->actionTo_Web_Links->setEnabled(false);
    ui->actionSummary_like_Category->setEnabled(false);

    ui->bActApks->setEnabled(false);

    QIcon ico;
    ico.addFile(":/img/info.png");
    ui->lRepoIcon->setPixmap(ico.pixmap(ui->lRepoIcon->size()));

    if(xmlM!=NULL){
        xmlM->disconnect();
        delete xmlM;
        xmlM= NULL;
    }
}

void MainWindow::CheckSig()
{
    bop=new CBinOutputParser();
    KeystoreInfo kInfo=bop->binOutputParser();
    QIcon ico;

    if(kInfo.Check()){
        ui->lKeyStore->setToolTip(kInfo.toString());
        ico.addFile(":/img/signature_ok.png");
        ui->lKeyStore->setIcon(ico);
    }else{
        ui->lKeyStore->setToolTip("No Key Store, Java JDK or incorrect configuration for Signature\nSet a correct configuration and hit this buttom!");
        ico.addFile(":/img/signature_no.png");
        ui->lKeyStore->setIcon(ico);
    }
    delete bop;
}

QString MainWindow::CheckName(QString name)
{
    name.replace("Á","A").replace("É","E").replace("Í","I").replace("Ó","O").replace("Ú","U");
    name.replace("á","a").replace("é","e").replace("í","i").replace("ú","u").replace("ó","o");
    name.replace(",","");
    name.replace(":","");
    name.replace("?","");
    name.replace("¿","");
    name.replace("!","");
    name.replace("¡","");
    name.replace("&","");
    name.replace("©","");
    name.replace("®","");
    name.replace("~","");
    name.replace("ñ","nn");
    name.replace(" ","_");
    name.replace("__","_");
    name.replace("/","-");

    return name;
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Escape && !procStatus->isVisible()) {
        // ui->menuRepository->show();
        ui->menuOpen_recent_repo->showNormal();
        int x=this->x()+this->width()/2 - ui->menuOpen_recent_repo->width()/2;
        int y=this->y()+this->height()/2 - ui->menuOpen_recent_repo->height()/2;
        ui->menuOpen_recent_repo->move(x,y);
    }
}

void MainWindow::on_bActApks_clicked()
{
    UpdateAppsList();
}

void MainWindow::on_bProcess_clicked()
{
    ui->cbCategories->setEnabled(false);

    QString url=xmlFilePath;
    salida="";
    int porciento=0;

    QString error="";
    QFile* file = new QFile(url);

    if (!file->open(QFile::ReadOnly | QFile::Text)) {
        error=QString("Can\'t read file %1:\n%2.").arg(xmlFilePath).arg(file->errorString());
        //std::cout<< error.toStdString()<< std::endl;
        ui->statusBar->showMessage(error);
        delete file;
    }else{
        QString errorStr;
        int errorLine;
        int errorColumn;

        //ui->statusBar->showMessage("Processing...(0%)");
        emit Progress(porciento);

        if (!domDocument.setContent(file, true, &errorStr, &errorLine, &errorColumn)) {
            error=QString("Error de Parseo en la linea %1, columna %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
            //std::cout<< error.toStdString()<< std::endl;
            ui->statusBar->showMessage(error);
            delete file;
        } else {

            delete file;

            procHilo= new CProcHilo();
            if(QSysInfo::windowsVersion()>QSysInfo::WV_VISTA){
                connect(procHilo, SIGNAL(Porciento(int)), taskbarProgress, SLOT(setValue(int)));
            }
            connect(procHilo, SIGNAL(Porciento(int)), procStatus, SLOT(slot_UpdatePorc(int) ));
            connect(procHilo, SIGNAL(Imagen(QPixmap)), this, SLOT(slot_ImagenAct(QPixmap)));
            connect(procHilo, SIGNAL(Mensaje(QString, int)), this, SLOT(slot_Buzon(QString, int)));
            connect(procHilo, SIGNAL(finished()), this, SLOT(DestroyHilo()));

            QString cat=ui->cbCategories->currentText().trimmed().isEmpty()?"Unknown":ui->cbCategories->currentText().trimmed();

            procHilo->SetParametros(xmlM, repository,files,domDocument, remoteFilesList, ui->rbDefault->isChecked(),cat);

            procStatus->show();
            int x=this->width()/2 - procStatus->width()/2;
            int y=this->height()/2 - procStatus->height()/2;
            procStatus->move(x,y);
            ui->menuBar->setEnabled(false);
            ui->lblock->move(0,0);

            procHilo->start();
        }
    }
    ui->cbCategories->setEnabled(true);
    remoteFilesList=false;

}


void MainWindow::on_actionOpen_repository_2_triggered()
{
    repository="";

    QFileDialog abrir;
    QString ruta= DatAccess().SelectRecent(0);
    repository= abrir.getExistingDirectory(this, tr("Abrir Ruta"), ruta);

    OpenRepository();

}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}




void MainWindow::on_rbDefault_clicked(bool checked)
{
    if(checked){
        ui->cbCategories->setEnabled(true);

        for (int i = 0; i < ui->lwNewApps->count(); ++i) {
            files.ObtenerPorPosMem(i+1)->SetCategory(ui->cbCategories->currentText());

            QIcon check;
            check.addFile(":/img/check");

            ui->lwNewApps->item(i)->setToolTip("Category: "+files.ObtenerPorPos(i+1).ObtenerCategory());
            ui->lwNewApps->item(i)->setIcon(check);
        }
    }
}

void MainWindow::on_actionApps_in_repository_triggered()
{

    appv= new AppsView(repository,xmlM,repoIcon);
    appv->setWindowIcon(progIco);
    connect(appv, SIGNAL(finished(int)), this, SLOT(DestroyView(int)) );
    appv->show();

}

void MainWindow::on_actionApk_report_triggered()
{

    apkr= new ApkReport(repository,xmlM,repoIcon);
    apkr->setWindowIcon(progIco);
    connect(apkr, SIGNAL(finished(int)), this, SLOT(DestroyReport(int)) );
    connect(apkr, SIGNAL(Files(QStringList)), this, SLOT(slot_RemoteFiles(QStringList)));
    apkr->show();

}

void MainWindow::on_actionAbout_triggered()
{
    about= new CAbout();
    about->setWindowIcon(progIco);
    about->show();
}

void MainWindow::on_actionApk_Info_View_triggered()
{

    apkiv= new ApkInfoView(repository,xmlM);
    apkiv->setWindowIcon(progIco);
    connect(apkiv, SIGNAL(finished(int)), this, SLOT(DestroyViewInfo(int)) );
    connect(apkiv, SIGNAL(Files(QStringList)), this, SLOT(slot_RemoteFiles(QStringList)));
    //apkiv->show();

}

void MainWindow::on_bClearSendedApps_clicked()
{

    remoteFilesList=false;
    UpdateAppsList();

}

void MainWindow::on_lwNewApps_itemDoubleClicked(QListWidgetItem *item)
{

    if(!ui->rbDefault->isChecked()){
        cc= new CatChooser(item->text(),repository);
        cc->setWindowIcon(progIco);
        connect(cc, SIGNAL(Cat(QString)), this, SLOT(slot_NewCat(QString)));
        cc->show();
    }
}


void MainWindow::on_cbCategories_currentIndexChanged(const QString &arg1)
{
    if(ui->rbDefault->isChecked()){
        for (int i = 0; i < ui->lwNewApps->count(); ++i) {
            files.ObtenerPorPosMem(i+1)->SetCategory(arg1);

            QIcon check;
            check.addFile(":/img/check");

            ui->lwNewApps->item(i)->setToolTip("Category: "+files.ObtenerPorPos(i+1).ObtenerCategory());
            ui->lwNewApps->item(i)->setIcon(check);
        }
    }
}

void MainWindow::LoadMYRepos()
{
    ui->menuOpen_recent_repo->clear();

    repos = DatAccess().SelectRepoFromId(-1);
    QList<QAction*> acciones;
    QAction *recent;
    for (int i = 1; i <= repos.Longitud(); ++i) {

        recent= new QAction(repos.ObtenerPorPos(i).ObtenerName(), ui->menuOpen_recent_repo);
        connect(recent, SIGNAL(triggered()), this, SLOT(slot_Recent()));
        connect(recent, SIGNAL(hovered()), this, SLOT(slot_SaveHovered()));
        acciones.append(recent);
    }

    ui->menuOpen_recent_repo->addActions(acciones);

}

void MainWindow::OpenRepository()
{

    ui->statusBar->showMessage("Opening repository...");


    if(!repository.isEmpty()){
        DatAccess().UpdateRecent(repository,0);
        xmlFilePath=repository+"/replink.xml";
        ui->repourl->setText(repository);

        QString error="";
        QFile* file = new QFile(xmlFilePath);

        if (!file->open(QFile::ReadOnly | QFile::Text)) {
            error=QString("Can\'t read file %1:\n%2.").arg(xmlFilePath).arg(file->errorString());
            //std::cout<< error.toStdString()<< std::endl;
            delete file;
            CloseRepo();
            ui->statusBar->showMessage(error);
        }else{
            QPixmap repoPix;
            QString rutaIcon=repository+"/icons/"+xmlM->XMLRepoIcon(xmlFilePath);

            if(QFileInfo(rutaIcon).exists()){
                repoIcon = QIcon();
                repoPix.load(rutaIcon);
                repoIcon.addPixmap(repoPix);
            }else{
                repoIcon.addFile(":/img/info.png");
            }

            QString cat=repository+"/categories.txt";
            file = new QFile(cat);

            ui->lRepoIcon->setPixmap(repoIcon.pixmap(ui->lRepoIcon->size()));

            if (!file->open(QFile::ReadOnly | QFile::Text)) {
                error=QString("Can\'t read file %1:  \n%2.").arg(cat).arg(file->errorString());
                //std::cout<< error.toStdString()<< std::endl;
                delete file;
                CloseRepo();
                ui->statusBar->showMessage(error);
            }else {


                QByteArray textFile=file->readAll();
                QString auxText=QString().fromStdString(textFile.toStdString());
                QStringList lines=auxText.split("\n");
                ui->cbCategories->clear();
                for (int var = 0; var < lines.length(); ++var) {
                    auxText=lines.at(var).trimmed();
                    if(!auxText.isEmpty()){
                        ui->cbCategories->addItem(auxText);
                    }
                }
                xmlM= new CXMLMethods();
                connect(xmlM, SIGNAL(Notice(QString,int)), this, SLOT(slot_Buzon(QString,int)));
                ui->gbInfo->setEnabled(true);
                ui->bActApks->setEnabled(true);
                delete file;
                UpdateAppsList();
            }
        }
    }else {
        ui->statusBar->showMessage("");
        CloseRepo();
    }
}

void MainWindow::on_actionRepositories_manager_triggered()
{

    repoM=new CRepoManager();
    repoM->setWindowIcon(progIco);
    connect(repoM, SIGNAL(finished(int)), this, SLOT(slot_RepoManager(int)));
    repoM->show();

}

void MainWindow::on_actionSign_Repo_triggered()
{

    if(!repository.isEmpty()){
        QMessageBox::StandardButton confirm;
        confirm = QMessageBox::question(this, "Singning...", "Please confirm the Repo Signing.", QMessageBox::Yes|QMessageBox::No);
        if(confirm == QMessageBox::Yes){
            xmlM->SignIndexXML(xmlFilePath);
            CXMLRepo xmlRepo=xmlM->XMLRepoInfo(xmlFilePath);
            QString sig=xmlRepo.ObtenerPubkey()!=""? "Yes": "No";
            ui->labelSigned->setText(sig);
            if(sig=="Yes"){
                QString toolt=ui->lWarning->toolTip();
                if(toolt=="The repo it's not signed!"){
                    ui->lWarning->setToolTip("");
                    ui->lWarning->hide();
                }else if(toolt!=""){
                    ui->lWarning->setToolTip(toolt.replace("The repo it's not signed!\n",""));
                }
            }
        }
    }


}

void MainWindow::on_actionAll_Packages_triggered()
{

    if(!repository.isEmpty()){
        QMessageBox::StandardButton confirm;
        confirm = QMessageBox::question(this, "Delete - All Packages", "Do you really want to delete All Packages?", QMessageBox::Yes|QMessageBox::No);
        if(confirm == QMessageBox::Yes){
            xmlM->DeleteAllPkg(xmlFilePath);
            UpdateAppsList();
        }
    }

}

void MainWindow::on_actionOpen_APK_files_triggered()
{

    QFileDialog abrir;
    QStringList manejadores = abrir.getOpenFileNames(
                this,
                "Select one or more APKs to open",
                DatAccess().SelectRecent(2),
                "Android Application (*.apk)");

    if(manejadores.length()>0){
        QStringList aux=manejadores.at(0).split("/");
        aux.removeAt(aux.length()-1);
        QString apksurl=aux.join("/");

        if(apksurl!=repository){
            DatAccess().UpdateRecent(apksurl,2);
            QMessageBox::StandardButton confirm;
            confirm = QMessageBox::question(this, "Copy to Repo", "These APK files aren't in the current Repository. Do you really want to add them?", QMessageBox::Yes|QMessageBox::No);
            if(confirm == QMessageBox::Yes){

                QString url;
                QString file;
                QFile *xfile;

                for (int j = 0; j < manejadores.length(); ++j) {
                    url=manejadores.at(j).toUtf8();
                    url.replace("\\","/");
                    aux= url.split("/");
                    file=aux.last();

                    xfile= new QFile(manejadores.at(j));
                    ui->statusBar->showMessage("Copy to Repo ("+ QString::number((j*100)/manejadores.length()) +"%)   Current file -> "+file);
                    xfile->copy(repository+"/"+file);
                    delete xfile;
                }

                ui->statusBar->showMessage("Copy to Repo -> Finished", 3000);

                remotes.clear();
                for (int i = 0; i < manejadores.length(); ++i) {
                    remotes.append(manejadores.at(i).split("/").last());
                }
                confirm = QMessageBox::question(this, "Default Category", "Do you want to add a default category for these APKs?", QMessageBox::Yes|QMessageBox::No);

                if(confirm == QMessageBox::Yes){
                    cc= new CatChooser("Selected APKs",repository);
                    connect(cc, SIGNAL(CatForAPKs(QString)), this, SLOT(slot_RemotesNewCat(QString)));
                    cc->setWindowIcon(progIco);
                    cc->show();
                }else{
                    CatRemoteFiles(remotes, "Uncategorized");
                    UpdateAppsList();
                }

            }
        }else{
            remotes.clear();
            for (int i = 0; i < manejadores.length(); ++i) {
                remotes.append(manejadores.at(i).split("/").last());
            }
            CatRemoteFiles(remotes, "Uncategorized");
            UpdateAppsList();
        }
    }

}

void MainWindow::on_toolButton_clicked()
{

    QString cat=repository+"/categories.txt";
    QFile *file = new QFile(cat);

    if (!file->open(QFile::ReadOnly | QFile::Text)) {
        QString error=QString("Can\'t read file %1:  \n%2.").arg(cat).arg(file->errorString());
        //std::cout<< error.toStdString()<< std::endl;
        delete file;
        CloseRepo();
        ui->statusBar->showMessage(error);
    }else {

        ui->cbCategories->clear();

        QByteArray textFile=file->readAll();
        QString auxText=QString().fromStdString(textFile.toStdString());
        QStringList lines=auxText.split("\n");
        ui->cbCategories->clear();
        for (int var = 0; var < lines.length(); ++var) {
            auxText=lines.at(var).trimmed();
            if(!auxText.isEmpty()){
                ui->cbCategories->addItem(auxText);
            }
        }
    }

}

void MainWindow::on_actionRemove_triggered()
{
    QListWidgetItem *item=ui->lwNewApps->currentItem();
    QString aux=item->text();
    QMessageBox::StandardButton confirm;
    confirm = QMessageBox::question(this, "Remove", "Are you sure you want to remove \""+ aux+"\" from this list?", QMessageBox::Yes|QMessageBox::No);
    if(confirm==QMessageBox::Yes){
        files.BorrarPorPos(ui->lwNewApps->currentRow()+1);
        ui->lwNewApps->takeItem(ui->lwNewApps->currentRow());
        ui->lCount->setText(QString::number(ui->lwNewApps->count()));
        if(ui->lwNewApps->count()==0){
            ui->gbAppProcess->setEnabled(false);
            ui->gbNew->setEnabled(false);
            ui->bClearSendedApps->setEnabled(false);
            remoteFilesList=false;
        }
    }
}

void MainWindow::on_actionMerge_repo_triggered()
{

    QFileDialog abrir;
    DatAccess db;
    QString manejador= abrir.getOpenFileName(this, tr("Abrir Repo"), db.SelectRecent(0), tr("Repo Index (replink.xml)"));

    QString url=manejador.toUtf8();
    if(xmlFilePath.replace("\\","/")!=url.replace("\\","/")){
        xmlM->MergeRepo(xmlFilePath,url);
        if(ui->actionAuto_Update->isChecked()){
            UpdateAppsList();
        }
    }else{
        QMessageBox::warning(this, "Error", "The index can't be the same! Duh.", QMessageBox::Ok);
    }

}


void MainWindow::on_actionClose_repo_triggered()
{
    CloseRepo();
}

void MainWindow::on_actionAuto_Rename_APKs_triggered(bool checked)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       "RepLink APK Manager", "RepLinkAPK");
    settings.setValue("Options/rename", checked?"1":"0");

}

void MainWindow::on_lKeyStore_clicked()
{
    CheckSig();
}

void MainWindow::on_actionOptions_triggered()
{
    config= new CConfig();
    connect(config, SIGNAL(finished(int)), this, SLOT(DestroyConfig(int)));
    config->show();

}

void MainWindow::on_actionApps_per_Categoy_count_triggered()
{

    countHilo->SetParams(repository);
    countHilo->start();
    ui->lLoading->show();


    if(countHilo->isRunning()){
        ui->statusBar->showMessage(ui->statusBar->currentMessage() +" Counting...");
    }else{
        ui->statusBar->showMessage("Counting...");
        ui->lLoading->repaint();
    }

}

void MainWindow::on_actionAuto_Update_triggered(bool checked)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       "RepLink APK Manager", "RepLinkAPK");
    settings.setValue("Options/update", checked?"1":"0");
}

void MainWindow::on_actionPreviews_triggered(bool checked)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       "RepLink APK Manager", "RepLinkAPK");
    settings.setValue("Options/preview", checked?"1":"0");
}

void MainWindow::on_actionRename_triggered()
{
    QFile f(repository+"/"+ui->lwNewApps->currentItem()->text());
    if(f.exists()){
        QString nuevo=ui->lwNewApps->currentItem()->data(6).toString();
        if(QFile(repository +"/"+nuevo).exists()){
            QMessageBox::critical(this,"Error","Currently there is a file with that name.");
        }else{
            if(f.rename(repository +"/"+nuevo)){
                for (int i = 1; i < files.Longitud(); ++i) {
                    if(files.ObtenerPorPos(i).ObtenerAPKName()==ui->lwNewApps->currentItem()->text()){
                        files.ObtenerPorPosMem(i)->SetAPKName(nuevo);
                    }
                }
                ui->lwNewApps->currentItem()->setText(nuevo);
                ui->lwNewApps->currentItem()->setBackground(QBrush());
                ui->lwNewApps->currentItem()->setData(6,QVariant(""));

            }else{
                QMessageBox::critical(this,"Error","Check the write access to the file.");
            }

        }
    }else{
        QMessageBox::critical(this,"Error","The file not longer exists.");
        ui->lwNewApps->takeItem(ui->lwNewApps->currentRow());
    }

}

void MainWindow::on_actionAPK_Files_Viewer_triggered()
{

    apkFilesV= new APKFilesViewer();
    connect(apkFilesV, SIGNAL(finished(int)), this, SLOT(DestroyFilesView(int)));
    apkFilesV->show();

}


void MainWindow::on_actionBackUp_Repo_triggered()
{

    QMessageBox::StandardButton confirm;
    bool flag;
    QString backupDir;

    for (int i = 1; i <= repos.Longitud(); ++i) {
        if(QDir().toNativeSeparators(repository)==QDir().toNativeSeparators(repos.ObtenerPorPos(i).ObtenerDir())){
            if(repos.ObtenerPorPos(i).ObtenerBackUpDir()!=""){
                confirm = QMessageBox::question(this, "BackUp...", "Do you want to backup on the current repository predefined location... ?\n"+repos.ObtenerPorPos(i).ObtenerBackUpDir()+"", QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
                if(confirm==QMessageBox::Yes){
                    flag=true;
                    backupDir=repos.ObtenerPorPos(i).ObtenerBackUpDir();
                    break;
                }
            }
        }
    }


    if(!flag && confirm != QMessageBox::Cancel){
        QFileDialog abrir;
        QString ruta= DatAccess().SelectRecent(4);
        backupDir= abrir.getExistingDirectory(this, tr("Abrir Ruta"), ruta);
        if(backupDir.trimmed()!=""){
            confirm = QMessageBox::question(this, "BackUp...", "Are you sure you want to backup the current repository on... ?\n"+backupDir.toUtf8()+"", QMessageBox::Yes|QMessageBox::No);
        }
    }

    if(confirm==QMessageBox::Yes || flag){
        if(backupDir.trimmed()!=""){
            DatAccess().UpdateRecent(backupDir,4);

            ui->actionBackUp_Repo->setEnabled(false);
            ui->actionBackUp_Repo->setVisible(false);
            ui->actionCancel_BackUp->setVisible(true);

            backupHilo= new CBackupHilo();
            connect(backupHilo, SIGNAL(Porciento(int,QString,QString)), this, SLOT(slot_BackupPorc(int,QString,QString)));
            connect(backupHilo, SIGNAL(finished()), this, SLOT(DestroyHiloBackup()));
            backupHilo->SetParams(repository,backupDir);
            backupHilo->start();
        }
    }

}

void MainWindow::on_actionCancel_BackUp_triggered()
{
    if(backupHilo->isRunning()){
        QMessageBox::StandardButton confirm;
        confirm = QMessageBox::question(this, "Cancel BackUp...", "Are you sure you want to cancel the current BackUp?", QMessageBox::Yes|QMessageBox::No);
        if(confirm==QMessageBox::Yes){
            backupHilo->Cerrar();
            backupHilo->exit();
            backupHilo->disconnect();
            ui->statusBar->showMessage("BackUp Canceled by user.",3000);
            ui->actionCancel_BackUp->setVisible(false);
            ui->actionBackUp_Repo->setVisible(true);
        }
    }
}

void MainWindow::on_actionTo_Web_Links_triggered()
{

    QFileDialog abrir;
    QString ruta= DatAccess().SelectRecent(6);
    QString outputDir= abrir.getExistingDirectory(this, tr("Abrir Ruta"), ruta);
    QString url=outputDir.toUtf8();

    if(url.trimmed()!=""){
        QMessageBox::StandardButton confirm;
        bool latest;
        bool plain;
        confirm = QMessageBox::question(this, "To Web Links...", "Do you want only latest?", QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);

        if(confirm!=QMessageBox::Cancel){
            if(confirm==QMessageBox::Yes){
                latest=true;
            }else{
                latest=false;
            }

            confirm = QMessageBox::question(this, "To Web Links...", "Would you prefer in Plain Text (.txt)?", QMessageBox::Yes|QMessageBox::No);
            if(confirm==QMessageBox::Yes){
                plain=true;
            }else{
                plain=false;
            }


            DatAccess().UpdateRecent(outputDir,6);

            ui->actionTo_Web_Links->setEnabled(false);

            toHtml= new CToHtlmHilo();
            connect(toHtml, SIGNAL(Porciento(int,QString,QString)), this, SLOT(slot_ToHtmlPorc(int,QString,QString)));
            connect(toHtml, SIGNAL(finished()), this, SLOT(DestroyHiloToHtml()));
            toHtml->SetParams(repository,latest,plain,outputDir);
            toHtml->start();
        }


    }


}

void MainWindow::on_actionSubstract_repo_triggered()
{

    QFileDialog abrir;
    DatAccess db;
    QString manejador= abrir.getOpenFileName(this, tr("Abrir Repo"), db.SelectRecent(0), tr("Repo Index (replink.xml)"));

    QString url=manejador.toUtf8();
    if(xmlFilePath.replace("\\","/")!=url.replace("\\","/")){
        xmlM->SubstractRepo(xmlFilePath,url);
        if(ui->actionAuto_Update->isChecked()){
            UpdateAppsList();
        }
    }else{
        QMessageBox::warning(this, "Error", "The index can't be the same! Duh.", QMessageBox::Ok);
    }

}

void MainWindow::on_actionSummary_like_Category_triggered()
{

        if(!repository.isEmpty()){
            QMessageBox::StandardButton confirm;
            confirm = QMessageBox::question(this, "Summary like Category", "Do you really want to turn Unknown Summaries to category?", QMessageBox::Yes|QMessageBox::No);
            if(confirm == QMessageBox::Yes){
                xmlM->SummaryLikeCategory(xmlFilePath);
                UpdateAppsList();
            }
        }

}

void MainWindow::on_actionGenerate_JSON_triggered()
{

        if(!repository.isEmpty()){
            QMessageBox::StandardButton confirm;
            confirm = QMessageBox::question(this, "Generate JSON...", "Please confirm to Generate JSON Index v1.", QMessageBox::Yes|QMessageBox::No);
            if(confirm == QMessageBox::Yes){
                ui->actionGenerate_JSON->setEnabled(false);
                jsonHilo= new CJSONHilo();
                connect(jsonHilo, SIGNAL(Porciento(int)), this, SLOT(slot_GenerateJsonPorc(int)));
                connect(jsonHilo, SIGNAL(finished()), this, SLOT(DestroyHiloGenerateJson()));
                jsonHilo->SetParams(xmlM,repository);
                jsonHilo->start();
            }
        }

}

void MainWindow::on_actionBackUp_APKs_Too_triggered(bool checked)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       "RepLink APK Manager", "RepLinkAPK");
    settings.setValue("Options/apkbackup", checked?"1":"0");
}
