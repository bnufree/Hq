#ifndef ZCHXMAINWINDOW_H
#define ZCHXMAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include "data_structure/hqutils.h"
#include <QStackedWidget>

namespace Ui {
class zchxMainWindow;
}

class QShareTablewidget;
class QShareGraphicWidget;
class QBlockTableWidget;
class QKuaixunListWidget;

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
    QKuaixunListWidget      *mInfoListWidget;
    QStackedWidget          *mWidgetMgr;
};

#endif // ZCHXMAINWINDOW_H
