#include "qshareexchangetable.h"
#include <QDebug>
#include "data_structure/hqutils.h"
#include "data_structure/sharedata.h"
#include "dbservices/dbservices.h"
#include "qsahreoptwidget.h"
#include "real/qsinastkresultmergethread.h"
#include "hqtaskmagrcenter.h"


QString ShareExchangeType2String(int type)
{
    if(type == ShareExchange_None) return QStringLiteral("未知");
    if(type == ShareExchange_Buy) return QStringLiteral("买入");
    if(type == ShareExchange_Sell) return QStringLiteral("卖出");
    if(type == ShareExchange_Share_Bonus) return QStringLiteral("除权");
    if(type == ShareExchange_Dividend_Bonus) return QStringLiteral("除息");
}

QShareProfitTablewidget::QShareProfitTablewidget(QWidget *parent) : HqTableWidget(parent)
{
    //设定抬头
    TableColDataList datalist;
    datalist.append(TableColData(QStringLiteral("序号"), STK_DISPLAY_SORT_TYPE_NONE));
    datalist.append(TableColData(QStringLiteral("代码"), STK_DISPLAY_SORT_TYPE_NONE));
    datalist.append(TableColData(QStringLiteral("名称"), STK_DISPLAY_SORT_TYPE_NONE));
    datalist.append(TableColData(QStringLiteral("数量"), STK_DISPLAY_SORT_TYPE_NONE));
    datalist.append(TableColData(QStringLiteral("盈亏"), STK_DISPLAY_SORT_TYPE_NONE));

    setHeaders(datalist);
    QExchangeRecordWorker* worker = new QExchangeRecordWorker();
    connect(worker, SIGNAL(signalSendShareProfitList(QList<ShareExchangeDataMgr>)), this, SLOT(slotRecvProfitDataList(QList<ShareExchangeDataMgr>)));

}


void QShareProfitTablewidget:: slotRecvProfitDataList(const QList<ShareExchangeDataMgr>& list )
{
    prepareUpdateTable(list.size());
    int i = 0;
    foreach (ShareExchangeDataMgr data, list) {
        int k =0;
        this->setItemText(i, k++, QString::number(i+1));
        this->setItemText(i, k++, data.mCode);
        this->setItemText(i, k++, data.mName);
        this->setItemText(i, k++, QString::number(data.mVol));
        this->setItemText(i, k++, HqUtils::double2Str(data.mProfit));
        this->item(i, 0)->setData(Qt::UserRole, QVariant::fromValue(data));
        i++;
    }

    for(int i=0; i<this->columnCount(); i++)
    {
        updateColumn(i);
    }
}




void QShareProfitTablewidget::slotCellDoubleClicked(int row, int col)
{

}


QShareExchangeTablewidget::QShareExchangeTablewidget(QWidget *parent) : HqTableWidget(parent)
{
    //设定抬头
    TableColDataList datalist;
    datalist.append(TableColData(QStringLiteral("序号"), STK_DISPLAY_SORT_TYPE_NONE));
    datalist.append(TableColData(QStringLiteral("日期"), STK_DISPLAY_SORT_TYPE_NONE));
    datalist.append(TableColData(QStringLiteral("代码"), STK_DISPLAY_SORT_TYPE_NONE));
    datalist.append(TableColData(QStringLiteral("名称"), STK_DISPLAY_SORT_TYPE_NONE));
    datalist.append(TableColData(QStringLiteral("类别"), STK_DISPLAY_SORT_TYPE_NONE));
    datalist.append(TableColData(QStringLiteral("数量"), STK_DISPLAY_SORT_TYPE_NONE));
    datalist.append(TableColData(QStringLiteral("价格"), STK_DISPLAY_SORT_TYPE_NONE));
    datalist.append(TableColData(QStringLiteral("金额"), STK_DISPLAY_SORT_TYPE_NONE));
    datalist.append(TableColData(QStringLiteral("手续费"), STK_DISPLAY_SORT_TYPE_NONE));
    datalist.append(TableColData(QStringLiteral("净流入"), STK_DISPLAY_SORT_TYPE_NONE));

    setHeaders(datalist);
    //开始更新数据
    connect(DATA_SERVICE, SIGNAL(signalSendShareExchangeRecord(int, int, QList<ShareExchangeData>)), this, SLOT(slotRecvDataList(int,int, QList<ShareExchangeData>)));
    setSearchCondition("", "", "");

}


void QShareExchangeTablewidget::setSearchCondition(const QString& code, const QString& start, const QString& end)
{
    //检查条件是否发生了变化
    if(mCode != code || mStartDate != start || mEndDate != end)
    {
        //mCurPage = 0;
        mCode = code;
        mStartDate = start;
        mEndDate = end;
    } else
    {
        //mCurPage++;
    }
//    emit DATA_SERVICE->signalQueryShareExchangeRecord(mCurPage, mCode, mStartDate, mEndDate);
}


void QShareExchangeTablewidget::slotRecvDataList(int page, int total_page, const QList<ShareExchangeData>& list)
{
//    mCurPage = page;
//    mTotalPage = total_page;
//    mPageSize = 50;

    prepareUpdateTable(list.size());
    int i = 0;
    foreach (ShareExchangeData data, list) {
        int k =0;
//        this->setItemText(i, k++, QString::number((mCurPage-1) * mPageSize + 1));
        this->setItemText(i, k++, data.mDateTime);
        this->setItemText(i, k++, data.mCode);
        this->setItemText(i, k++, data.mName);
        this->setItemText(i, k++, ShareExchangeType2String(data.mType));
        this->setItemText(i, k++, QString::number(data.mNum));
        this->setItemText(i, k++, HqUtils::double2Str(data.mPrice));
        this->setItemText(i, k++, HqUtils::double2Str(data.mMoney));
        this->setItemText(i, k++, HqUtils::double2Str(data.mYongjin + data.mYinhuasui + data.mOther));
        this->setItemText(i, k++, HqUtils::double2Str(data.mNetIncome));
        this->item(i, 0)->setData(Qt::UserRole, data.mCode);
        i++;

    }
}



void QShareExchangeTablewidget::displayFirstPage()
{
    emit DATA_SERVICE->signalQueryShareExchangeRecord(1, mCode, mStartDate, mEndDate);
}

void QShareExchangeTablewidget::displayNextPage()
{
    //emit DATA_SERVICE->signalQueryShareExchangeRecord(++mCurPage, mCode, mStartDate, mEndDate);
}


void QShareExchangeTablewidget::displayLastPage()
{
    //emit DATA_SERVICE->signalQueryShareExchangeRecord(mTotalPage, mCode, mStartDate, mEndDate);
}

void QShareExchangeTablewidget::displayPreviousPage()
{
    //emit DATA_SERVICE->signalQueryShareExchangeRecord(--mCurPage, mCode, mStartDate, mEndDate);
}



void QShareExchangeTablewidget::slotCustomContextMenuRequested(const QPoint &pos)
{    
    HqTableWidget::slotCustomContextMenuRequested(pos);
}


void QShareExchangeTablewidget::slotCellDoubleClicked(int row, int col)
{

}

void QShareExchangeTablewidget::slotCellClicked(int row, int col)
{

}
