#include "catchooser.h"
#include "ui_catchooser.h"

CatChooser::CatChooser(QString apkname, QString repository, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CatChooser)
{
    ui->setupUi(this);

    QFile *file;
    QString cat=repository+"/categories.txt";
    file = new QFile(cat);
    QListWidgetItem *newItem;

    if (!file->open(QFile::ReadOnly | QFile::Text)) {
        QString error=QString("Can\'t read file %1:  \n%2.").arg(cat).arg(file->errorString());
        //std::cout<< error.toStdString()<< std::endl;
        QMessageBox::warning(this, "Error", error, QMessageBox::Ok);
        this->setEnabled(false);
        this->close();
        delete file;
    }else {
        this->setWindowTitle(apkname+" - Category Chooser");
        //this->setWindowIcon(ico);

        QByteArray textFile=file->readAll();
        QString auxText=QString().fromStdString(textFile.toStdString());
        QStringList lines=auxText.split("\n");
        ui->listWidget->clear();
        for (int var = 0; var < lines.length(); ++var) {
            auxText=lines.at(var).trimmed();
            if(!auxText.isEmpty()){
                newItem = new QListWidgetItem();

                //newItem->setSizeHint(QSize(100, 30));
                newItem->setText(auxText);
                newItem->setTextAlignment(Qt::AlignCenter);


                ui->listWidget->addItem(newItem);
            }
        }
    }


}

QString CatChooser::SelectedCategory()
{
    return this->category;
}

CatChooser::~CatChooser()
{
    delete ui;
}

void CatChooser::on_buttonBox_rejected()
{
    category="Uncategorized";
    emit CatForAPKs(category);
}

void CatChooser::on_listWidget_itemActivated(QListWidgetItem *item)
{
    QString wTitle=this->windowTitle();
    category=item->text();
    if(wTitle=="Selected APKs - Category Chooser"){
        emit CatForAPKs(category);
    } else{
        emit Cat(category);
    }
    this->hide();
}
