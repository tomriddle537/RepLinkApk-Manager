#ifndef CATEDITOR_H
#define CATEDITOR_H

#include <QDialog>
#include <QFile>
#include <QDir>
#include <iostream>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QTextStream>
#include <QMessageBox>
#include <QMenu>
#include <QPoint>



namespace Ui {
class CatEditor;
}

class CatEditor : public QDialog
{
    Q_OBJECT

public:
    explicit CatEditor(QString repo, QWidget *parent = 0);
    ~CatEditor();

private slots:
    void on_buttonBox_accepted();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_tbAdd_clicked();

    void PopUp(const QPoint& pos);

    void on_actionRemove_triggered();

private:
    Ui::CatEditor *ui;
    QString repository;

    void OpenCatEditor();
    void ReCount();
};

#endif // CATEDITOR_H
