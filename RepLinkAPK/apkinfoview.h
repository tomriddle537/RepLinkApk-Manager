#ifndef APKINFOVIEW_H
#define APKINFOVIEW_H

#include <QDialog>
#include <cxmlmethods.h>
#include <QProcess>
#include <QDir>
#include <QIcon>
#include <QPixmap>
#include <QFileInfo>
#include <appinfo.h>
#include <QDebug>
#include <QFileDialog>
#include <caaptparser.h>
#include <pkginfo.h>
#include <listaSE.h>
#include <dataccess.h>
#include <QMessageBox>


namespace Ui {
class ApkInfoView;
}

class ApkInfoView : public QDialog
{
    Q_OBJECT

public:
    explicit ApkInfoView(QString repository, CXMLMethods *xmlM, QWidget *parent = 0);
    explicit ApkInfoView(QString url, QString repository,  CXMLMethods *xmlM, QWidget *parent = 0);
    explicit ApkInfoView(QString url, QWidget *parent = 0);
    ApkInfoView();

    ~ApkInfoView();

signals:
    void Files(QStringList);
    void UpdatePkgInfo(PkgInfo);

private slots:
    void slot_ImagenAct(QPixmap pix);
    void on_bRename_clicked();

    void on_bCopyToRepo_clicked();

    void on_bSendProcess_clicked();


    void on_tbNext_clicked();

    void on_tbPrev_clicked();

    void on_tbLog_clicked();

private:
    Ui::ApkInfoView *ui;
    QString repository;
    CXMLMethods *xmlM;
    CBinOutputParser *bop;
    QString url;
    QString file;
    QString log;
    QIcon ico;
    QFile *xfile;
    bool copiando;
    bool inXML;
    bool direct;

    void ApkView();
    QString AndroidVersion(QString sdk);
    void UpdateButtons(QString newName);
    void Clear();
    void PrepareIndex(int value);
};

#endif // APKINFOVIEW_H
