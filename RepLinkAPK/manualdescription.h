#ifndef MANUALDESCRIPTION_H
#define MANUALDESCRIPTION_H

#include <QDialog>
#include <QFile>
#include <QDir>
#include <iostream>
#include <QFileDialog>
#include <QIcon>
#include <QPixmap>
#include <cxmlmethods.h>
#include <appinfo.h>
#include <QMessageBox>


namespace Ui {
class manualDescription;
}

class manualDescription : public QDialog
{
    Q_OBJECT

public:
    explicit manualDescription(QString repo, AppInfo &appAct, QWidget *parent = 0);
    ~manualDescription();

signals:
    void UpdateAppInfo(AppInfo);
private slots:
    void on_pushButton_clicked();

    void on_buttonBox_accepted();

    void on_eName_editingFinished();

    void on_eWeb_textChanged(const QString &arg1);

    void on_gbSource_clicked(bool checked);

    void on_cbCategory_activated(const QString &arg1);

    void on_toolButton_clicked();

    void on_cbVersion_activated(int index);

    void on_pushButtonMain_clicked();

    void on_pushButtonPatch_clicked();

private:

    Ui::manualDescription *ui;
    QString repository;
    AppInfo appAct;
    bool requiremets;

    void FormatTexts(QString &name, QString &summary, QString &desc);


};

#endif // MANUALDESCRIPTION_H
