#include "zchxmainwindow.h"
#include "ui_zchxmainwindow.h"
#include "real/qnorthflowinfodisplaywidget.h"
#include "real/qhqsysteminfowidget.h"
#include "qindexwidget.h"
#include <QResizeEvent>
#include <QDesktopWidget>
#include <QApplication>
#include "table/qsharetablewidget.h"
#include "table/qblocktablewidget.h"
#include "hqtaskmagrcenter.h"
#include <QSystemTrayIcon>
#include "history/qsharegraphicwidget.h"
#include <QSplitter>
#include <QToolButton>
#include "qandroidbutton.h"
#include "real/hqkuaixun.h"
#include "qkuaixunlistwidget.h"
#include <QScroller>
#include <QScrollBar>
#include "table/zchxhqdisplaywidget.h"
#include <QDesktopWidget>
#include "qpopwindow.h"
#include <QKeyEvent>

enum zchxBtnIndex{
    Btn_Info = 0,
    Btn_Hq,
    Btn_HK,
    Btn_Lhb,
    Btn_New,
    Btn_End,
};


zchxMainWindow::zchxMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::zchxMainWindow),
    mInfoListWidget(0)
{
    ui->setupUi(this);
    mWidgetMgr = new QStackedWidget(this);
    //快讯信息显示
    mInfoListWidget = new QKuaixunListWidget(this);
#ifdef Q_OS_WIN
    int width = QApplication::desktop()->availableGeometry().width();
    mInfoListWidget->setFixedWidth(width * 0.25);
    ui->centralwidget->layout()->addWidget(mInfoListWidget);
#else
    mWidgetMgr->addWidget(mInfoListWidget);
#endif
    ui->centralwidget->layout()->addWidget(mWidgetMgr);
    //启动系统巡演
//    HQTaskMagrCenter::instance()->start();



#if 0
    //行情信息显示
//    mHqList = new QShareTablewidget(this);
    mWidgetMgr->addWidget(new zchxHqDisplayWidget(this));
    //
//    mWidgetMgr->addWidget(new QNorthFlowInfoDisplayWidget(this));
    //测试QtableWidget
    {
        QTableWidget* w = new QTableWidget(this);
        w->setRowCount(200);
        w->setColumnCount(1);
        w->horizontalHeader()->setVisible(false);
        w->verticalHeader()->setVisible(false);
        w->horizontalScrollBar()->setVisible(false);
        w->verticalScrollBar()->setVisible(false);
        w->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        w->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        QScroller::grabGesture(w);
        for(int i=0;i <w->rowCount(); i++)
        {
            w->setItem(i, 0, new QTableWidgetItem(QString::number(i+1)));
        }
        mWidgetMgr->addWidget(w);
    }

    //添加TAB按钮
    mCtrlWidget = new QWidget(this);
    mCtrlWidget->setLayout(new QHBoxLayout);
    this->statusBar()->addWidget(mCtrlWidget);
    //按钮名称
    QStringList btnTitles;
    btnTitles.append(QStringLiteral("资讯"));
    btnTitles.append(QStringLiteral("行情"));
    btnTitles.append(QStringLiteral("港资"));
    btnTitles.append(QStringLiteral("龙虎榜"));
    btnTitles.append(QStringLiteral("新股申购"));
    //按钮对应的widget

    for(int i=0; i<Btn_End; i++)
    {
        QAndroidButton* btn = new QAndroidButton(btnTitles[i]);
        if(i < mWidgetMgr->count())
        {
            btn->setRelatedWidget(mWidgetMgr->widget(i));
        } else
        {
            btn->setRelatedWidget(0);
        }
        connect(btn, SIGNAL(clicked()), this, SLOT(slotCtrlBtnClicked()));
        mCtrlWidget->layout()->addWidget(btn);
    }

#endif

#if 0
//    this->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    ui->centralwidget->layout()->addWidget(mBlockTableWidget = new QBlockTableWidget(ui->centralwidget));
    ui->centralwidget->layout()->addWidget(mHqList = new QShareTablewidget(ui->centralwidget));
    QFrame *right = new QFrame(ui->centralwidget);
    right->setLayout(new QVBoxLayout);
    ui->centralwidget->layout()->addWidget(right);
    QHBoxLayout* hlayout = qobject_cast<QHBoxLayout*>(ui->centralwidget->layout());
    if(hlayout)
    {
        hlayout->setStretch(0, 1);
        hlayout->setStretch(1, 8);
        hlayout->setStretch(2, 3);
    }

    //添加实时北向曲线
    right->layout()->addWidget(new QNorthFlowInfoDisplayWidget);
    right->layout()->addWidget(mShareGraphicWidget = new QShareGraphicWidget(ui->centralwidget));
    //状态栏添加实时指数
    statusBar()->addWidget(new QIndexWidget(this), QApplication::desktop()->availableGeometry().width() * 0.66);
    //状态栏添加系统时间
//    statusBar()->addPermanentWidget(new QHqSystemInfoWidget(this));



    connect(mHqList, SIGNAL(signalSetDisplayHSHK(QString)), mShareGraphicWidget, SLOT(setCode(QString)));
    connect(mHqList, SIGNAL(signalDisplayDetailOfCode(QString)), mShareGraphicWidget, SLOT(setCode(QString)));
    connect(mBlockTableWidget, SIGNAL(signalDisplayBlockDetailCodesList(QStringList)),
            mHqList, SLOT(setSelfShareCodesList(QStringList)));

#endif
#ifdef Q_OS_WIN
    //添加图标
    QIcon appIcon = QIcon(":/icon/image/Baidu_96px.png");
    this->setWindowIcon(appIcon);
    QSystemTrayIcon* systemIcon = new QSystemTrayIcon(this);
    systemIcon->setIcon(appIcon);
    systemIcon->setVisible(true);
    connect(systemIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(slotSystemTrayOperation(QSystemTrayIcon::ActivationReason)));
#endif

//    slotCtrlBtnClicked(mCtrlWidget->layout()->itemAt(0)->widget());
    HqKuaixun *infoThread = new HqKuaixun(this);
    connect(infoThread, SIGNAL(signalSendKuaiXun(KuaiXunList)), this, SLOT(slotRecvKuaiXunList(KuaiXunList)));
    connect(infoThread, SIGNAL(signalSendKuaiXun(KuaiXunList)), mInfoListWidget, SLOT(appendData(KuaiXunList)));
    infoThread->start();

}

