#ifndef ZCHXMAINWINDOW_H
#define ZCHXMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class zchxMainWindow;
}

class zchxMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit zchxMainWindow(QWidget *parent = 0);
    ~zchxMainWindow();

private:
    Ui::zchxMainWindow *ui;
};

#endif // ZCHXMAINWINDOW_H
