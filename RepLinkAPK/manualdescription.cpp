#include "manualdescription.h"
#include "ui_manualdescription.h"

manualDescription::manualDescription(QString repo, AppInfo &app, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::manualDescription)
{
    ui->setupUi(this);
    this->repository=repo;
    this->appAct=app;


    QString error="";
    QString cat=repository+"/categories.txt";
    QFile* file = new QFile(cat);

    QPixmap pixmap;
    QIcon ico;

    ui->lCopiando->hide();
    pixmap.load(repository+"/icons/"+appAct.ObtenerId()+".png");

    if(pixmap.width()> 110 || pixmap.height()> 110 ){
        ico.addPixmap(pixmap.scaled(110,110,Qt::KeepAspectRatioByExpanding));
        ui->lIcon->setPixmap(ico.pixmap(110,110));
    }else{
        ico.addPixmap(pixmap);
        ui->lIcon->setPixmap(pixmap);
    }

    this->setWindowTitle(appAct.ObtenerId());
    this->setWindowIcon(ico);

    if (!file->open(QFile::ReadOnly | QFile::Text)) {
        error=QString("Can\'t read file %1:  \n%2.").arg(cat).arg(file->errorString());
        //std::cout<< error.toStdString()<< std::endl;
        QMessageBox::warning(this, "Error", error, QMessageBox::Ok);

    }else {
        QByteArray textFile=file->readAll();
        QString auxText=QString().fromStdString(textFile.toStdString());
        QStringList lines=auxText.split("\n");
        for (int var = 0; var < lines.length(); ++var) {
            auxText=lines.at(var).trimmed();
            if(!auxText.isEmpty()){
                ui->cbCategory->addItem(auxText);
            }
        }
    }

    ui->cbCategory->setCurrentText(appAct.ObtenerCategory());
    ui->eCategories->setText(appAct.ObtenerCategories());
    ui->cbLicense->setCurrentText(appAct.ObtenerLicense());
    ui->eName->setText(appAct.ObtenerName());
    ui->eSummary->setText(appAct.ObtenerSummary());
    ui->cRoot->setChecked(appAct.ObtenerRequirements()=="root");

    if(appAct.ObtenerSource().trimmed()!=""){
        ui->gbSource->setChecked(true);
        QString protocol= appAct.ObtenerSource().trimmed().split("://").at(0);
        for (int k = 0; k < ui->comboBox->count(); ++k) {
            if(ui->comboBox->itemText(k)==protocol+"://"){
                ui->comboBox->setCurrentIndex(k);
                break;
            }
        }

        QStringList partes=appAct.ObtenerSource().trimmed().split("://").at(1).split("/");
        ui->eSource->setText(partes.last());
        partes.removeLast();
        ui->eWeb->setText(partes.join("/"));
        QString url =ui->comboBox->currentText()+ui->eWeb->text().replace("http://","").replace("https://","").replace("ftp://","").replace("://","")+"/"+ui->eSource->text();
        ui->lUrl->setText(url);
    }

    if(appAct.ObtenerPackages().Longitud()>0){
        for (int pack=1;pack<=appAct.ObtenerPackages().Longitud();pack++) {
            ui->cbVersion->addItem(appAct.ObtenerPackages().ObtenerPorPos(pack).ObtenerVersion()+" ("+appAct.ObtenerPackages().ObtenerPorPos(pack).ObtenerVersionCode()+")");
        }
        ui->pushButtonMain->setEnabled(false);
        ui->pushButtonPatch->setEnabled(false);
    }else{
        ui->gbDatabase->setEnabled(false);
    }

    QString document=appAct.ObtenerDescription();
    document.replace("&lt;p&gt;","<p>").replace("&lt;/p&gt;","</p>");
    ui->tDesc->setHtml(document);
}

manualDescription::~manualDescription()
{
    delete ui;
}

void manualDescription::on_pushButton_clicked()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       "RepLink APK Manager", "RepLinkAPK");
    QString data=settings.value("AppDirs/data").toString();

    QFileDialog abrir;
    QString manejador= abrir.getOpenFileName(this, tr("Abrir Comprimido"), (data.trimmed().isEmpty()?repository:data), tr("Comprimido(*.zip)\nAny File(*.*)"));
    QString url=manejador.toUtf8();
    if(!url.trimmed().isEmpty()){



        if(!data.trimmed().isEmpty()){
            QDir d(settings.value("AppDirs/data").toString());
            if(d.exists() && (d.absolutePath()!= QFileInfo(url).absoluteDir().absolutePath())){
                QMessageBox::StandardButton confirm;
                confirm = QMessageBox::question(this, "Copy to Data", "These ZIP file isn't in the defined Data Directory. Do you want to copy it?", QMessageBox::Yes|QMessageBox::No);
                if(confirm == QMessageBox::Yes){
                    QFile f(url);
                    ui->lCopiando->setVisible(true);
                    ui->lCopiando->repaint();
                    if(f.copy(d.absolutePath()+"/"+appAct.ObtenerId()+".zip")){
                        url=d.absolutePath()+"/"+appAct.ObtenerId()+".zip";
                        ui->lCopiando->setVisible(false);
                    }else{
                        ui->lCopiando->setVisible(false);
                        QMessageBox::warning(this, "Error", "Couldn't copy the file!", QMessageBox::Ok);
                    }
                }
            }
        }


        QStringList partes= url.split("/");
        QString source=partes.last();
        ui->eSource->setText(source);
        QString url =ui->comboBox->currentText()+ui->eWeb->text().replace("http://","").replace("https://","").replace("ftp://","").replace("://","")+"/"+ui->eSource->text();
        ui->lUrl->setText(url);
    } else {
        ui->eSource->setText("");
        ui->gbSource->setChecked(false);
    }
}

