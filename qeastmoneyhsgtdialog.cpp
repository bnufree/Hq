#include "qeastmoneyhsgtdialog.h"
#include "ui_qeastmoneyhsgtdialog.h"

QEastMoneyHSGTDialog::QEastMoneyHSGTDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QEastMoneyHSGTDialog)
{
    ui->setupUi(this);
    ui->DateEdit->setEnabled(false);
    ui->NameEdit->setEnabled(false);
    ui->MarketTypeCBX->setEnabled(false);
    this->setWindowFlags(this->windowFlags() | Qt::WindowMinimizeButtonHint);
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->DateEdit->setDate(QDate::currentDate());
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(DATA_SERVICE, SIGNAL(signalSendTop10ChinaStockInfos(QList<ChinaShareExchange>)), this, SLOT(slotRecvTop10Infos(QList<ChinaShareExchange>)));
}

QEastMoneyHSGTDialog::~QEastMoneyHSGTDialog()
{
    delete ui;
}

void QEastMoneyHSGTDialog::on_SearchBTB_clicked()
{
    int market = 0;
    if(ui->MarketCHK->isChecked()) market = ui->MarketTypeCBX->currentIndex();
    QDate date;
    if(ui->DateCHK->isChecked()) date = ui->DateEdit->date();
    QString code;
    if(ui->CodeCHK->isChecked()) code = ui->NameEdit->text();
    emit DATA_SERVICE->signalQueryTop10ChinaStockInfos(date, code, market);
}

void QEastMoneyHSGTDialog::slotRecvTop10Infos(const QList<ChinaShareExchange> &list)
{
    ui->tableWidget->setRowCount(list.count());
    ui->tableWidget->sortByColumn(4);
    int row = 0;
    foreach (ChinaShareExchange info, list) {
        int col = 0;
        ui->tableWidget->setItem(row, col++, new QTableWidgetItem(info.mDate.toString("yyyy-MM-dd")));
        ui->tableWidget->setItem(row, col++, new QTableWidgetItem(info.code));
        ui->tableWidget->setItem(row, col++, new QTableWidgetItem(info.name));
        ui->tableWidget->setItem(row, col++, new QTableWidgetItem(QString::number(info.cur)));
        ui->tableWidget->setItem(row, col++, new QTableWidgetItem(QString("").sprintf("%.2f%", info.per)));
        col++;
        col++;
        ui->tableWidget->setItem(row, col++, new QTableWidgetItem(QString::number(info.mTop10Buy/10000)));
        ui->tableWidget->setItem(row, col++, new QTableWidgetItem(QString::number(info.mTop10Sell/10000)));
        ui->tableWidget->setItem(row, col++, new QTableWidgetItem(QString::number((info.mTop10Buy-info.mTop10Sell) /10000)));
        ui->tableWidget->item(row, 0)->setData(Qt::UserRole+1, info.code);
        row++;
    }

}

void QEastMoneyHSGTDialog::on_MarketCHK_clicked(bool checked)
{
    ui->MarketTypeCBX->setEnabled(checked);
}

void QEastMoneyHSGTDialog::on_DateCHK_clicked(bool checked)
{
    ui->DateEdit->setEnabled(checked);
}

void QEastMoneyHSGTDialog::on_CodeCHK_clicked(bool checked)
{
    ui->NameEdit->setEnabled(checked);
}

void QEastMoneyHSGTDialog::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    if(row >= 0)
    {
        QString code = ui->tableWidget->item(row, 0)->data(Qt::UserRole+1).toString();
        emit DATA_SERVICE->signalQueryTop10ChinaStockInfos(QDate(), code, 0);
    }

}
