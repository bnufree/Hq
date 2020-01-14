#ifndef ZCHXMAINWINDOW_H
#define ZCHXMAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include "data_structure/hqutils.h"

namespace Ui {
class zchxMainWindow;
}

class QShareTablewidget;
class QShareGraphicWidget;
class QBlockTableWidget;

class zchxMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit zchxMainWindow(QWidget *parent = 0);
    ~zchxMainWindow();

private slots:
    void slotSystemTrayOperation(QSystemTrayIcon::ActivationReason val);
    void slotSystemTrayMenuClicked();
    void slotRecvKuaiXunList(const KuaiXunList& list);
protected:
    void resizeEvent(QResizeEvent* e);
    void closeEvent(QCloseEvent * e);

private:
    Ui::zchxMainWindow *ui;
    QShareTablewidget       *mHqList;
    QShareGraphicWidget     *mShareGraphicWidget;
    QBlockTableWidget       *mBlockTableWidget;
};

#endif // ZCHXMAINWINDOW_H