zchxMainWindow::~zchxMainWindow()
{
    delete ui;
}

void zchxMainWindow::resizeEvent(QResizeEvent *e)
{
    QMainWindow::resizeEvent(e);
//    if(mCtrlWidget) mCtrlWidget->setMinimumWidth(e->size().width()-10);
}

void zchxMainWindow::closeEvent(QCloseEvent *e)
{
#ifdef Q_OS_WIN
    qDebug()<<"close now !!!!!!!!";
    hide();
    e->ignore();
#else
    static quint64 last_close_time = 0;

    qint64 current = QDateTime::currentMSecsSinceEpoch();
    if(current - last_close_time <= 5000)
    {
        e->accept();
        return;
    }
    last_close_time = current;
//    statusBar()->showMessage(QStringLiteral("再按一次退出"), 5000);
    QLabel *label = new QLabel(QStringLiteral("再按一次退出"), ui->centralwidget);
    label->setWindowFlags(Qt::WindowStaysOnTopHint);
    QRect rect = label->rect();
    rect.moveCenter(ui->centralwidget->rect().center());
    label->setGeometry(rect);
    QTimer::singleShot(5000, label, SLOT(deleteLater()));

    e->ignore();
    return;
#endif
}

void zchxMainWindow::slotSystemTrayOperation(QSystemTrayIcon::ActivationReason val)
{
    switch (val) {
    case QSystemTrayIcon::DoubleClick:
    {
        setVisible(!isVisible());
    }
        break;
    case QSystemTrayIcon::Context:
    {
        QMenu *popMenu = new QMenu(this);
        QList<QAction*> actlist;
        QStringList poplist;
        poplist<<QStringLiteral("显示")<<QStringLiteral("退出");
        int index = -1;
        foreach (QString name, poplist) {
            index++;
            QAction *act = new QAction(this);
            act->setText(name);
            act->setData(index);
            connect(act, &QAction::triggered, this, &zchxMainWindow::slotSystemTrayMenuClicked);
            actlist.append(act);
        }

        popMenu->addActions(actlist);
        popMenu->popup(QCursor::pos());
    }
        break;
    default:
        break;
    }

}

void zchxMainWindow::slotSystemTrayMenuClicked()
{
    QAction *src = (QAction*) sender();
    if(!src) return;
    int val = src->data().toInt();
    if(val == 0)
    {
        //显示
        setVisible(!isVisible());
    } else
    {
        exit(0);
    }

}

void zchxMainWindow::slotRecvKuaiXunList(const KuaiXunList& list)
{
#ifdef Q_OS_WIN
    if(!isHidden())return;
    if(list.size() == 0) return;
    qDebug()<<"recv list pop:"<<list.size();
    QKuaixunListWidget *widget = new QKuaixunListWidget;
    widget->appendData(list);
    widget->setFixedWidth(80);
    QRect rect = QApplication::desktop()->screenGeometry();
    widget->move(rect.width() - 100, rect.height() - 40);
    widget->show();

    QTimer::singleShot(5000, this, [=](){widget->hide(); widget->deleteLater();});
#endif

}

void zchxMainWindow::slotCtrlBtnClicked()
{
    QAndroidButton* btn = qobject_cast<QAndroidButton*>(sender());
    if(!btn) return;
    slotCtrlBtnClicked(btn);
}

void zchxMainWindow::slotCtrlBtnClicked(QWidget* w)
{
    QAndroidButton* btn = qobject_cast<QAndroidButton*>(w);
    if(!btn) return;
    mWidgetMgr->setCurrentWidget(btn->relatedWidget());
    for(int i=0; i<mCtrlWidget->layout()->count(); i++)
    {
        QWidget* temp = mCtrlWidget->layout()->itemAt(i)->widget();
        QAndroidButton* current = qobject_cast<QAndroidButton*>(temp);
        if(btn == current)
        {
            current->setSelected(true);
        } else
        {
            current->setSelected(false);

        }

    }
}

