#include "qforignvolcheckdlg.h"
#include "ui_qforignvolcheckdlg.h"
#include "date/shareworkingdatetime.h"
#include "qhkexchangevoldataprocess.h"
#include <QPushButton>

QForignVolCheckDlg::QForignVolCheckDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QForignVolCheckDlg)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    initTable();
}

QForignVolCheckDlg::~QForignVolCheckDlg()
{
    mPool.clear();
    delete ui;
}


void QForignVolCheckDlg::initTable()
{
#if 0
    //获取当前一年对应的工作日
    QList<QDate> dataList =  TradeDateMgr::getHisWorkingDay();
    ui->tableWidget->setRowCount(dataList.count());

    for(int i=0; i<dataList.size(); i++)
    {
        int col = 0;
        ui->tableWidget->setItem(i, col++, new QTableWidgetItem(dataList[i].toString("yyyy-MM-dd")));
        ui->tableWidget->item(i, 0)->setData(Qt::UserRole+1, dataList[i]);
        ui->tableWidget->setItem(i, col++, new QTableWidgetItem(QString::number(0)));
        ui->tableWidget->setItem(i, col++, new QTableWidgetItem(QString::number(0)));
        QPushButton *btn = new QPushButton(tr("重新获取"));
        connect(btn, SIGNAL(clicked(bool)), this, SLOT(slotFetchBtnClicked()));
        btn->setProperty("date", dataList[i]);
        ui->tableWidget->setCellWidget(i, col++, btn);
        QHKExchangeVolDataProcess * process = new QHKExchangeVolDataProcess(dataList[i], QHKExchangeVolDataProcess::Fetch_Only_File, 0);
        process->setAutoDelete(true);
        connect(process, SIGNAL(signalSendDataList(ShareForignVolFileDataList,QDate)), this, SLOT(slotFetchForeignData(ShareForignVolFileDataList,QDate)));
        mPool.start(process);
    }
#endif
}

void QForignVolCheckDlg::slotFetchBtnClicked()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if(!btn) return;
    QDate date = btn->property("date").toDate();
    QHKExchangeVolDataProcess * process = new QHKExchangeVolDataProcess(date, QHKExchangeVolDataProcess::Fetch_Only_Web, 0);
    process->setAutoDelete(true);
    connect(process, SIGNAL(signalSendDataList(ShareForignVolFileDataList,QDate)), this, SLOT(slotFetchForeignData(ShareForignVolFileDataList,QDate)));
    mPool.start(process);
}

void QForignVolCheckDlg::slotFetchForeignData(const ShareForignVolFileDataList& list, const QDate& date)
{
    //找到所在的行
    for(int i=0; i<ui->tableWidget->rowCount(); i++)
    {
        if(ui->tableWidget->item(i, 0)->data(Qt::UserRole + 1).toDate() == date)
        {
            int sh_num = 0, sz_num = 0;
            for(int k = 0; k<list.size(); k++)
            {
                if(list[k].mCode >= 500000) sh_num++;
                else sz_num++;
            }
            ui->tableWidget->item(i, 1)->setText(QString::number(sz_num));
            ui->tableWidget->item(i, 2)->setText(QString::number(sh_num));
        }
    }
}
