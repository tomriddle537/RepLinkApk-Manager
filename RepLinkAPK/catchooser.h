#ifndef CATCHOOSER_H
#define CATCHOOSER_H

#include <QDialog>
#include <QString>
#include <QFile>
#include <iostream>
#include <QMessageBox>
#include <QListWidgetItem>


namespace Ui {
class CatChooser;
}

class CatChooser : public QDialog
{
    Q_OBJECT

public:
    explicit CatChooser(QString apkname, QString repository, QWidget *parent = 0);
    QString SelectedCategory();
    ~CatChooser();

signals:
    void Cat(QString);
    void CatForAPKs(QString);


private slots:
    void on_buttonBox_rejected();

    void on_listWidget_itemActivated(QListWidgetItem *item);

private:
    Ui::CatChooser *ui;
    QString category;
};

#endif // CATCHOOSER_H
