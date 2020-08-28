#include "apkfilesviewer.h"
#include "ui_apkfilesviewer.h"

APKFilesViewer::APKFilesViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::APKFilesViewer)
{
    ui->setupUi(this);
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->listWidget->setIconSize(QSize(48,48));

    ui->ltext->hide();

    t= new QTimer();

    connect(ui->listWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(PopUp(QPoint)));

    qRegisterMetaType<ApkPreview>("ApkPreview");

    previewHilo = new CPreviewHilo();
    connect(previewHilo, SIGNAL(Preview(ApkPreview)), this, SLOT(slot_Preview(ApkPreview)));
    connect(previewHilo, SIGNAL(finished()), this, SLOT(slot_PreviewFinish()));

    repository= DatAccess().SelectRecent(3);
    ui->ePath->setText(repository);
    OpenFolder();
}

APKFilesViewer::~APKFilesViewer()
{
    delete ui;
}

void APKFilesViewer::slot_Preview(ApkPreview prev)
{
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        if(ui->listWidget->item(i)->data(309).toString()==prev.ObtenerApkName()){
            QIcon check=QIcon();
            check.addPixmap(prev.ObtenerPixmap());
            ui->listWidget->item(i)->setIcon(check);
            ui->listWidget->setIconSize(QSize(48,48));
            ui->listWidget->item(i)->setToolTip(prev.toString());

            if(prev.toString().indexOf("Recomended: ")!=-1){
                ui->listWidget->item(i)->setBackground(QColor(150,50,0,50));
            }
            QVariant var;
            var.setValue(prev);
            ui->listWidget->item(i)->setData(306,var);
            break;
        }
    }

}

void APKFilesViewer::PopUp(const QPoint &pos)
{

    if(ui->listWidget->selectedItems().count()>0 && ui->listWidget->indexAt(pos).isValid()){
        if(ui->listWidget->indexAt(pos).data(308).toString()!="Dir"){
            ApkPreview aux=qvariant_cast<ApkPreview>(ui->listWidget->indexAt(pos).data(306));
            QPoint globalPos= ui->listWidget->mapToGlobal(pos);
            QMenu popupMenu;
            if(aux.RecomendedName()!=aux.ObtenerApkName()){
                popupMenu.addAction(ui->actionRename);
            }
            popupMenu.addAction(ui->actionDelete);
            if(ui->listWidget->selectedItems().length()>1){
                popupMenu.addAction(ui->actionDelete_All_Selected);
            }

            popupMenu.exec(globalPos);
        }
    }else {
        ui->tbRename->setEnabled(false);
        ui->tbReload->setEnabled(false);
        if(ui->listWidget->count()>0){
            if(ui->listWidget->item(0)->data(309).toString()=="MY PC"){
                on_tbMiPC_clicked();
            }else if(ui->listWidget->item(0)->data(309).toString()==".."){
                Atras();
            }
        }else{
            Atras();
        }
    }

}

void APKFilesViewer::slot_PreviewFinish()
{
    ui->tbRename->setEnabled(true);
    ui->tbReload->setEnabled(true);
}

void APKFilesViewer::slot_Renamed(PkgInfo pkg)
{
    ApkPreview aux= qvariant_cast<ApkPreview>(ui->listWidget->currentItem()->data(306));
    aux.SetAPKName(pkg.ObtenerApkName());
    ui->listWidget->currentItem()->setText(pkg.ObtenerApkName());
    ui->listWidget->currentItem()->setBackground(QBrush());
    ui->listWidget->currentItem()->setToolTip(aux.toString());
    QVariant var;
    var.setValue(aux);
    ui->listWidget->currentItem()->setData(306,var);
    ui->listWidget->currentItem()->setData(309,pkg.ObtenerApkName());
    QString texto=aux.ObtenerApkName() + " | "+aux.ObtenerSize();
    ui->label->setText(texto==" | "?"":texto);
}

void APKFilesViewer::slot_textView()
{
    ui->ltext->hide();
}


void APKFilesViewer::SelectFolder(){
    QFileDialog abrir;
    repository = abrir.getExistingDirectory(this, tr("Abrir Ruta"), repository);
    OpenFolder();
}

