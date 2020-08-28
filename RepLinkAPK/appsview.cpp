#include "appsview.h"
#include "ui_appsview.h"

AppsView::AppsView(QString repository, CXMLMethods *xmlM, QIcon repoIcon, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AppsView)
{

    ui->setupUi(this);
    this->xmlM=xmlM;
    this->repository=repository;
    this->setWindowIcon(repoIcon);

    if(this->repository!=""){

        ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(ui->listWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(PopUp(QPoint)));

        getApps=new CListHilo();
        qRegisterMetaType<AppInfo>("AppInfo");

        connect(getApps, SIGNAL(SendInfo(AppInfo)), this, SLOT(slot_GetApp(AppInfo)));
        connect(getApps, SIGNAL(Porciento(int)), this, SLOT(Porciento(int)));
        connect(getApps, SIGNAL(finished()), this, SLOT(DestroyHilo()));

        getApps->SetParams(repository);
        getApps->start();

        ui->status->setText("Loading...");
    }
    ui->orderAlph->setEnabled(false);
    ui->lineEdit->setEnabled(false);
    ui->orderCantPkg->setEnabled(false);


}

AppsView::~AppsView()
{
    delete ui;
}

void AppsView::slot_UpdateApp(AppInfo app)
{

    xmlM->UpdateAppInfo(repository+"/replink.xml", app);

    ui->listWidget->currentItem()->setToolTip(app.toString());
    ui->listWidget->currentItem()->setText(app.ObtenerName());

    QVariant var;
    var.setValue(app);

    ui->listWidget->currentItem()->setData(6,var);
    ui->lineEdit->setText(ui->lineEdit->text()+" ");
    //delete mdesc;
    ui->lineEdit->setText(ui->lineEdit->text());
}

void AppsView::slot_Preview(ApkPreview prev)
{
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        if(ui->listWidget->item(i)->text()==prev.ObtenerName()){
            QIcon check=QIcon();
            check.addPixmap(prev.ObtenerPixmap());
            ui->listWidget->item(i)->setIcon(check);
            break;
        }
    }
}

void AppsView::slot_GetApp(AppInfo auxApp)
{

    QListWidgetItem *newItem;


    newItem = new QListWidgetItem();


    previewHilo=new CPreviewHilo();
    ListaSE<NewAPKFile> files;
    files.AdicionarCabeza(NewAPKFile(auxApp.ObtenerName(),auxApp.ObtenerId()));
    previewHilo->SetParams(repository,files,false);
    connect(previewHilo, SIGNAL(Preview(ApkPreview)), this, SLOT(slot_Preview(ApkPreview)) );
    previewHilo->start();
    /*QIcon ico;

    ico= QIcon(pixmap);

    newItem->setIcon(ico);*/

    QVariant var;
    var.setValue(auxApp);

    newItem->setData(6,var);


    newItem->setSizeHint(QSize(200, 40));
    newItem->setToolTip(auxApp.toString());
    newItem->setText(auxApp.ObtenerName());

    newItem->setTextAlignment(Qt::AlignVCenter);

    ui->listWidget->addItem(newItem);

}

void AppsView::slot_UpdatePkgs(ListaSE<PkgInfo> pkgs)
{
    AppInfo aux= qvariant_cast<AppInfo>(ui->listWidget->currentItem()->data(6) );
    aux.SetPackages(pkgs);
    QVariant var;
    var.setValue(aux);
    ui->listWidget->currentItem()->setData(6,var);
}


void AppsView::slot_UpdatePkgs(int pos)
{
    AppInfo aux= qvariant_cast<AppInfo>(ui->listWidget->currentItem()->data(6) );
    ListaSE<PkgInfo> pkgs= aux.ObtenerPackages();
    pkgs.BorrarPorPos(pos+1);
    aux.SetPackages(pkgs);
    QVariant var;
    var.setValue(aux);
    ui->listWidget->currentItem()->setData(6,var);
    ui->listWidget->currentItem()->setToolTip(aux.toString());
}

void AppsView::DestroyHilo()
{
    disconnect(getApps, SIGNAL(SendInfo(AppInfo)), this, SLOT(slot_GetApp(AppInfo)));
    disconnect(getApps, SIGNAL(Porciento(int)), this, SLOT(Porciento(int)));
    disconnect(getApps, SIGNAL(finished()), this, SLOT(DestroyHilo()));
    delete getApps;
}

void AppsView::DestroyPkgs(int result)
{
    Q_UNUSED(result);
    pkglv->disconnect();
    delete pkglv;
}

void AppsView::Porciento(int porc)
{
    if(porc!=100){
        ui->lCount->setText(QString::number(porc)+"%");
    }else{
        ui->status->setText("Loaded");
        ui->lineEdit->setEnabled(true);
        ui->orderAlph->setEnabled(true);
        ui->orderCantPkg->setEnabled(true);
        ui->lineEdit->setFocus();
        ReCount();
    }
}

