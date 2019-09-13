#include "qshareforeignvoltablewidget.h"
#include "data_structure/shareworkingdatetime.h"
#include <QPushButton>
#include "history/qhkexchangevoldataprocess.h"

QShareForeignVolTableWidget::QShareForeignVolTableWidget(QWidget *parent) : HqTableWidget(parent)
{
    //设定抬头
    TableColDataList datalist;
    datalist.append(TableColData(QStringLiteral("日期"), STK_DISPLAY_SORT_TYPE_NONE));
    datalist.append(TableColData(QStringLiteral("深圳"), STK_DISPLAY_SORT_TYPE_CODE));
    datalist.append(TableColData(QStringLiteral("上海"), STK_DISPLAY_SORT_TYPE_NAME));
    datalist.append(TableColData(QStringLiteral("操作"), STK_DISPLAY_SORT_TYPE_CHGPER));
    setHeaders(datalist);
}

void QShareForeignVolTableWidget::slotStartInit()
{
    //初始化
    //获取当前一年对应的工作日
    QList<QDate> dataList =  ShareWorkingDate::getHisWorkingDay();
    prepareUpdateTable(dataList.count());
    for(int i=0; i<dataList.size(); i++)
    {
        qDebug()<<"date:"<<dataList[i].toString("yyyy-MM-dd");
        int col = 0;
        setItemText(i, col++, dataList[i].toString("yyyy-MM-dd"));
        item(i, 0)->setData(Qt::UserRole+1, dataList[i]);
        setItemText(i, col++, QString::number(0));
        setItemText(i, col++, QString::number(0));
        QPushButton *btn = new QPushButton(tr("重新获取"));
        connect(btn, SIGNAL(clicked(bool)), this, SLOT(slotFetchBtnClicked()));
        btn->setProperty("date", dataList[i]);
        btn->setStyleSheet("color:white");
        this->setCellWidget(i, col++, btn);
        QHKExchangeVolDataProcess * process = new QHKExchangeVolDataProcess(dataList[i], QHKExchangeVolDataProcess::Fetch_Only_File, 0);
        process->setAutoDelete(true);
        connect(process, SIGNAL(signalSendDataList(ShareForignVolFileDataList,QDate)), this, SLOT(slotFetchForeignData(ShareForignVolFileDataList,QDate)));
        mPool.start(process);
    }
}

void QShareForeignVolTableWidget::slotFetchBtnClicked()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if(!btn) return;
    QDate date = btn->property("date").toDate();
    QHKExchangeVolDataProcess * process = new QHKExchangeVolDataProcess(date, QHKExchangeVolDataProcess::Fetch_All, 0);
    process->setAutoDelete(true);
    connect(process, SIGNAL(signalSendDataList(ShareForignVolFileDataList,QDate)), this, SLOT(slotFetchForeignData(ShareForignVolFileDataList,QDate)));
    mPool.start(process);
}

void QShareForeignVolTableWidget::slotFetchForeignData(const ShareForignVolFileDataList& list, const QDate& date)
{
    //找到所在的行
    for(int i=0; i<this->rowCount(); i++)
    {
        if(this->item(i, 0)->data(Qt::UserRole + 1).toDate() == date)
        {
            int sh_num = 0, sz_num = 0;
            for(int k = 0; k<list.size(); k++)
            {
                if(list[k].mCode >= 500000) sh_num++;
                else sz_num++;
            }
            this->item(i, 1)->setText(QString::number(sz_num));
            this->item(i, 2)->setText(QString::number(sh_num));
        }
    }
}
