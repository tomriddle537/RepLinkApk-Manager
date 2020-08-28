#include "apkreport.h"
#include "ui_apkreport.h"

ApkReport::ApkReport(QString repository, CXMLMethods *xmlM, QIcon repoIcon, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ApkReport)
{
    ui->setupUi(this);
    this->xmlM=xmlM;
    this->repository=repository;
    this->setWindowIcon(repoIcon);

    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(PopUp(QPoint)));

    QMovie *movie = new QMovie(":/img/win-loading.gif");
    movie->setScaledSize(ui->lLoading->size());
    ui->lLoading->setMovie(movie);
    movie->start();

    OpenRepo();
    Report();
}

ApkReport::~ApkReport()
{
    delete ui;
}

void ApkReport::slot_GetReport(QString dobles, QStringList unused, QStringList missed)
{
    ui->listWidget->clear();
    ui->listWidget_2->clear();
    ui->textEdit->clear();

    ui->textEdit->setHtml(dobles);
    // ui->lCantD->setText(QString::number(dobles.length()));
    ui->listWidget_2->addItems(missed);
    ui->lCantM->setText(QString::number(missed.length()));
    ui->listWidget->addItems(unused);
    ui->lCantU->setText(QString::number(unused.length()));

    if(unused.length()>0){
        ui->toolButton_2->setEnabled(true);
    }else{
        ui->toolButton_2->setEnabled(false);
    }

    ui->lLoading->hide();
}

void ApkReport::OpenRepo()
{
    QDir currentDir = QDir(repository);
    QString  fileName = "*.apk";
    existFiles  = currentDir.entryList(QStringList(fileName), QDir::Files | QDir::NoSymLinks);
}

void ApkReport::Report()
{
    ui->lLoading->show();

    reportHilo= new CReportHilo();
    connect(reportHilo, SIGNAL(Report(QString,QStringList,QStringList)), this, SLOT(slot_GetReport(QString,QStringList,QStringList)));
    reportHilo->SetParams(repository, existFiles);
    reportHilo->start();
}


void ApkReport::on_actionDelete_triggered()
{
    QListWidgetItem *item=ui->listWidget->currentItem();
    QString aux=item->text();
    QMessageBox::StandardButton confirm;
    if(!yesToAll){
        confirm = QMessageBox::question(this, "Delete", "Are you sure you want to delete \""+ aux+"\" permanently?", QMessageBox::Yes|QMessageBox::No);
    }
    if(confirm==QMessageBox::Yes || yesToAll){
        QFile f(QDir::toNativeSeparators(repository+"/"+ui->listWidget->currentItem()->text()));
        if(f.remove()){
            ui->listWidget->takeItem(ui->listWidget->currentRow());

            QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                               "RepLink APK Manager", "RepLinkAPK");

            if(settings.value("Options/update").toString()=="1" && !yesToAll){
                OpenRepo();
                ui->listWidget->clear();
                ui->listWidget_2->clear();
                ui->textEdit->clear();
                Report();
            }else if(yesToAll){
                ui->listWidget->removeItemWidget(ui->listWidget->currentItem());
            }
        }else{
            QMessageBox::warning(this, "Error", "The file \""+ aux+"\" could not be deleted.", QMessageBox::Yes|QMessageBox::No);
        }
    }
}

void ApkReport::PopUp(const QPoint &pos)
{
    if(ui->listWidget->selectedItems().count()>0 && ui->listWidget->indexAt(pos).isValid()){
        QPoint globalPos= ui->listWidget->mapToGlobal(pos);
        QMenu popupMenu;
        popupMenu.addAction(ui->actionDelete);

        popupMenu.exec(globalPos);
    }
}

void ApkReport::on_toolButton_2_clicked()
{
    QStringList files;
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        if(ui->listWidget->item(i)->isSelected()){
            files.append(ui->listWidget->item(i)->text());
        }else if (ui->listWidget->selectedItems().count()==0) {
            files.append(ui->listWidget->item(i)->text());
        }
    }

    emit Files(files);
    emit finished(0);
    this->close();
}

void ApkReport::on_tbRefresh_clicked()
{
    OpenRepo();
    Report();
}

void ApkReport::on_tbClose_clicked()
{
    this->close();
}

void ApkReport::on_tbDelete_clicked()
{
    QList<QListWidgetItem*> lista = ui->listWidget->selectedItems();

    QMessageBox::StandardButton confirm;
    if(lista.length()==0){
        for (int i = 0; i < ui->listWidget->count(); ++i) {
            lista.append(ui->listWidget->item(i));
        }
    }

    confirm = QMessageBox::question(this, "Delete All", "Are you sure you want to delete these "+QString::number(lista.length())+" APKs permanently?", QMessageBox::YesToAll|QMessageBox::Yes|QMessageBox::No);
    if(confirm==QMessageBox::Yes){
        ui->listWidget->clearSelection();
        for (int i = 0; i < lista.length(); ++i) {
            ui->listWidget->setCurrentItem(lista.at(i));
            on_actionDelete_triggered();
        }
    }else if(confirm==QMessageBox::YesToAll){
        yesToAll=true;
        for (int i = 0; i < lista.length(); ++i) {
            ui->listWidget->setCurrentItem(lista.at(i));
            on_actionDelete_triggered();
        }
        yesToAll=false;
    }

    on_tbRefresh_clicked();
}