void manualDescription::on_buttonBox_accepted()
{

    QString name=ui->eName->text().trimmed();
    QString desc=ui->tDesc->toPlainText().trimmed();

    QString sum=ui->eSummary->text().trimmed().isEmpty()?"Unknown":ui->eSummary->text().trimmed();

    if(ui->checkBox->isChecked()){
        FormatTexts(name,sum,desc);
    }

    if(desc.isEmpty() || desc=="."){
        desc="<p>Unknown.</p>";
    }else{
        QStringList descs= desc.split("\n");
        desc="";
        for (int i = 0; i < descs.length(); ++i) {
            desc+="<p>"+descs.at(i)+"</p>";
        }
    }

    appAct.SetName(name);
    appAct.SetSummary(sum);
    appAct.SetDescription(desc);

    appAct.SetCategory(ui->cbCategory->currentText().trimmed().isEmpty()?"Unknown":ui->cbCategory->currentText().trimmed());
    appAct.SetCategories(ui->eCategories->text().trimmed().isEmpty()?appAct.ObtenerCategory():ui->eCategories->text().trimmed());
    appAct.SetLicense(ui->cbLicense->currentText().trimmed().isEmpty()?"Unknown":ui->cbLicense->currentText().trimmed());
    appAct.SetRequirements(ui->cRoot->isChecked()?"root":"");

    if(appAct.ObtenerPackages().Longitud()>0 && ui->cbVersion->currentIndex()!=0){
        QString versionCodeObb= appAct.ObtenerPackages().ObtenerPorPos(ui->cbVersion->currentIndex()).ObtenerVersionCode();
        appAct.SetVersionCodeToObb(versionCodeObb);
        appAct.SetMainObb(ui->eMainObb->text());
        appAct.SetPatchObb(ui->ePatchObb->text());
    }

    if(ui->gbSource->isChecked()){
        QString url =ui->comboBox->currentText()+ui->eWeb->text().replace("http://","").replace("https://","").replace("ftp://","").replace("://","")+"/"+ui->eSource->text();
        appAct.SetSource(url);
    }else{
        appAct.SetSource("");
    }

    emit UpdateAppInfo(appAct);

    emit accepted();

}

void manualDescription::on_eName_editingFinished()
{
    if(ui->eName->text().isEmpty()){
        ui->buttonBox->buttons().at(0)->setEnabled(false);
    }else{
        ui->buttonBox->buttons().at(0)->setEnabled(true);
    }
}

void manualDescription::FormatTexts(QString &name, QString &summary, QString &desc)
{
    name=name.trimmed();
    summary=summary.trimmed();//.toLower();
    desc=desc.trimmed();//.toLower();


    QChar first;
    QStringList partes=summary.split(" ");
    for (int x = 0; x < partes.length(); ++x) {
        if(partes.at(x).isEmpty()){
            partes.removeAt(x);
            x--;
        }
    }
    summary=partes.join(" ");

    partes=desc.split(" ");
    for (int x = 0; x < partes.length(); ++x) {
        if(partes.at(x).isEmpty()){
            partes.removeAt(x);
            x--;
        }
    }
    desc=partes.join(" ");

    QString temp;
    partes=name.split(" ");
    for (int x = 0; x < partes.length(); ++x) {
        if(partes.at(x).isEmpty()){
            partes.removeAt(x);
            x--;
        }else{
            first= partes.at(x).at(0).toUpper();
            temp=partes.at(x);
            temp.remove(0,1);
            temp.insert(0,first);
            partes.replace(x,temp);
        }
    }
    name=partes.join(" ");

    while(name.at(name.length()-1)=='.'){
        name.remove(name.length()-1,1);
    }

    while(summary.at(summary.length()-1)=='.'){
        summary.remove(summary.length()-1,1);
    }
    if(!desc.isEmpty()){


        temp="";
        partes=desc.split("\n");
        QStringList tempDesc;
        for (int y = 0; y < partes.length(); ++y) {
            if(!partes.at(y).isEmpty()){
                temp=partes.at(y);
                first= temp.at(0).toUpper();
                temp.remove(0,1);
                temp.insert(0,first);

                while(temp.at(temp.length()-1)=='.'||temp.at(temp.length()-1)==' '){
                    temp.remove(temp.length()-1,1);
                }

                tempDesc.append(temp+".");
            }
        }
        desc=tempDesc.join("\n");
    }
    first= summary.at(0).toUpper();
    summary.remove(0,1);
    summary.insert(0,first);

    //summary.insert(summary.length(),'.');
}

