#ifndef APKFILESVIEWER_H
#define APKFILESVIEWER_H

#include <QDialog>
#include <QFile>
#include <QDir>
#include <previewHilo.h>
#include <QIcon>
#include <QPixmap>
#include <QListWidgetItem>
#include <listaSE.h>
#include <apkpreview.h>
#include <newapkfile.h>
#include <QFileDialog>
#include <dataccess.h>
#include <QMenu>
#include <QPoint>
#include <apkinfoview.h>
#include <pkginfo.h>
#include <QTimer>
#include <QWheelEvent>
#include <QProcess>

namespace Ui {
class APKFilesViewer;
}

class APKFilesViewer : public QDialog
{
    Q_OBJECT

public:
    explicit APKFilesViewer(QWidget *parent = 0);
    ~APKFilesViewer();

private slots:
    void slot_Preview(ApkPreview prev);
    void PopUp(const QPoint& pos);
    void slot_PreviewFinish();
    void slot_Renamed(PkgInfo pkg);
    void slot_textView();

    void on_actionRename_triggered();

    void on_tbOpen_clicked();

    void on_listWidget_currentRowChanged(int currentRow);


    void on_listWidget_itemSelectionChanged();

    void on_tbReload_clicked();

    void on_actionDelete_triggered();

    void on_tbMiPC_clicked();

    void on_tbRename_clicked();

    void on_actionDelete_All_Selected_triggered();

    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_tbView_clicked();

    void on_tbOrderAZ_clicked();

    void on_tbUp_clicked();

    void on_ePath_returnPressed();


    void on_listWidget_itemActivated(QListWidgetItem *item);

    void on_tbSelectAll_clicked();

    void on_tbSelectAll_2_clicked();

private:
    Ui::APKFilesViewer *ui;
    ListaSE<NewAPKFile> files;
    ListaSE<CPreviewHilo*> prevs;
    CPreviewHilo *previewHilo;

    QString repository;
    bool sortAsc;
    bool renaming;
    bool yesToAll;
    bool listMode;

    QTimer *t;

    void SelectFolder();
    void OpenFolder();
    void Atras();

    void wheelEvent(QWheelEvent *e);

};

#endif // APKFILESVIEWER_H
