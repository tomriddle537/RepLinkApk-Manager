#include "pkglistview.h"
#include "ui_pkglistview.h"

PkgListView::PkgListView(QString repository, QString appname, ListaSE<PkgInfo> packages, CXMLMethods *xmlM, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PkgListView)
{
    ui->setupUi(this);
    this->repository=repository;
    this->xmlM=xmlM;

    QPixmap pixmap;
    QIcon ico;

    pixmap.load(repository+"/icons/"+packages.ObtenerPorPos(1).ObtenerId()+".png");

    ico= QIcon(pixmap);

    this->ui->label->setText(packages.ObtenerPorPos(1).ObtenerId());
    this->setWindowTitle(appname);
    this->setWindowIcon(ico);

    QListWidgetItem *newItem;
    PkgInfo auxPkg;

    ui->listWidget->clear();

    for (int i = 1; i <= packages.Longitud(); ++i) {
        newItem = new QListWidgetItem();
        auxPkg= packages.ObtenerPorPos(i);

        QVariant var;
        var.setValue(auxPkg);

        newItem->setData(6,var);

        newItem->setSizeHint(QSize(75, 40));
        newItem->setToolTip(auxPkg.toString());
        newItem->setText(auxPkg.ObtenerVersion());

        newItem->setTextAlignment(Qt::AlignCenter);

        ui->listWidget->addItem(newItem);
    }

    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(PopUp(QPoint)));
}

PkgListView::~PkgListView()
{
    delete ui;
}


void PkgListView::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    PkgInfo aux=qvariant_cast<PkgInfo>(item->data(6));
    apki = new ApkInfoView(repository+"/"+aux.ObtenerApkName(),  repository, xmlM);
    connect(apki, SIGNAL(UpdatePkgInfo(PkgInfo)), this, SLOT(slot_UpdatePkg(PkgInfo)));
    apki->show();
}

void PkgListView::slot_UpdatePkg(PkgInfo pkg)
{
    ui->listWidget->currentItem()->setToolTip(pkg.toString());

    QVariant var;
    var.setValue(pkg);

    ui->listWidget->currentItem()->setData(6,var);

    ListaSE<PkgInfo> aux;

    for (int i = 0; i < ui->listWidget->count(); ++i) {
        aux.AdicionarCola( qvariant_cast<PkgInfo>(ui->listWidget->item(i)->data(6)));
    }

   emit UpdatePkgs(aux);

}

void PkgListView::PopUp(const QPoint &pos)
{
    if(ui->listWidget->selectedItems().count()>0 && ui->listWidget->indexAt(pos).isValid() && ui->listWidget->currentRow()!=0){
        QPoint globalPos= ui->listWidget->mapToGlobal(pos);
        QMenu popupMenu;
        popupMenu.addAction(ui->actionDeletePkg);

        popupMenu.exec(globalPos);
    }
}

void PkgListView::on_actionDeletePkg_triggered()
{
    QListWidgetItem *item=ui->listWidget->currentItem();
    PkgInfo aux=qvariant_cast<PkgInfo>(item->data(6));
    QMessageBox::StandardButton confirm;
    confirm = QMessageBox::question(this, "Delete", "Are you sure you want to delete \""+ aux.ObtenerVersion()+"\" ?", QMessageBox::Yes|QMessageBox::No);
    if(confirm==QMessageBox::Yes){
        xmlM->DeletePkg(repository+"/replink.xml", aux);
        emit DeletePkg(ui->listWidget->currentRow());
        ui->listWidget->takeItem(ui->listWidget->currentRow());
    }
}
