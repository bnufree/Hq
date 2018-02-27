#include "qexchangedatemangagedialog.h"
#include "ui_qexchangedatemangagedialog.h"
#include <QDebug>
#include "utils/profiles.h"

QExchangeDateMangageDialog::QExchangeDateMangageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QExchangeDateMangageDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() ^ Qt::FramelessWindowHint);
    //初始化已经设定的日期
    mMgrDates = Profiles::instance()->value("DateManage", "Expired").toStringList();
    mMgrDates.sort();
    //foreach (QString date, dates) {
        ui->listWidget->addItems(mMgrDates);
    //}
       // mMgrDates.sort();
}

QExchangeDateMangageDialog::~QExchangeDateMangageDialog()
{
    qDebug()<<__FUNCTION__<<"  "<<__LINE__;
    delete ui;
}

void QExchangeDateMangageDialog::on_DelBtn_clicked()
{
    if(ui->listWidget->currentItem() && ui->listWidget->currentItem()->isSelected())
    {
        mMgrDates.removeAll(ui->listWidget->currentItem()->text());
        ui->listWidget->takeItem(ui->listWidget->currentRow());
    }

}

void QExchangeDateMangageDialog::on_AddBtn_clicked()
{
    QString seldate = ui->calendarWidget->selectedDate().toString("yyyy-MM-dd");
    if(!mMgrDates.contains(seldate))
    {
        ui->listWidget->addItem(seldate);
        mMgrDates.append(seldate);
    }

}

void QExchangeDateMangageDialog::on_calendarWidget_currentPageChanged(int year, int month)
{

}

void QExchangeDateMangageDialog::on_CloseBtn_clicked()
{
    qDebug()<<"expired:"<<mMgrDates;
    Profiles::instance()->setValue("DateManage", "Expired", QVariant(mMgrDates));
    emit accepted();
}
