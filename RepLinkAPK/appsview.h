#ifndef APPSVIEW_H
#define APPSVIEW_H

#include <QDialog>
#include <QVariant>
#include <QIcon>
#include <QPixmap>
#include <QMenu>
#include <QPoint>
#include <cxmlmethods.h>
#include <listHilo.h>
#include <QListWidgetItem>
#include <appinfo.h>
#include <pkglistview.h>
#include <manualdescription.h>
#include <QMessageBox>
#include <sortHilo.h>
#include <apkpreview.h>
#include <previewHilo.h>
#include <newapkfile.h>


namespace Ui {
class AppsView;
}

class AppsView : public QDialog
{
    Q_OBJECT

public:
    explicit AppsView(QString repository, CXMLMethods *xmlM, QIcon repoIcon, QWidget *parent = 0);
    ~AppsView();

private slots:
    void slot_UpdateApp(AppInfo app);
    void slot_GetApp(AppInfo app);
    void slot_UpdatePkgs(ListaSE<PkgInfo> pkgs);
    void slot_UpdatePkgs(int pos);
    void slot_Preview(ApkPreview prev);

    void DestroyHilo();
    void DestroyPkgs(int result);
    void Porciento(int porc);
    void on_lineEdit_textChanged(const QString &arg1);

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_AppsView_finished(int result);

    void PopUp(const QPoint& pos);

    void on_actionViewPackages_triggered();

    void on_actionDeleteApp_triggered();

    void on_orderAlph_clicked();

    void on_orderCantPkg_clicked();


private:
    Ui::AppsView *ui;
    CListHilo *getApps;
    manualDescription *mdesc;
    PkgListView *pkglv;
    CPreviewHilo *previewHilo;
    QString repository;
    CXMLMethods *xmlM;
    bool sortAsc;
    bool sortPkgs;
    SortHilo *sortH;
    void ReCount();

};

#endif // APPSVIEW_H
