#include "processing.h"
#include "ui_processing.h"

processing::processing(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::processing)
{
    ui->setupUi(this);
    CreateTaskbar();
    QMovie *movie = new QMovie(":/img/cube.gif");
    movie->setScaledSize(ui->lLoading->size());
    ui->lLoading->setMovie(movie);
    ui->lLoading->setGeometry(ui->lLoading->x(), ui->lLoading->y()-13, ui->lLoading->size().width(), ui->lLoading->size().height());
    movie->start();
}

processing::~processing()
{
    delete ui;
}

void processing::slot_UpdateInfo(QPixmap pixmap)
{
    ico.addPixmap(pixmap);
    if(pixmap.width()> 110 || pixmap.height()> 110 ){
        ico.addPixmap(pixmap.scaled(110,110,Qt::KeepAspectRatioByExpanding));
        ui->lIcon->setPixmap(ico.pixmap(110,110));
        ui->lIcon->setGeometry((int)(this->size().width()/2 - 110/2), 100, 110, 110);
    }else{
        ico.addPixmap(pixmap);
        ui->lIcon->setPixmap(pixmap);
        ui->lIcon->setGeometry((int)(this->size().width()/2 - pixmap.size().width()/2), 100, ico.pixmap(pixmap.size()).size().width(), ico.pixmap(pixmap.size()).size().height());
    }
}

void processing::slot_UpdatePorc(int porc)
{

    ui->progressBar->setValue(porc);
}

void processing::keyPressEvent(QKeyEvent *e){
    if(e->key()!=Qt::Key_Escape) QDialog::keyPressEvent(e);
}

void processing::CreateTaskbar()
{


}
