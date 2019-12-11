#include "qhqsysteminfowidget.h"
#include "ui_qhqsysteminfowidget.h"
#include "dbservices/dbservices.h"
#include "basic_info/qshareactivedateupdatethread.h"
#include <QDateTime>

QHqSystemInfoWidget::QHqSystemInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QHqSystemInfoWidget)
{
    ui->setupUi(this);
}

QHqSystemInfoWidget::~QHqSystemInfoWidget()
{
    delete ui;
}

void QHqSystemInfoWidget::slotUpdateSystemStatus(qint64 time, int status)
{
    ui->time->setText(QDateTime::fromMSecsSinceEpoch(time).toString("yyyy-MM-dd hh:mm:ss"));
    switch (status) {
    case HQ_NotOpen:
        ui->status->setText(QStringLiteral("未开盘"));
        break;
    case HQ_Closed:
        ui->status->setText(QStringLiteral("交易结束"));
        break;
    case HQ_InCharge:
        ui->status->setText(QStringLiteral("交易中"));
        break;
    default:
        break;
    }
    if(status == HQ_InCharge)
    {
        setStyleSheet("QLabel{color:green; font-weight:bold;}");
    } else
    {
        setStyleSheet("QLabel{color:red;}");
    }

//    qDebug()<<ui->time->text()<<ui->status->text();
}