void AppsView::on_lineEdit_textChanged(const QString &arg1)
{
    int cant=ui->listWidget->count();
    if(!arg1.isEmpty()){
        bool idL;
        bool nameL;
        bool sumL;
        bool catL;
        bool catiesL;
        bool licenseL;





        bool idU;
        bool nameU;
        bool sumU;
        bool catU;
        bool catiesU;
        bool licenseU;

        bool noPKG;

        QStringList partes=arg1.toLower().split(" ");
        for (int x = 0; x < partes.length(); ++x) {
            if(partes.at(x).isEmpty()){
                partes.removeAt(x);
                x--;
            }
        }

        QString arg=partes.join(" ");
        AppInfo aux;

        for (int i = 0; i < ui->listWidget->count(); ++i) {
            aux=qvariant_cast<AppInfo>(ui->listWidget->item(i)->data(6));

            idL= aux.ObtenerId().toLower().indexOf(arg)!=-1;
            nameL=QString(aux.ObtenerName().toLower()).replace("á","a").replace("é","e").replace("í","i").replace("ú","u").replace("ó","o").indexOf(arg)!=-1;
            sumL=aux.ObtenerSummary().toLower().indexOf(arg)!=-1;
            catL=aux.ObtenerCategory().toLower().indexOf(arg)!=-1;
            catiesL=aux.ObtenerCategories().toLower().indexOf(arg)!=-1;
            licenseL=aux.ObtenerLicense().toLower().indexOf(arg)!=-1;

            idU=aux.ObtenerId().toUpper().indexOf(arg)!=-1;
            nameU=QString(aux.ObtenerName().toUpper()).replace("Á","A").replace("É","E").replace("Í","I").replace("Ú","U").replace("Ó","O").toUpper().indexOf(arg)!=-1;
            sumU=aux.ObtenerSummary().toUpper().indexOf(arg)!=-1;
            catU=aux.ObtenerCategory().toUpper().indexOf(arg)!=-1;
            catiesU=aux.ObtenerCategories().toUpper().indexOf(arg)!=-1;
            licenseU=aux.ObtenerLicense().toUpper().indexOf(arg)!=-1;

            noPKG=aux.ObtenerNoPackages()== arg;

            if(idL||nameL||sumL||catL||catiesL||licenseL|| noPKG){
                ui->listWidget->item(i)->setHidden(false);
            }else if(idU||nameU||sumU||catU||catiesU||licenseU){
                ui->listWidget->item(i)->setHidden(false);
            }else if(aux.ObtenerRequirements().indexOf(arg)!=-1){
                ui->listWidget->item(i)->setHidden(false);
            }else{
                ui->listWidget->item(i)->setHidden(true);
                cant--;
            }
        }
    } else {
        for (int i = 0; i < ui->listWidget->count(); ++i) {
            ui->listWidget->item(i)->setHidden(false);
        }
    }
    ui->lCount->setText(QString::number(cant));
}

void AppsView::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    if(ui->status->text()=="Loaded"){
        AppInfo aux=qvariant_cast<AppInfo>(item->data(6));
        mdesc = new manualDescription(repository, aux);
        connect(mdesc, SIGNAL(UpdateAppInfo(AppInfo)), this, SLOT(slot_UpdateApp(AppInfo)));
        mdesc->show();
    }
}

void AppsView::on_AppsView_finished(int result)
{
    Q_UNUSED(result);
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        delete ui->listWidget->item(i);
    }

}

void AppsView::PopUp(const QPoint &pos)
{
    if(ui->listWidget->selectedItems().count()>0 && ui->listWidget->indexAt(pos).isValid() && ui->status->text()=="Loaded"){
        QPoint globalPos= ui->listWidget->mapToGlobal(pos);
        QMenu popupMenu;

        AppInfo aux = qvariant_cast<AppInfo>(ui->listWidget->currentItem()->data(6));
        if(aux.ObtenerPackages().Longitud()>0){
            popupMenu.addAction(ui->actionViewPackages);
        }
        popupMenu.addAction(ui->actionDeleteApp);

        popupMenu.exec(globalPos);
    }
}

void AppsView::on_actionViewPackages_triggered()
{
    AppInfo aux=qvariant_cast<AppInfo>(ui->listWidget->currentItem()->data(6));
    pkglv=new PkgListView(repository, aux.ObtenerName(), aux.ObtenerPackages(), xmlM);
    connect(pkglv, SIGNAL(UpdatePkgs(ListaSE<PkgInfo>)) , this, SLOT(slot_UpdatePkgs(ListaSE<PkgInfo>)));
    connect(pkglv, SIGNAL(DeletePkg(int)) , this, SLOT(slot_UpdatePkgs(int)));
    connect(pkglv, SIGNAL(finished(int)) , this, SLOT(DestroyPkgs(int)));

    pkglv->show();
}

void AppsView::on_actionDeleteApp_triggered()
{
    QListWidgetItem *item=ui->listWidget->currentItem();
    AppInfo aux=qvariant_cast<AppInfo>(item->data(6));
    QMessageBox::StandardButton confirm;
    confirm = QMessageBox::question(this, "Delete", "Are you sure you want to delete \""+ aux.ObtenerName()+"\" ?", QMessageBox::Yes|QMessageBox::No);
    if(confirm==QMessageBox::Yes){
        xmlM->DeleteApp(repository+"/replink.xml", aux);
        ui->listWidget->takeItem(ui->listWidget->currentRow());
        ReCount();
    }
}

void AppsView::on_orderAlph_clicked()
{
    if(sortAsc){

        ui->listWidget->sortItems(Qt::AscendingOrder);
        sortAsc=false;
    }else{
        ui->listWidget->sortItems(Qt::DescendingOrder);
        sortAsc=true;
    }
}

void AppsView::on_orderCantPkg_clicked()
{

    sortH=new SortHilo();
    connect(sortH, SIGNAL(Porciento(int)), this, SLOT(Porciento(int)));
    //connect(sortH, SIGNAL(finished()), this, SLOT(DestroyHilo()));


    ui->lineEdit->setText("");
    sortH->SetParametros(ui->listWidget,!sortPkgs);


    if(!sortPkgs){
        sortPkgs=true;
    }else{
        sortPkgs=false;
    }


    sortH->start();

    ui->status->setText("Sorting...");
    ui->orderAlph->setEnabled(false);
    ui->lineEdit->setEnabled(false);
    ui->orderCantPkg->setEnabled(false);

}

void AppsView::ReCount()
{
        int cant=ui->listWidget->count();
        ui->lCount->setText(QString::number(cant));
}