void manualDescription::on_eWeb_textChanged(const QString &arg1)
{
    QString aux=arg1;
    QString url =ui->comboBox->currentText()+aux.replace("http://","").replace("https://","").replace("ftp://","").replace("://","")+"/"+ui->eSource->text();
    ui->lUrl->setText(url);
}

void manualDescription::on_gbSource_clicked(bool checked)
{
    if(checked && ui->eSource->text().isEmpty()){
        on_pushButton_clicked();
    }
}

void manualDescription::on_cbCategory_activated(const QString &arg1)
{
    if(ui->eCategories->text().indexOf(arg1)==-1){
        ui->eCategories->setText(arg1+","+ui->eCategories->text());
    }
}

void manualDescription::on_toolButton_clicked()
{
    if(ui->eCategories->text().indexOf("Recomendadas")==-1){
        ui->eCategories->setText(ui->eCategories->text()+",Recomendadas");
    }
}

void manualDescription::on_cbVersion_activated(int index)
{
    if(index==0){
        ui->eMainObb->setText("");
        ui->ePatchObb->setText("");
        ui->pushButtonMain->setEnabled(false);
        ui->pushButtonPatch->setEnabled(false);
    }else{
        ui->eMainObb->setText(appAct.ObtenerPackages().ObtenerPorPos(index).ObtenerMainObb());
        ui->ePatchObb->setText(appAct.ObtenerPackages().ObtenerPorPos(index).ObtenerPatchObb());
        ui->pushButtonMain->setEnabled(true);
        ui->pushButtonPatch->setEnabled(true);

    }

}

void manualDescription::on_pushButtonMain_clicked()
{
    QString filter= "Obb(main.*" + this->appAct.ObtenerId() + ".obb)\nAny File(*.*)";
    QFileDialog abrir;
    QString manejador= abrir.getOpenFileName(this, tr("Abrir Obb"), (repository), filter);
    QString url=manejador.toUtf8();
    if(!url.trimmed().isEmpty()){

        QStringList partes= url.split("/");
        QString mainObb=partes.last();


        QDir d(this->repository);
        if(d.exists() && (d.absolutePath()!= QFileInfo(url).absoluteDir().absolutePath())){
            QMessageBox::StandardButton confirm;
            confirm = QMessageBox::question(this, "Copy to Data", "These OBB file isn't in the defined Repo Directory. Do you want to copy it?", QMessageBox::Yes|QMessageBox::No);
            if(confirm == QMessageBox::Yes){
                QFile f(url);
                ui->lCopiando->setVisible(true);
                ui->lCopiando->repaint();
                if(f.copy(d.absolutePath()+"/"+mainObb)){
                    url=d.absolutePath()+"/"+mainObb;
                    ui->lCopiando->setVisible(false);
                }else{
                    ui->lCopiando->setVisible(false);
                    QMessageBox::warning(this, "Error", "Couldn't copy the file!", QMessageBox::Ok);
                }
            }
        }




        ui->eMainObb->setText(mainObb);
    } else {
        ui->eMainObb->setText("");
    }
}

void manualDescription::on_pushButtonPatch_clicked()
{
    QString filter= "Obb(patch.*" + this->appAct.ObtenerId() + ".obb)\nAny File(*.*)";
    QFileDialog abrir;
    QString manejador= abrir.getOpenFileName(this, tr("Abrir Obb"), (repository), filter);
    QString url=manejador.toUtf8();
    if(!url.trimmed().isEmpty()){

        QStringList partes= url.split("/");
        QString patchObb=partes.last();


        QDir d(this->repository);
        if(d.exists() && (d.absolutePath()!= QFileInfo(url).absoluteDir().absolutePath())){
            QMessageBox::StandardButton confirm;
            confirm = QMessageBox::question(this, "Copy to Data", "These OBB file isn't in the defined Repo Directory. Do you want to copy it?", QMessageBox::Yes|QMessageBox::No);
            if(confirm == QMessageBox::Yes){
                QFile f(url);
                ui->lCopiando->setVisible(true);
                ui->lCopiando->repaint();
                if(f.copy(d.absolutePath()+"/"+patchObb)){
                    url=d.absolutePath()+"/"+patchObb;
                    ui->lCopiando->setVisible(false);
                }else{
                    ui->lCopiando->setVisible(false);
                    QMessageBox::warning(this, "Error", "Couldn't copy the file!", QMessageBox::Ok);
                }
            }
        }




        ui->ePatchObb->setText(patchObb);
    } else {
        ui->ePatchObb->setText("");
    }
}
