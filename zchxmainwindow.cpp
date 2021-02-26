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
#include <QTextBrowser>
#include <qetfscalethread.h>
#include "northbundwidget.h"
#include "qshareexchangedisplaywidget.h"
#include "table/HqMergeTableWidget.h"

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
    mInfoListWidget(0),
    mSysTrayIcon(0)
{
    ui->setupUi(this);
    mWidgetMgr = new QStackedWidget(this);
    ui->centralwidget->layout()->addWidget(mWidgetMgr);
    //添加TAB按钮
    mCtrlWidget = new QWidget(this);
    mCtrlWidget->setLayout(new QHBoxLayout);
    ui->centralwidget->layout()->addWidget(mCtrlWidget);
    ui->statusbar->setVisible(false);
    //快讯信息显示
    mInfoListWidget = new QKuaixunListWidget(this);
    HQTaskMagrCenter::instance()->registerInfoDisplaywidget(mInfoListWidget);
    createCtrlObject(QStringLiteral("资讯"), mInfoListWidget);
//    createCtrlObject(QStringLiteral("行情中心"), new zchxHqDisplayWidget(this));
    createCtrlObject(QStringLiteral("港资"), new NorthBundWidget(this));
//    createCtrlObject(QStringLiteral("龙虎榜"), new QWidget);
//    createCtrlObject(QStringLiteral("新股申购"), new QWidget);
//    createCtrlObject(QStringLiteral("交易记录"), new QShareExchangeDisplayWidget(this));






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
    mSysTrayIcon = new QSystemTrayIcon(this);
    mSysTrayIcon->setIcon(appIcon);
    mSysTrayIcon->setVisible(true);
    connect(mSysTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(slotSystemTrayOperation(QSystemTrayIcon::ActivationReason)));
#endif

//

    slotCtrlBtnClicked(mCtrlWidget->layout()->itemAt(0)->widget());


    QTimer::singleShot(1000, HQTaskMagrCenter::instance(), SLOT(slotStart()));

    int total_height = QApplication::desktop()->geometry().height();
    int total_mm = QApplication::desktop()->heightMM();
    qDebug()<<"total height:"<<total_height<<total_mm;

}

zchxMainWindow::~zchxMainWindow()
{
    delete ui;
}

void zchxMainWindow::resizeEvent(QResizeEvent *e)
{
//    if(mCtrlWidget) mCtrlWidget->setFixedWidth(e->size().width()-10);
    QMainWindow::resizeEvent(e);

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
    label->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::SubWindow);
    QRect rect = label->rect();
    rect.moveCenter(ui->centralwidget->rect().center());
    label->setGeometry(rect);
    QTimer::singleShot(5000, label, SLOT(deleteLater()));
    label->show();

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
//    if(!isHidden())return;
    if(list.size() == 0) return;
    qDebug()<<"recv list pop:"<<list.size();
    KuaixunData data = list.first();
    QTextBrowser *browser = new QTextBrowser;
    browser->setWindowFlags(Qt::SubWindow | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    browser->setWordWrapMode(QTextOption::WordWrap);
    browser->verticalScrollBar()->setVisible(false);
    browser->horizontalScrollBar()->setVisible(false);
    browser->setStyleSheet("background:black;color:white;font-family:Microsoft Yahei;font-size: 12pt; font-weight:bold;");

    browser->append(data.src_time + "  " + data.sourceString());
    browser->append(data.digest);
    QRect rect = QApplication::desktop()->availableGeometry();
    browser->setFixedWidth(rect.width() * 0.2);
    browser->adjustSize();
    browser->move(rect.width() - browser->width() - 20, rect.height() - browser->height() - 60);
    QTimer::singleShot(10*1000, this, [=](){browser->deleteLater();});
    browser->show();

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

void zchxMainWindow::createCtrlObject(const QString &title, QWidget *widget)
{
    zchxCtrolObject obj;
    obj.mDisplayWidget = widget;
    obj.mTitle = title;
    obj.mBtn = new QAndroidButton(title);
    obj.mBtn->setRelatedWidget(widget);
    connect(obj.mBtn, SIGNAL(clicked()), this, SLOT(slotCtrlBtnClicked()));
    mCtrlWidget->layout()->addWidget(obj.mBtn);
    mCtrlObjectList.append(obj);
    mWidgetMgr->addWidget(widget);
}

