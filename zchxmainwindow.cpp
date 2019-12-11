#include "zchxmainwindow.h"
#include "ui_zchxmainwindow.h"
#include "real/qnorthflowinfodisplaywidget.h"
#include "real/qhqsysteminfowidget.h"
#include "qindexwidget.h"
#include <QResizeEvent>
#include <QDesktopWidget>
#include <QApplication>
#include "table/qsharetablewidget.h"

zchxMainWindow::zchxMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::zchxMainWindow)
{
    ui->setupUi(this);
//    this->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    //添加实时数据到左侧
    ui->left_frame->layout()->addWidget(new QShareTablewidget(this));
    //添加实时北向曲线
    ui->right_frame->layout()->addWidget(new QNorthFlowInfoDisplayWidget);
    //状态栏添加实时指数
    statusBar()->addPermanentWidget(new QIndexWidget(this), QApplication::desktop()->availableGeometry().width() * 0.66);
    //状态栏添加系统时间
    statusBar()->addPermanentWidget(new QHqSystemInfoWidget(0, this));
}

zchxMainWindow::~zchxMainWindow()
{
    delete ui;
}

void zchxMainWindow::resizeEvent(QResizeEvent *e)
{
    QMainWindow::resizeEvent(e);
    ui->right_frame->setFixedWidth(e->size().width()* 0.25);
}
