#include "qeastmoneyhsgtdialog.h"
#include "ui_qeastmoneyhsgtdialog.h"
#include "utils/sharedata.h"


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
    connect(DATA_SERVICE, SIGNAL(signalSendTop10ChinaShareInfos(QList<ChinaShareExchange>)), this, SLOT(slotRecvTop10Infos(QList<ChinaShareExchange>)));
    connect(DATA_SERVICE, SIGNAL(signalSendShareForeignVol(ShareDataList)), this, SLOT(slotRecvListInfo(ShareDataList)));
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
    }

}

void QEastMoneyHSGTDialog::slotDisplayCode(const QString &code)
{
    emit DATA_SERVICE->signalQueryShareForeignVol(code);
}

void QEastMoneyHSGTDialog::slotRecvListInfo(const ShareDataList& list)
{
    ui->tableWidget->setRowCount(list.count());
    int row = 0;
    foreach (ShareData info, list) {
        int col = 0;
        ui->tableWidget->setItem(row, col++, new QTableWidgetItem(QDateTime::fromMSecsSinceEpoch(info.mTime).toString("yyyy-MM-dd")));
        ui->tableWidget->setItem(row, col++, new QTableWidgetItem(info.mCode));
        ui->tableWidget->setItem(row, col++, new QTableWidgetItem(info.mName));
        ui->tableWidget->setItem(row, col++, new QTableWidgetItem(QString::number(info.mClose, 'f', 2)));
        ui->tableWidget->setItem(row, col++, new QTableWidgetItem(QString("").sprintf("%.2f%", info.mChgPercent)));
        ui->tableWidget->setItem(row, col++, new QTableWidgetItem(QString::number(info.mForeignVol/10000.0, 'f', 1)));
        ui->tableWidget->setItem(row, col++, new QTableWidgetItem(QString::number(info.mForeignVolChg/10000.0, 'f', 1)));
        ui->tableWidget->setItem(row, col++, new QTableWidgetItem(QString::number(info.mForeignCap/100000000.0, 'f', 2)));
        ui->tableWidget->item(row, 0)->setData(Qt::UserRole+1, info.mCode);
        row++;
    }

}
