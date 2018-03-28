#include "qsahrerealdisplaywidget.h"
#include "ui_qsahrerealdisplaywidget.h"

QSahreRealDisplayWidget::QSahreRealDisplayWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QSahreRealDisplayWidget)
{
    ui->setupUi(this);
}

QSahreRealDisplayWidget::~QSahreRealDisplayWidget()
{
    delete ui;
}
