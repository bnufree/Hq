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
class QSystemTrayIcon;
class QAndroidButton;

struct zchxCtrolObject{
    QWidget*            mDisplayWidget;
    QString             mTitle;
    QAndroidButton*     mBtn;
};

class zchxMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit zchxMainWindow(QWidget *parent = 0);
    ~zchxMainWindow();

private slots:
    void createCtrlObject(const QString& title, QWidget* widget);
    void slotSystemTrayOperation(QSystemTrayIcon::ActivationReason val);
    void slotSystemTrayMenuClicked();
    void slotRecvKuaiXunList(const KuaiXunList& list);
    void slotCtrlBtnClicked();
    void slotCtrlBtnClicked(QWidget* btn);
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
    QWidget                 *mCtrlWidget;
    QSystemTrayIcon         *mSysTrayIcon;
    QList<zchxCtrolObject>   mCtrlObjectList;
};

#endif // ZCHXMAINWINDOW_H
