#ifndef CABOUT_H
#define CABOUT_H

#include <QDialog>
#include <QPixmap>
#include <QIcon>

namespace Ui {
class CAbout;
}

class CAbout : public QDialog
{
    Q_OBJECT

public:
    explicit CAbout(QWidget *parent = 0);
    ~CAbout();

private:
    Ui::CAbout *ui;
};

#endif // CABOUT_H
