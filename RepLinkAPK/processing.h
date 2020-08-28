#ifndef PROCESSING_H
#define PROCESSING_H

#include <QDialog>
#include <QIcon>
#include <QKeyEvent>
#include <QWinTaskbarProgress>
#include <QWinTaskbarButton>
#include <QMovie>

namespace Ui {
class processing;
}

class processing : public QDialog
{
    Q_OBJECT

public:
    explicit processing(QWidget *parent = 0);
    ~processing();

private slots:
   void slot_UpdateInfo(QPixmap pixmap);
   void slot_UpdatePorc(int porc);

private:
    Ui::processing *ui;
    QIcon ico;
    void keyPressEvent(QKeyEvent *e);
    void CreateTaskbar();
};

#endif // PROCESSING_H
