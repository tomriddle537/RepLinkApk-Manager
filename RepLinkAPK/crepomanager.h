#ifndef CREPOMANAGER_H
#define CREPOMANAGER_H

#include <QDialog>
#include <dataccess.h>
#include <repoinfo.h>
#include <listaSE.h>
#include <QPixmap>
#include <QIcon>
#include <QMenu>
#include <QPoint>
#include <QVariant>
#include <QMessageBox>
#include <QListWidgetItem>
#include <repoeditorview.h>
#include <QFileDialog>
#include <caaptparser.h>
#include <QProcess>
#include <QByteArray>


namespace Ui {
class CRepoManager;
}

class CRepoManager : public QDialog
{
    Q_OBJECT

public:
    explicit CRepoManager(QWidget *parent = 0);
    ~CRepoManager();

private slots:
    void slot_RepoEditor(int result);
    void slot_Buzon(QString msg,int type);

    void PopUp(const QPoint& pos);

    void on_buttonBox_rejected();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_actionDeleteRepo_triggered();

    void on_tbRefresh_clicked();

    void on_tbAdd_clicked();

private:
    Ui::CRepoManager *ui;
    ListaSE<RepoInfo> repos;
    DatAccess bd;
    repoeditorview *rEditor;
    CXMLMethods *xmlM;
    CBinOutputParser *bop;

    void LoadRepos();
};

#endif // CREPOMANAGER_H