void APKFilesViewer::OpenFolder()
{
    if(!repository.isEmpty()){
        ui->tbRename->setEnabled(false);
        ui->ltext->hide();
        ui->label->setText("");

        ui->ePath->setText(repository);
        QDir currentDir = QDir(repository);

        if(currentDir.exists()){
            DatAccess().UpdateRecent(repository,3);
        }

        QString  fileName = "*.apk";
        QStringList dirs = currentDir.entryList(QStringList("*"), QDir::Dirs | QDir::NoSymLinks, QDir::NoSort);
        QStringList temp= currentDir.entryList(QStringList(fileName), QDir::Files | QDir::NoSymLinks, QDir::NoSort);

        files= ListaSE<NewAPKFile>();
        for (int i = 0; i < temp.length(); ++i) {
            files.AdicionarCola(NewAPKFile(temp.at(i)));
        }

        ui->listWidget->clear();

        QListWidgetItem *newItem;

        if(repository.lastIndexOf(":")==1 && ((repository.lastIndexOf("/")==2 && repository.length()==3 ) || repository.length()==2)){

            newItem = new QListWidgetItem();
            newItem->setToolTip("MY PC");
            newItem->setText("MY PC");
            newItem->setData(309, "MY PC");
            newItem->setSizeHint(QSize(78, 78));
            QIcon check=QIcon();
            check.addFile(":/img/pc.png");
            newItem->setIcon(check);
            newItem->setData(308,QVariant("MYPC"));
            newItem->setTextAlignment(Qt::AlignTop);
            ui->listWidget->insertItem(0,newItem);
        }

        for (int j = 0; j < dirs.length(); ++j) {
            newItem = new QListWidgetItem();

            if(dirs.at(j)!="." && dirs.at(j)!="System Volume Information"){
                newItem->setToolTip(dirs.at(j));
                newItem->setText(dirs.at(j));
                newItem->setData(309, dirs.at(j));


                newItem->setSizeHint(QSize(78, 78));
                QIcon check=QIcon();
                if(dirs.at(j)!=".."){
                    check.addFile(":/img/folder.png");
                    newItem->setBackground(QColor(150,150,150,50));
                    newItem->setData(256, QVariant( QDir(repository+"/"+dirs.at(j)).entryList(QStringList("*"), QDir::Dirs | QDir::NoSymLinks, QDir::NoSort).length()-2));
                    newItem->setData(257, QVariant( QDir(repository+"/"+dirs.at(j)).entryList(QStringList(fileName), QDir::Files | QDir::NoSymLinks, QDir::NoSort).length()));
                }else{
                    check.addFile(":/img/back.png");
                }
                newItem->setIcon(check);
                newItem->setData(308,QVariant("Dir"));



                newItem->setTextAlignment(Qt::AlignCenter);
                if(dirs.at(j)!=".."){
                    ui->listWidget->addItem(newItem);
                }else{
                    ui->listWidget->insertItem(0,newItem);
                }
            }
        }

        for (int j = 1; j <= files.Longitud(); ++j) {
            newItem = new QListWidgetItem();

            newItem->setToolTip(files.ObtenerPorPos(j).toString());
            newItem->setText(files.ObtenerPorPos(j).ObtenerAPKName());
            newItem->setData(309, files.ObtenerPorPos(j).ObtenerAPKName());


            newItem->setSizeHint(QSize(78, 78));
            QIcon check=QIcon();
            check.addFile(":/img/apkfile.png");
            newItem->setTextAlignment(Qt::AlignCenter);
            newItem->setIcon(check);
            newItem->setData(308,QVariant("APK"));

            ui->listWidget->addItem(newItem);
        }

        ui->lCount->setText(QString::number(files.Longitud()));

        //        for (int x = 1; x <= files.Longitud(); ++x) {
        //            ListaSE<NewAPKFile> xfiles;
        //            xfiles.AdicionarCabeza(NewAPKFile(files.ObtenerPorPos(x)));
        previewHilo = new CPreviewHilo();
        //  prevs.AdicionarCola(new CPreviewHilo());
        connect(previewHilo, SIGNAL(Preview(ApkPreview)), this, SLOT(slot_Preview(ApkPreview)));
        connect(previewHilo, SIGNAL(finished()), this, SLOT(slot_PreviewFinish()));
        //  prevs.ObtenerPorPos(x)->SetParams(repository, files);
        //  prevs.ObtenerPorPos(x)->start();
        previewHilo->SetParams(repository, files);
        previewHilo->start();
        //}

        ui->listWidget->setFocus();
        // slot_PreviewFinish();



    }
}


