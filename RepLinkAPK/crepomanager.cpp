#include "crepomanager.h"
#include "ui_crepomanager.h"

CRepoManager::CRepoManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CRepoManager)
{
    ui->setupUi(this);
    LoadRepos();
    ui->buttonBox->buttons().at(0)->setText("Close");
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    xmlM=new CXMLMethods();
    connect(ui->listWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(PopUp(QPoint)));
    connect(xmlM, SIGNAL(Notice(QString,int)), this, SLOT(slot_Buzon(QString,int)));

}

CRepoManager::~CRepoManager()
{
    delete ui;
}

void CRepoManager::slot_RepoEditor(int result)
{
    Q_UNUSED(result);
    LoadRepos();
    //delete rEditor;
}

void CRepoManager::slot_Buzon(QString msg, int type)
{
    Q_UNUSED(type);
    QMessageBox::information(this,"Reminder", msg + " Remember to make a proper config to the program.");
}

void CRepoManager::LoadRepos()
{

    repos = bd.SelectRepoFromId(-1);

    ui->listWidget->clear();

    for (int j = 1; j <= repos.Longitud(); ++j) {
        QPixmap pixmap;

        QListWidgetItem *newItem;
        QIcon ico;

        newItem = new QListWidgetItem();

        //pixmap.load(repos.ObtenerPorPos(j).ObtenerDir()+"/icons/repo_icon.png");


        QString rutaIcon=repos.ObtenerPorPos(j).ObtenerDir()+"/icons/"+xmlM->XMLRepoIcon(repos.ObtenerPorPos(j).ObtenerDir()+"/replink.xml");

        if(QFileInfo(rutaIcon).exists()){
            pixmap.load(rutaIcon);
        }else{
            pixmap.load(":/img/info_small.png");
        }


        ico= QIcon(pixmap);

        newItem->setIcon(ico);

        QVariant var;
        var.setValue(repos.ObtenerPorPos(j));

        newItem->setData(6,var);

        if(!QFile(repos.ObtenerPorPos(j).ObtenerDir()+"/replink.xml").exists()){
              newItem->setBackground(QColor(150,50,0,50));
        }


        newItem->setSizeHint(QSize(290, 30));
        newItem->setToolTip(repos.ObtenerPorPos(j).toString());
        newItem->setText(repos.ObtenerPorPos(j).ObtenerName());


        ui->listWidget->addItem(newItem);
    }
}


void CRepoManager::PopUp(const QPoint &pos)
{
    if(ui->listWidget->selectedItems().count()>0 && ui->listWidget->indexAt(pos).isValid()){
        QPoint globalPos= ui->listWidget->mapToGlobal(pos);
        QMenu popupMenu;
        popupMenu.addAction(ui->actionDeleteRepo);
        popupMenu.exec(globalPos);
    }
}

void CRepoManager::on_buttonBox_rejected()
{
    emit finished(0);
}

void CRepoManager::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    RepoInfo aux=qvariant_cast<RepoInfo>(item->data(6));
    rEditor = new repoeditorview(aux.ObtenerDir(),aux.ObtenerId());
    connect(rEditor, SIGNAL(finished(int)), this, SLOT(slot_RepoEditor(int)));
    rEditor->show();
}

void CRepoManager::on_actionDeleteRepo_triggered()
{
    QListWidgetItem *item=ui->listWidget->currentItem();
    RepoInfo aux=qvariant_cast<RepoInfo>(item->data(6));
    QMessageBox::StandardButton confirm;
    confirm = QMessageBox::question(this, "Delete", "Are you sure you want to delete \""+ aux.ObtenerName()+"\" ?", QMessageBox::Yes|QMessageBox::No);
    if(confirm==QMessageBox::Yes){
        bd.DeleteRepo(aux.ObtenerId());
        ui->listWidget->takeItem(ui->listWidget->currentRow());
        LoadRepos();
    }
}

void CRepoManager::on_tbRefresh_clicked()
{
    LoadRepos();
}

void CRepoManager::on_tbAdd_clicked()
{
    QMessageBox::StandardButton confirm;
    confirm = QMessageBox::question(this, "Add", "Are you adding an existing repository?", QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
    QFileDialog abrir;
    CXMLRepo rinf;
    if(confirm == QMessageBox::Yes){
        QString manejador= abrir.getOpenFileName(this, tr("Abrir Repo"), bd.SelectRecent(5), tr("Repo Index (replink.xml)"));

        QString url=manejador.toUtf8();


        if(url.trimmed()!=""){
            bd.UpdateRecent(url,5);
            if(url.indexOf(' ')==-1){
                url.replace("\\","/");

                rinf=xmlM->XMLRepoInfo(url);

                QStringList partes= url.split("/");
                partes.removeAt(partes.length()-1);
                QString dir=partes.join("/");

                bool result= bd.InsertRepo(rinf.ObtenerName()==""?"Existing Repo":rinf.ObtenerName(), dir, rinf.ObtenerUrl()==""?"http://existingrepo.com":rinf.ObtenerUrl());
                if(!result){
                    QMessageBox::warning(this, "SQL Error", "Something has been unsuccesful.\nMake sure that there is no other repo with these characteristics and...\nTry Again.", QMessageBox::Ok);
                }else{
                    LoadRepos();
                }
            }else{
                QMessageBox::warning(this, "Error", "The repo's path can't include spaces. (' ')", QMessageBox::Ok);
            }
        }
    }else if(confirm == QMessageBox::No){
        QString reponew= abrir.getExistingDirectory(this, tr("Abrir Ruta"), bd.SelectRecent(0));

        reponew.replace("\\","/");
        QFileInfo fileI= QFileInfo(reponew+"/replink.xml");

        if(reponew.trimmed()!=""){
            if(reponew.indexOf(' ')==-1){
                if(!fileI.exists()){

                    QString pubkey=xmlM->GetPubKey();

                    //QStringList lines=pubkey.split("\n");

                    //std::cout << pubkey.toStdString() <<std::endl;

                    rinf=CXMLRepo("New Repo", "http://newrepo.com", "New Repo", "repo-icon.png", pubkey, "17");
                    xmlM->CreateNewRepo(rinf, reponew);
                    if(fileI.exists()){
                        bool result= bd.InsertRepo(rinf.ObtenerName()==""?"Existing Repo":rinf.ObtenerName(), reponew, rinf.ObtenerUrl()==""?"http://existingrepo.com":rinf.ObtenerUrl());
                        if(!result){
                            QMessageBox::warning(this, "SQL Error", "Something has been unsuccesful.\nMake sure that there is no other repo with these characteristics and...\nTry Again.", QMessageBox::Ok);
                        }else{
                            LoadRepos();
                        }
                    }else{
                        QMessageBox::warning(this, "Error", "This directory has not writing permissions", QMessageBox::Ok);
                    }


                }else{
                    QMessageBox::warning(this, "Existing Repo", "This directory has files from an existing repository", QMessageBox::Ok);
                }
            }else{
                QMessageBox::warning(this, "Error", "The repo's path can't include spaces. (' ')", QMessageBox::Ok);
            }
        }
    }
}
