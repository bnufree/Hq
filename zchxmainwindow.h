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

protected:
    void resizeEvent(QResizeEvent* e);

private:
    Ui::zchxMainWindow *ui;
};

#endif // ZCHXMAINWINDOW_H