void APKFilesViewer::Atras()
{

        repository=repository.replace("\\","/");
        repository=repository.replace("//", "/");
        if(repository.lastIndexOf("/")==2 || repository.length()==2){
            QStringList ante=repository.split("/");
            ante.removeLast();
            repository=ante.join("/");

        }else{
            QStringList ante=repository.split("/");
            ante.removeLast();
            repository=ante.join("/");
            if(!QDir(repository).exists() && ui->listWidget->item(0)->text()!="MY PC"){
                Atras();
            }
        }
        ui->ePath->setText(repository);
        OpenFolder();

}

void APKFilesViewer::wheelEvent(QWheelEvent *e)
{
    Q_UNUSED(e);
    ui->ltext->hide();
}

void APKFilesViewer::on_actionRename_triggered()
{

    QFile f(repository+"/"+ui->listWidget->item(0)->data(309).toString());
    if(f.exists()){
        ui->ltext->hide();
        ApkPreview aux=qvariant_cast<ApkPreview>(ui->listWidget->currentItem()->data(306));
        QString nuevo=aux.RecomendedName();
        if(QFile(repository +"/"+nuevo).exists()){
            if(!renaming){
                QMessageBox::critical(this,"Error","Currently there is a file with that name.");
            }
        }else{
            if(f.rename(repository +"/"+aux.ObtenerApkName(),repository +"/"+nuevo)){
                ui->listWidget->currentItem()->setText(nuevo);
                ui->listWidget->currentItem()->setBackground(QBrush());
                aux.SetAPKName(nuevo);
                ui->listWidget->currentItem()->setToolTip(aux.toString());
                QVariant var;
                var.setValue(aux);
                ui->listWidget->currentItem()->setData(306,var);
                QString texto=aux.ObtenerApkName() + " | "+aux.ObtenerSize();
                ui->label->setText(texto==" | "?"":texto);
            }else{
                QMessageBox::critical(this,"Error","Check the write access to the file.");
            }

        }
    }else{
        QMessageBox::critical(this,"Error","The file not longer exists.");
        ui->listWidget->takeItem(ui->listWidget->currentRow());
    }
}

void APKFilesViewer::on_tbOpen_clicked()
{
    SelectFolder();
}

void APKFilesViewer::on_listWidget_currentRowChanged(int currentRow)
{
    if(ui->listWidget->selectedItems().count()>0)
    {
        if(ui->listWidget->item(currentRow)->data(308).toString()=="Dir" && ui->listWidget->item(currentRow)->text()!=".."){
            ui->label->setText(ui->listWidget->item(currentRow)->data(256).toString()+" Directories | "+
                               ui->listWidget->item(currentRow)->data(257).toString()+" APK Files");
        }else if(ui->listWidget->item(currentRow)->data(308).toString()=="APK"){
            ApkPreview aux=qvariant_cast<ApkPreview>(ui->listWidget->item(currentRow)->data(306));
            QString texto=aux.ObtenerApkName() + " | "+aux.ObtenerSize();
            ui->label->setText(texto==" | "?"":texto);
        }else{
            ui->label->setText("");
        }
    }
}


void APKFilesViewer::on_listWidget_itemSelectionChanged()
{
    if(ui->listWidget->selectedItems().count()==0){
        ui->label->setText("");
    }
}

void APKFilesViewer::on_tbReload_clicked()
{
    OpenFolder();
}

void APKFilesViewer::on_actionDelete_triggered()
{
    QListWidgetItem *item=ui->listWidget->currentItem();
    QString aux=item->data(309).toString();
    QMessageBox::StandardButton confirm;
    if(!yesToAll){
        confirm = QMessageBox::question(this, "Delete", "Are you sure you want to delete \""+ aux+"\" permanently?", QMessageBox::Yes|QMessageBox::No);
    }
    if(confirm==QMessageBox::Yes || yesToAll){
        QFile f(QDir::toNativeSeparators(repository+"/"+ui->listWidget->currentItem()->data(309).toString()));
        if(f.remove()){
            ui->listWidget->takeItem(ui->listWidget->currentRow());
        }else{
            QMessageBox::warning(this, "Error", "The file \""+ aux+"\" could not be deleted.", QMessageBox::Yes|QMessageBox::No);
        }
    }
    ui->ltext->hide();
    yesToAll=false;
}

