#include "cabout.h"
#include "ui_cabout.h"

CAbout::CAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CAbout)
{
    ui->setupUi(this);

}

CAbout::~CAbout()
{
    delete ui;
}
