#ifndef CCONFIG_H
#define CCONFIG_H

#include <QDialog>
#include <QSettings>
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <windows.h>

namespace Ui {
class CConfig;
}

class CConfig : public QDialog
{
    Q_OBJECT

public:
    explicit CConfig(QWidget *parent = 0);
    ~CConfig();

private slots:
    void on_buttonBox_accepted();

    void on_tbJava_clicked();

    void on_tbKeyStore_clicked();

    void on_pushButton_pressed();

    void on_pushButton_released();


private:
    Ui::CConfig *ui;
    QSettings *settings;
    QString shortPathName(const QString &file);
};

#endif // CCONFIG_H
