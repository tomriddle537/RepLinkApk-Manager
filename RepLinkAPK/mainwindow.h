#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QThread>
#include <iostream>
#include <QSysInfo>
#include <QMainWindow>
#include <QDebug>
#include <QWinTaskbarButton>
#include <QWinTaskbarProgress>
#include <QDomDocument>
#include <QPixmap>
#include <QFileDialog>
#include <QDate>
#include <cxmlmethods.h>
#include <QDir>
#include <QIcon>
#include <QFile>
#include <QMenu>
#include <QPoint>
#include <procHilo.h>
#include <QFileInfo>
#include <QMessageBox>
#include <QException>
#include <listaSE.h>
#include <catchooser.h>
#include <cabout.h>
#include <cxmlpackage.h>
#include <appsview.h>
#include <apkinfoview.h>
#include <cxmlapp.h>
#include <apkreport.h>
#include <caaptparser.h>
#include <processing.h>
#include <newapkfile.h>
#include <crepomanager.h>
#include <repoinfo.h>
#include <QProcess>
#include <QByteArray>
#include <updateHilo.h>
#include <keystoreinfo.h>
#include <cconfig.h>
#include <countHilo.h>
#include <previewHilo.h>
#include <QKeyEvent>
#include <apkfilesviewer.h>
#include <backupHilo.h>
#include <jsonHilo.h>
#include <tohtml.h>

namespace Ui {
class MainWindow;
}
class CProcHilo;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void Progress(int);
    void Image(QPixmap);


private slots:
    void slot_ImagenAct(QPixmap pix);
    void slot_Buzon(QString msg,int type);
    void slot_NewCat(QString cat);
    void slot_RemotesNewCat(QString cat);
    void slot_Recent();
    void slot_SaveHovered();
    void slot_RepoManager(int);
    void slot_RemoteFiles(QStringList files);
    void slot_UpdateRepoInfo(QString cantApps, QString cantPkg, CXMLRepo xmlRepo);
    void slot_AppPerCat(QStringList appPerCat);
    void slot_Preview(ApkPreview prev);
    void slot_Porc(int);
    void slot_BackupPorc(int porc, QString category, QString apkname);
    void slot_GenerateJsonPorc(int porc);
    void slot_ToHtmlPorc(int porc, QString app, QString apkname);

    void PopUp(const QPoint& pos);



    void DestroyView(int result);
    void DestroyFilesView(int result);
    void DestroyReport(int result);
    void DestroyViewInfo(int result);
    void DestroyConfig(int result);
    void DestroyHilo();
    void DestroyHiloBackup();
    void DestroyHiloGenerateJson();
    void DestroyHiloToHtml();





    void on_bActApks_clicked();

    void on_bProcess_clicked();

    void on_actionOpen_repository_2_triggered();

    void on_actionExit_triggered();


    void on_rbDefault_clicked(bool checked);

    void on_actionApps_in_repository_triggered();

    void on_actionApk_report_triggered();

    void on_actionAbout_triggered();

    void on_actionApk_Info_View_triggered();

    void on_bClearSendedApps_clicked();

    void on_lwNewApps_itemDoubleClicked(QListWidgetItem *item);


    void on_cbCategories_currentIndexChanged(const QString &arg1);

    void on_actionRepositories_manager_triggered();

    void on_actionSign_Repo_triggered();

    void on_actionAll_Packages_triggered();

    void on_actionOpen_APK_files_triggered();

    void on_toolButton_clicked();

    void CatRemoteFiles(QStringList files, QString cat);

    void on_actionRemove_triggered();

    void on_actionMerge_repo_triggered();

    void on_actionClose_repo_triggered();

    void on_actionAuto_Rename_APKs_triggered(bool checked);

    void on_lKeyStore_clicked();

    void on_actionOptions_triggered();

    void on_actionApps_per_Categoy_count_triggered();


    void on_actionAuto_Update_triggered(bool checked);

    void on_actionPreviews_triggered(bool checked);

    void on_actionRename_triggered();

    void on_actionAPK_Files_Viewer_triggered();



    void on_actionBackUp_Repo_triggered();


    void on_actionCancel_BackUp_triggered();

    void on_actionTo_Web_Links_triggered();


    void on_actionSubstract_repo_triggered();

    void on_actionSummary_like_Category_triggered();

    void on_actionGenerate_JSON_triggered();

    void on_actionBackUp_APKs_Too_triggered(bool checked);

private:
    QString repository;
    QString salida;
    QString hoveredAct;
    ListaSE<NewAPKFile> files;
    QString xmlFilePath;
    Ui::MainWindow *ui;
    CXMLMethods *xmlM;
    CBinOutputParser *bop;
    CAbout *about;
    AppsView *appv;
    ApkInfoView *apkiv;
    ApkReport *apkr;
    CRepoManager *repoM;
    CProcHilo *procHilo;
    CCountHilo *countHilo;
    CUpdateHilo *updateHilo;
    CBackupHilo *backupHilo;
    CJSONHilo *jsonHilo;
    CToHtlmHilo *toHtml;
    CPreviewHilo *previewHilo;
    CatChooser *cc;
    CConfig *config;
    QIcon progIco;
    QMovie *movie;
    manualDescription *mdesc;
    QWinTaskbarButton *taskbarButton;
    QWinTaskbarProgress *taskbarProgress;
    processing *procStatus;
    QDomDocument domDocument;
    QIcon repoIcon;
    ListaSE<RepoInfo> repos;
    QStringList remotes;
    APKFilesViewer *apkFilesV;
    int cantAPK;
    bool remoteFilesList;
    void LoadMYRepos();
    void OpenRepository();
    void UpdateAppsList();
    void CloseRepo();
    void CheckSig();
    QString CheckName(QString name);

    void keyPressEvent(QKeyEvent *e);
};

#endif // MAINWINDOW_H
