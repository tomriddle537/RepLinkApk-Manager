#ifndef REPOEDITORVIEW_H
#define REPOEDITORVIEW_H

#include <QDialog>
#include <repoinfo.h>
#include <cxmlrepo.h>
#include <cxmlmethods.h>
#include <QProcess>
#include <QDir>
#include <QIcon>
#include <QPixmap>
#include <QFileInfo>
#include <QDebug>
#include <QFileDialog>
#include <dataccess.h>
#include <cateditor.h>
#include <QMessageBox>


namespace Ui {
class repoeditorview;
}

class repoeditorview : public QDialog
{
    Q_OBJECT

public:
    explicit repoeditorview(QString repository, int id, QWidget *parent = 0);
    ~repoeditorview();

private slots:
    void on_buttonBox_accepted();

    void on_pushButton_2_clicked();


    void on_bBackupDir_clicked();

private:
    Ui::repoeditorview *ui;
    CXMLMethods *xmlM;
    CatEditor *catE;
    RepoInfo rInfo;
    QString repository;
    int id;
    QIcon repoIcon;
    DatAccess bd;

};

#endif // REPOEDITORVIEW_H
