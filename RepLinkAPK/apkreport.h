#ifndef APKREPORT_H
#define APKREPORT_H

#include <QDialog>
#include <cxmlmethods.h>
#include <QIcon>
#include <QMenu>
#include <QPoint>
#include <QProcess>
#include <QSettings>
#include <QDir>
#include <QFileInfo>
#include <QAbstractButton>
#include <listaSE.h>
#include <QMessageBox>
#include <QMovie>
#include <reportHilo.h>



namespace Ui {
class ApkReport;
}

class ApkReport : public QDialog
{
    Q_OBJECT

public:
    explicit ApkReport(QString repository, CXMLMethods *xmlM, QIcon repoIcon, QWidget *parent = 0);
    ~ApkReport();

signals:
    void Files(QStringList);

private slots:
    void slot_GetReport(QString dobles,QStringList unused, QStringList missed);

    void on_actionDelete_triggered();

    void PopUp(const QPoint& pos);

    void on_toolButton_2_clicked();

    void on_tbRefresh_clicked();

    void on_tbClose_clicked();

    void on_tbDelete_clicked();

private:
    Ui::ApkReport *ui;
    QString repository;
    CXMLMethods *xmlM;
    QStringList existFiles;
    CReportHilo *reportHilo;
    bool yesToAll;

    void OpenRepo();
    void Report();

    ListaSE<QStringList> ExactSizeApps();

};

#endif // APKREPORT_H
