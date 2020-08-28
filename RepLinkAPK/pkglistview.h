#ifndef PKGLISTVIEW_H
#define PKGLISTVIEW_H

#include <QDialog>
#include <QIcon>
#include <QPixmap>
#include <QMenu>
#include <QPoint>
#include <QFileInfo>
#include <appinfo.h>
#include <listaSE.h>
#include <QVariant>
#include <QListWidgetItem>
#include <cxmlmethods.h>
#include <QDebug>
#include <apkinfoview.h>
#include <QMessageBox>

namespace Ui {
class PkgListView;
}

class PkgListView : public QDialog
{
    Q_OBJECT

signals:
    void UpdatePkgs(ListaSE<PkgInfo>);
    void DeletePkg(int);

public:
    explicit PkgListView(QString repository, QString appname, ListaSE<PkgInfo> packages, CXMLMethods *xmlM, QWidget *parent = 0);
    ~PkgListView();

private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void slot_UpdatePkg(PkgInfo pkg);

    void PopUp(const QPoint& pos);

    void on_actionDeletePkg_triggered();

private:
    Ui::PkgListView *ui;
    QString repository;
    CXMLMethods *xmlM;
    ApkInfoView *apki;


};

#endif // PKGLISTVIEW_H
