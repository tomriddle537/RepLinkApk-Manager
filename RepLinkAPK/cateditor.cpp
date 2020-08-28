#include "cateditor.h"
#include "ui_cateditor.h"

CatEditor::CatEditor(QString repo, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CatEditor)
{
    ui->setupUi(this);
    ui->buttonBox->buttons().at(0)->setText("Save");
    repository=repo;
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(PopUp(QPoint)));

    OpenCatEditor();
    ReCount();
}

void CatEditor::OpenCatEditor(){
    QString error="";
    QString cat=repository+"/categories.txt";
    QFile* file = new QFile(cat);

    if (!file->open(QFile::ReadOnly | QFile::Text)) {
        this->setEnabled(false);
        this->close();
        error=QString("Can\'t read file %1:  \n%2.").arg(cat).arg(file->errorString());
        QMessageBox::warning(this, "Error", error, QMessageBox::Ok);
        // std::cout<< error.toStdString()<< std::endl;
    }else {
        this->show();

        QByteArray textFile=file->readAll();
        QString auxText=QString().fromStdString(textFile.toStdString());
        QStringList lines=auxText.split("\n");
        for (int var = 0; var < lines.length(); ++var) {
            auxText=lines.at(var).trimmed();
            if(!auxText.isEmpty()){
                ui->listWidget->addItem(lines.at(var));
            }
        }
    }
}

CatEditor::~CatEditor()
{
    delete ui;
}

void CatEditor::on_buttonBox_accepted()
{
    QString fileName=repository+"/categories.txt";
    QFile file(QDir::toNativeSeparators(fileName));
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Categories"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
    }else {

        QTextStream out(&file);
        QStringList lineas;
        for (int i = 0; i < ui->listWidget->count(); ++i) {
            lineas << ui->listWidget->item(i)->text();
        }

        out << lineas.join("\n");
    }
}


void CatEditor::on_lineEdit_textChanged(const QString &arg1)
{
    if(arg1.trimmed().isEmpty()){
        ui->tbAdd->setEnabled(false);
    }else{
        ui->tbAdd->setEnabled(true);
    }
}

void CatEditor::on_tbAdd_clicked()
{
    QString cattemp=ui->lineEdit->text();
    QString temp;
    QChar first;
    QStringList partes=cattemp.split(" ");
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
    cattemp=partes.join(" ");
    ui->listWidget->addItem(cattemp);
    ui->lineEdit->setText("");
    ReCount();
}

void CatEditor::PopUp(const QPoint &pos)
{
    if(ui->listWidget->selectedItems().count()>0 && ui->listWidget->indexAt(pos).isValid()){
        QPoint globalPos= ui->listWidget->mapToGlobal(pos);
        QMenu popupMenu;
        popupMenu.addAction(ui->actionRemove);
        popupMenu.exec(globalPos);
    }
}

void CatEditor::ReCount()
{
    int cant=ui->listWidget->count();
    ui->lCount->setText(QString::number(cant));
    ui->lineEdit->setFocus();
}


void CatEditor::on_actionRemove_triggered()
{
    if(ui->listWidget->currentRow()!=-1){
        ui->listWidget->takeItem(ui->listWidget->currentRow());
        ReCount();
    }
}
