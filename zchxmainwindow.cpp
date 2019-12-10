#include "zchxmainwindow.h"
#include "ui_zchxmainwindow.h"

zchxMainWindow::zchxMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::zchxMainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
}

zchxMainWindow::~zchxMainWindow()
{
    delete ui;
}
