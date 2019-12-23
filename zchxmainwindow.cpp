#include "zchxmainwindow.h"
#include "ui_zchxmainwindow.h"
#include "real/qnorthflowinfodisplaywidget.h"
#include "real/qhqsysteminfowidget.h"
#include "qindexwidget.h"
#include <QResizeEvent>
#include <QDesktopWidget>
#include <QApplication>
#include "table/qsharetablewidget.h"
#include "hqtaskmagrcenter.h"
#include <QSystemTrayIcon>
#include "history/qsharegraphicwidget.h"
#include <QSplitter>

zchxMainWindow::zchxMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::zchxMainWindow)
{
    ui->setupUi(this);
//    this->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    QSplitter *main = new QSplitter(Qt::Horizontal, this);
    ui->centralwidget->layout()->addWidget(main);
    //添加实时数据到左侧
    main->addWidget(mHqList = new QShareTablewidget(ui->centralwidget));

    QFrame *right = new QFrame(main);
    right->setLayout(new QVBoxLayout);
    main->setStretchFactor(0, 1);
    main->setStretchFactor(1, 1);

    //添加实时北向曲线
    right->layout()->addWidget(new QNorthFlowInfoDisplayWidget);
    right->layout()->addWidget(mShareGraphicWidget = new QShareGraphicWidget(ui->centralwidget));
    //状态栏添加实时指数
    statusBar()->addPermanentWidget(new QIndexWidget(this), QApplication::desktop()->availableGeometry().width() * 0.66);
    //状态栏添加系统时间
//    statusBar()->addPermanentWidget(new QHqSystemInfoWidget(this));

    //添加图标
    QIcon appIcon = QIcon(":/icon/image/Baidu_96px.png");
    this->setWindowIcon(appIcon);
    QSystemTrayIcon* systemIcon = new QSystemTrayIcon(this);
    systemIcon->setIcon(appIcon);
    systemIcon->setVisible(true);
    connect(systemIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(slotSystemTrayOperation(QSystemTrayIcon::ActivationReason)));

    //启动系统巡演
    HQTaskMagrCenter::instance()->start();
    connect(mHqList, SIGNAL(signalSetDisplayHSHK(QString)), mShareGraphicWidget, SLOT(setCode(QString)));
    connect(mHqList, SIGNAL(signalDisplayDetailOfCode(QString)), mShareGraphicWidget, SLOT(setCode(QString)));
}

zchxMainWindow::~zchxMainWindow()
{
    delete ui;
}

void zchxMainWindow::resizeEvent(QResizeEvent *e)
{
    QMainWindow::resizeEvent(e);
}

void zchxMainWindow::closeEvent(QCloseEvent *e)
{
    qDebug()<<"close now !!!!!!!!";
    hide();
    e->ignore();
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

