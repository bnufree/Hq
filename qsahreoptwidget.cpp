#include "qsahreoptwidget.h"
#include "ui_qsahreoptwidget.h"
#include "data_structure/hqutils.h"
#include "dbservices/dbservices.h"
#include <QTimer>

QSahreOptWidget::QSahreOptWidget(const QString& code, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QSahreOptWidget),
    mCode(code)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);
    this->setAttribute(Qt::WA_DeleteOnClose);
    int height = HqUtils::convertMM2Pixel(10);
    this->setFixedSize(4*height+3, height);
    QTimer::singleShot(3*1000, this, SLOT(close()));
}


QSahreOptWidget::~QSahreOptWidget()
{
    delete ui;
}

void QSahreOptWidget::on_add_clicked()
{
    DATA_SERVICE->signalSetFavCode(mCode);
}

void QSahreOptWidget::on_del_clicked()
{
    DATA_SERVICE->signalSetFavCode(mCode);
}

void QSahreOptWidget::on_up_clicked()
{
    DATA_SERVICE->signalSetFavCode(mCode);
}

void QSahreOptWidget::on_down_clicked()
{
    DATA_SERVICE->signalSetFavCode(mCode);
}
