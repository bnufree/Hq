#include "qnorthbounddisplayweidget.h"
#include "ui_qnorthbounddisplayweidget.h"
#include "real/qnorthflowinfodisplaywidget.h"

QNorthBoundDisPlayWeidget::QNorthBoundDisPlayWeidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QNorthBoundDisPlayWeidget)
{
    ui->setupUi(this);
}

QNorthBoundDisPlayWeidget::~QNorthBoundDisPlayWeidget()
{
    delete ui;
}