void APKFilesViewer::on_tbMiPC_clicked()
{
    QFileInfoList drives= QDir().drives();
    QString  fileName = "*.apk";

    ui->listWidget->clear();

    QListWidgetItem *newItem;

    for (int i = 0; i < drives.length(); ++i) {
        newItem = new QListWidgetItem();
        newItem->setToolTip(drives.at(i).group());
        newItem->setText(drives.at(i).absolutePath());
        newItem->setSizeHint(QSize(78, 78));
        QIcon check=QIcon();
        check.addFile(":/img/hdd.png");
        newItem->setData(256, QVariant( QDir(drives.at(i).absoluteDir()).entryList(QStringList("*"), QDir::Dirs | QDir::NoSymLinks, QDir::NoSort).length()));
        newItem->setData(257, QVariant( QDir(drives.at(i).absoluteDir()).entryList(QStringList(fileName), QDir::Files | QDir::NoSymLinks, QDir::NoSort).length()));
        newItem->setIcon(check);
        newItem->setData(308,QVariant("HDD"));
        newItem->setData(309,drives.at(i).absolutePath());
        newItem->setTextAlignment(Qt::AlignCenter);
        ui->listWidget->addItem(newItem);
    }

    ui->ePath->setText("MY PC");


}

void APKFilesViewer::on_tbRename_clicked()
{
    QMessageBox::StandardButton confirm;
    confirm = QMessageBox::question(this, "Rename All", "Are you sure you want to rename all APKs? Be shure of what you're doing!", QMessageBox::Yes|QMessageBox::No);
    if(confirm==QMessageBox::Yes){
        renaming=true;
        for (int i = 0; i < ui->listWidget->count(); ++i) {
            if(ui->listWidget->item(i)->data(308).toString()!="Dir"){
                ApkPreview aux=qvariant_cast<ApkPreview>(ui->listWidget->item(i)->data(306));
                if(aux.RecomendedName()!=aux.ObtenerApkName()){
                    ui->listWidget->setCurrentItem(ui->listWidget->item(i));
                    on_actionRename_triggered();
                }
            }
        }
        renaming=false;
    }
}

void APKFilesViewer::on_actionDelete_All_Selected_triggered()
{
    QList<QListWidgetItem*> lista = ui->listWidget->selectedItems();

    int total=lista.length();
    for (int i =total -1 ; i > -1 ; i--) {
        if(lista.at(i)->data(308).toString() != "APK"){
            lista.removeAt(i);
        }
    }
    QMessageBox::StandardButton confirm;
    confirm = QMessageBox::question(this, "Delete All", "Are you sure you want to delete these "+QString::number(lista.length())+" APKs permanently?", QMessageBox::YesToAll|QMessageBox::Yes|QMessageBox::No);
    if(confirm==QMessageBox::Yes){
        ui->listWidget->clearSelection();
        for (int i = 0; i < lista.length(); ++i) {
            if(lista.at(i)->data(308).toString() == "APK"){
                ui->listWidget->setCurrentItem(lista.at(i));
                on_actionDelete_triggered();
            }
        }
    }else if(confirm==QMessageBox::YesToAll){
        yesToAll=true;
        for (int i = 0; i < lista.length(); ++i) {
            if(lista.at(i)->data(308).toString() == "APK"){
                ui->listWidget->setCurrentItem(lista.at(i));
                on_actionDelete_triggered();
            }
        }
        yesToAll=false;
    }
}



void APKFilesViewer::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if(previous && ui->listWidget->viewMode()== QListView::IconMode){
        previous->setSizeHint(QSize(78, 78));
    }
    if(current && ui->listWidget->viewMode()== QListView::IconMode){

        if(current->text().length()>11){
            QString aux=current->text();
            for (int i = 11; i < current->text().length(); i+=11) {
                aux=aux.insert(i,"\n");
            }
            ui->ltext->show();
            ui->ltext->setText(aux);
            ui->ltext->move(ui->listWidget->visualItemRect(current).x()+12,
                            ui->listWidget->visualItemRect(current).y()+ui->ltext->height() +65);

            if(t){
                t->stop();
                t->disconnect();
                delete t;
            }
            t= new QTimer();
            t->setInterval(5000);
            connect(t, SIGNAL(timeout()), this, SLOT(slot_textView()));
            t->start();
        }else{
            ui->ltext->hide();
        }

    }
}

