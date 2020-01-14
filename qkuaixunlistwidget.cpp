#include "qkuaixunlistwidget.h"
#include "ui_qkuaixunlistwidget.h"

QKuaixunListWidget::QKuaixunListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QKuaixunListWidget)
{
    ui->setupUi(this);
}

QKuaixunListWidget::~QKuaixunListWidget()
{
    delete ui;
}
