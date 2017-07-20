#include "qeastmoneyhsgtdialog.h"
#include "ui_qeastmoneyhsgtdialog.h"

QEastMoneyHSGTDialog::QEastMoneyHSGTDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QEastMoneyHSGTDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() | Qt::WindowMinimizeButtonHint);
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->DateEdit->setDate(QDate::currentDate());
    connect(DATA_SERVICE, SIGNAL(signalSendTop10ChinaStockInfos(QList<ChinaShareExchange>)), this, SLOT(slotRecvTop10Infos(QList<ChinaShareExchange>)));
}

QEastMoneyHSGTDialog::~QEastMoneyHSGTDialog()
{
    delete ui;
}

void QEastMoneyHSGTDialog::on_SearchBTB_clicked()
{
    emit DATA_SERVICE->signalQueryTop10ChinaStockInfos(ui->DateEdit->date());
}

void QEastMoneyHSGTDialog::slotRecvTop10Infos(const QList<ChinaShareExchange> &list)
{
    ui->tableWidget->setRowCount(list.count());
    int row = 0;
    foreach (ChinaShareExchange info, list) {
        int col = 0;
        ui->tableWidget->setItem(row, col++, new QTableWidgetItem(info.mDate.toString("yyyy-MM-dd")));
        ui->tableWidget->setItem(row, col++, new QTableWidgetItem(info.code));
        ui->tableWidget->setItem(row, col++, new QTableWidgetItem(info.name));
        ui->tableWidget->setItem(row, col++, new QTableWidgetItem(QString::number(info.cur)));
        ui->tableWidget->setItem(row, col++, new QTableWidgetItem(QString("%1%").arg(info.per)));
        col++;
        col++;
        ui->tableWidget->setItem(row, col++, new QTableWidgetItem(QString::number(info.mTop10Buy/10000)));
        ui->tableWidget->setItem(row, col++, new QTableWidgetItem(QString::number(info.mTop10Sell/10000)));
        ui->tableWidget->setItem(row, col++, new QTableWidgetItem(QString::number((info.mTop10Buy-info.mTop10Sell) /10000)));
        row++;
    }

}