void APKFilesViewer::on_tbView_clicked()
{
    if(listMode){
        ui->listWidget->setViewMode(QListView::IconMode);
        listMode=false;
    }else{
        ui->listWidget->setViewMode(QListView::ListMode);
        listMode=true;
    }
}


void APKFilesViewer::on_tbOrderAZ_clicked()
{
    ui->ltext->hide();
    if(sortAsc){
        ui->listWidget->sortItems(Qt::AscendingOrder);
        sortAsc=false;
    }else{
        ui->listWidget->sortItems(Qt::DescendingOrder);
        sortAsc=true;
    }

    for (int i = 0; i < ui->listWidget->count(); ++i) {
        if(ui->listWidget->item(i)->data(309).toString()==".."){
            QListWidgetItem *back=ui->listWidget->item(i)->clone();
            ui->listWidget->takeItem(i);
            ui->listWidget->insertItem(0,back);
            break;
        }
    }

    QList<QListWidgetItem*> folders;
    for (int i = ui->listWidget->count()-1; i > 0; i--) {
        if(ui->listWidget->item(i)->data(308).toString()=="Dir"){
            QListWidgetItem *folder=ui->listWidget->item(i)->clone();
            folders.append(folder);
            ui->listWidget->takeItem(i);
            //ui->listWidget->insertItem(1,folder);
        }
    }
    for (int k = 0; k < folders.count(); ++k) {
        ui->listWidget->insertItem(1,folders.at(k));
    }
}

void APKFilesViewer::on_tbUp_clicked()
{
    if(!ui->ePath->hasFocus()){
        Atras();
    }
}

void APKFilesViewer::on_ePath_returnPressed()
{
    if(ui->ePath->text()!=repository){
        if(QDir(ui->ePath->text()).exists()){
            repository=ui->ePath->text();
            OpenFolder();
        }else{
            QMessageBox::warning(this, "Error", "There is no existing directory for\n"+ui->ePath->text());
        }
    }
}



void APKFilesViewer::on_listWidget_itemActivated(QListWidgetItem *item)
{
    if(item->data(308).toString()=="Dir" || item->data(308).toString()=="HDD"){
        ui->tbRename->setEnabled(false);
        ui->tbReload->setEnabled(false);
        if(previewHilo->isRunning()){
            previewHilo->Cerrar();
            previewHilo->exit();
            previewHilo->disconnect();
            delete previewHilo;
        }
        if(item->data(309).toString()==".."){
            Atras();
        }else{
            repository=repository.replace("\\","/");
            repository=repository.replace("//", "/");
            if((repository.lastIndexOf("/")==2 && repository.length()==3)||repository.length()==2){
                if((repository.lastIndexOf(":")==1 && ((repository.lastIndexOf("/")==2 && item->data(309).toString().length()==3)|| repository.length()==2)) && ui->ePath->text()=="MY PC"){
                    repository=item->data(309).toString();
                }else{
                    repository=repository+item->data(309).toString();
                }

            }else if(item->data(308).toString()=="HDD"){
                repository=item->data(309).toString();
            }else{
                repository=repository+"/"+item->data(309).toString();
            }
            ui->ePath->setText(repository);
            OpenFolder();
        }

    }else if(item->data(308).toString()=="MYPC"){
        on_tbMiPC_clicked();
    }else{
        ApkInfoView *apkv=new ApkInfoView(repository+"/"+item->data(309).toString());
        connect(apkv, SIGNAL(UpdatePkgInfo(PkgInfo)), this, SLOT(slot_Renamed(PkgInfo)));
        apkv->show();
    }
}



void APKFilesViewer::on_tbSelectAll_clicked()
{
    ui->ePath->setFocus();
    ui->ePath->setSelection(0,ui->ePath->text().length());
}

void APKFilesViewer::on_tbSelectAll_2_clicked()
{
    QProcess cmd;
    QString command="explorer "+repository;
    cmd.start("cmd.exe /c " +command.replace("/","\\"));
    cmd.waitForFinished(-1);
}
