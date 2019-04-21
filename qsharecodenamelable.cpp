#include "qsharecodenamelable.h"
#include "ui_qsharecodenamelable.h"

QShareCodeNameLable::QShareCodeNameLable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QShareCodeNameLable)
{
    ui->setupUi(this);
}

QShareCodeNameLable::~QShareCodeNameLable()
{
    delete ui;
}
