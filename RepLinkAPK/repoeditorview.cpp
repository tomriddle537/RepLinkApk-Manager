#include "repoeditorview.h"
#include "ui_repoeditorview.h"

repoeditorview::repoeditorview(QString repo, int id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::repoeditorview)
{
    ui->setupUi(this);
    this->repository=repo;
    this->id=id;

    QFileInfo infoFile;
    infoFile=QFileInfo(repository+"/replink.xml");
    if(infoFile.exists()){
        CXMLRepo repo = xmlM->XMLRepoInfo(repository+"/replink.xml");

        if(repo.ObtenerName()!=""){
            QPixmap repoPix;
            repoPix.load(repository+"/icons/"+repo.ObtenerIcon());
            repoIcon.addPixmap(repoPix);
            ui->lIcon->setPixmap(repoIcon.pixmap(ui->lIcon->size()));


            ui->lineEdit->setText(repository);
            ui->eName->setText(repo.ObtenerName());
            QString protocol= repo.ObtenerUrl().split("://").at(0);
            for (int k = 0; k < ui->comboBox->count(); ++k) {
                if(ui->comboBox->itemText(k)==protocol+"://"){
                    ui->comboBox->setCurrentIndex(k);
                    break;
                }
            }
            ui->eBackup->setText(DatAccess().SelectRepoFromId(id).ObtenerPorPos(1).ObtenerBackUpDir());
            ui->eUrl->setText(repo.ObtenerUrl().split("://").at(1));
            ui->tDesc->setPlainText(repo.ObtenerDescription());
        }else{
            QMessageBox::warning(this, "Error", "There was a problem with the repo config files.");
            this->hide();
        }
    }


}

repoeditorview::~repoeditorview()
{
    delete ui;
}

void repoeditorview::on_buttonBox_accepted()
{
    QFileInfo infoFile;
    infoFile=QFileInfo(ui->lineEdit->text()+"/replink.xml");
    if(infoFile.exists()){
        RepoInfo repo=RepoInfo(id, ui->eName->text(), ui->lineEdit->text(), ui->comboBox->currentText()+ui->eUrl->text().replace("http://","").replace("https://","").replace("ftp://","").replace("://",""), ui->eBackup->text());
        repo.SetDescription(ui->tDesc->toPlainText());
        xmlM->UpdateRepoInfo(ui->lineEdit->text()+"/replink.xml",repo);
        bool result = bd.UpdateRepoInfo(repo);
        if(result){
            emit finished(0);
        }
    }
}

void repoeditorview::on_pushButton_2_clicked()
{
catE = new CatEditor(ui->lineEdit->text());
catE->show();
}



void repoeditorview::on_bBackupDir_clicked()
{
    QFileDialog abrir;
    QString direc= abrir.getExistingDirectory(this, tr("Abrir Ruta"), ui->eBackup->text());

    QDir d(direc);

    if(d.exists()){
        ui->eBackup->setText(QString(d.absolutePath()).replace("\\","/"));
    }else{
        QMessageBox::warning(this,"Error", "This directory it's not correct. ");
    }
}
