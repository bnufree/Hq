#include "qsharetablewidget.h"
#include <QDebug>
#include "data_structure/hqutils.h"
#include "data_structure/sharedata.h"
#include "dbservices/dbservices.h"
#include "qsahreoptwidget.h"
#include "real/qsinastkresultmergethread.h"
#include "hqtaskmagrcenter.h"


QShareTablewidget::QShareTablewidget(QWidget *parent) : HqMergeTableWidget(parent)
{
    //设定抬头
    TableColDataList datalist;
    datalist.append(TableColData(QStringLiteral("名称"), STK_DISPLAY_SORT_TYPE_NAME));
    datalist.append(TableColData(QStringLiteral("最新"), STK_DISPLAY_SORT_TYPE_PRICE));
    datalist.append(TableColData(QStringLiteral("涨跌(%)"), STK_DISPLAY_SORT_TYPE_CHGPER));
    datalist.append(TableColData(QStringLiteral("成交(亿)"), STK_DISPLAY_SORT_TYPE_CJE));
    datalist.append(TableColData(QStringLiteral("资金比"), STK_DISPLAY_SORT_TYPE_MONEYR));
    datalist.append(TableColData(QStringLiteral("周变动"), STK_DISPLAY_SORT_TYPE_LAST_WEEK));
    datalist.append(TableColData(QStringLiteral("月变动"), STK_DISPLAY_SORT_TYPE_LAST_MONTH));
    datalist.append(TableColData(QStringLiteral("年变动"), STK_DISPLAY_SORT_TYPE_LAST_YEAR,true));
    datalist.append(TableColData(QStringLiteral("资金流(万)"), STK_DISPLAY_SORT_TYPE_ZJLX));
//    datalist.append(TableColData(QStringLiteral("股息率%"), STK_DISPLAY_SORT_TYPE_GXL, true));
//    datalist.append(TableColData(QStringLiteral("送转"), STK_DISPLAY_SORT_TYPE_SZZBL, true));
    datalist.append(TableColData(QStringLiteral("总市值(亿)"), STK_DISPLAY_SORT_TYPE_TCAP));
    datalist.append(TableColData(QStringLiteral("流通市值(亿)"), STK_DISPLAY_SORT_TYPE_MCAP, true));
//    datalist.append(TableColData(QStringLiteral("净资产收益率(%)"), STK_DISPLAY_SORT_TYPE_JZCSYL, true));
//    datalist.append(TableColData(QStringLiteral("盈亏"), STK_DISPLAY_SORT_TYPE_PROFIT));
    datalist.append(TableColData(QStringLiteral("持股市值(亿)"), STK_DISPLAY_SORT_TYPE_FOREIGN_CAP));
    datalist.append(TableColData(QStringLiteral("净买入△1(万)"), STK_DISPLAY_SORT_TYPE_FOREIGN_JMR_CHG));
    datalist.append(TableColData(QStringLiteral("净买入△5(万)"), STK_DISPLAY_SORT_TYPE_FOREIGN_JMR_CHG5));
    datalist.append(TableColData(QStringLiteral("净买入△10(万)"), STK_DISPLAY_SORT_TYPE_FOREIGN_JMR_CHG10));
    datalist.append(TableColData(QStringLiteral("换手率(%)"), STK_DISPLAY_SORT_TYPE_HSL));
//    datalist.append(TableColData(QStringLiteral("登记日"), STK_DISPLAY_SORT_TYPE_GQDJR));
//    datalist.append(TableColData(QStringLiteral("公告日"), STK_DISPLAY_SORT_TYPE_YAGGR));

    setHeaders(datalist, datalist.size() - 1);
    initMenu();
    //开始更新数据
    connect(HQTaskMagrCenter::instance()->hqCenter(), SIGNAL(sendStkDataList(ShareDataList,qint64)),
            this, SLOT(slotRecvAllShareDateList(ShareDataList,qint64)));

}


void QShareTablewidget::slotRecvAllShareDateList(const ShareDataList& list,qint64 time)
{
    qDebug()<<"recv time:"<<QDateTime::currentDateTime()<<QDateTime::fromMSecsSinceEpoch(time)<<QThread::currentThread();
//    QMutexLocker locker(&mDataMutex);
    mShareDataList = list;
//    QTimer::singleShot(100, this, SLOT(updateTable()));
//    updateTable();
}

void QShareTablewidget::updateTableInfo()
{
    if(isHidden()) return;
    QTime t;
    t.start();
    ShareDataList list;
    if(1)
    {
//        QMutexLocker locker(&mDataMutex);
        setTotalRowCount(mShareDataList.size());
        list = mShareDataList.mid(mDisplayRowStart, mMaxDisRow);
    }
    prepareUpdateTable(list.size());
    int i = 0;
    foreach (ShareData data, list) {
        int k =0;
//        this->setItemText(i, k++, QString::number((page-1)*pagesize + i+1));
//        this->setCodeName(i, k++, data.mCode, data.mName);
        //固定列的显示
        mFixTable->setItemText(i, k++, data.mName);
        if(data.mIsFav) mFixTable->item(i, k-1)->setTextColor(QColor(255, 100, 100));
        mFixTable->item(i, 0)->setData(Qt::UserRole, data.mCode);
        //移动列的显示
        k = 0;
        QColor dis_color = data.mRealInfo.mChgPercent > 0 ? Qt::red : data.mRealInfo.mChgPercent < 0 ? Qt::green : Qt::white;
        mMoveTable->setItemText(i, k++, HqUtils::double2Str(data.mRealInfo.mClose), dis_color);
        double val = mShareMap[data.mCode];
        QString flag = val < data.mRealInfo.mChgPercent ? QStringLiteral("↑") : val > data.mRealInfo.mChgPercent ? QStringLiteral("↓") : "";
        mMoveTable->setItemText(i, k++, QString("%1%2").arg(flag).arg(QString::number(data.mRealInfo.mChgPercent, 'f', 2)));
        mShareMap[data.mCode] = data.mRealInfo.mChgPercent;
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f", data.mRealInfo.mMoney / 100000000.0));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f",data.mRealInfo.mMoneyRatio));
//        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f",data.mHistory.mLast3DaysChgPers));
//        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f",data.mHistory.mLast5DaysChgPers));
//        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f",data.mHistory.mLast10DaysChgPers));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f", data.mCounterInfo.mWeekChgPer));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f",data.mCounterInfo.mMonthChgPer));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f",data.mCounterInfo.mYearChgPer));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.0f", data.mRealInfo.mZJLX));
//        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f",data.mGXL * 100));
//        mMoveTable->setItemText(i, k++, QString("").sprintf("%.0f",data.mBonusData.mSZZG));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.0f",data.mRealInfo.mTotalCap / 100000000.0 ));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.0f",data.mRealInfo.mMutalbleCap/ 100000000.0 ));
//        mMoveTable->setItemText(i, k++, QString("").sprintf("%.0f",data.mFinanceData.mROE));
//        this->setItemText(i, k++, QString("").sprintf("%.0f",data.mProfit));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f", data.mForeignInfo.mCap / 100000000.0));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.0f", data.mForeignInfo.mJMR1 / 10000.0));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.0f", data.mForeignInfo.mJMR5/ 10000.0));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.0f", data.mForeignInfo.mJMR10 / 10000.0));
//        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f", data.mForeignCap / 100000000.0));
//        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f", data.mForeignCapChg / 100000000.0));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f",data.mRealInfo.mHsl * 100));
//        mMoveTable->setItemText(i, k++, data.mBonusData.mGQDJR.toString());
//        mMoveTable->setItemText(i, k++, data.mBonusData.mYAGGR.toString());
        mMoveTable->setItemText(i, k++, data.mRealInfo.mTime.toString(true));
        mMoveTable->updateFavShareIconOfRow(i, data.mIsFav);
        mMoveTable->item(i, 0)->setData(Qt::UserRole, data.mCode);
//        mMoveTable->item(i, 0)->setData(Qt::UserRole+1, QVariant::fromValue(data.mBlockList));
        mMoveTable->item(i, 0)->setData(Qt::UserRole+1, data.getBlockCodesList());
        i++;
    }
    qDebug()<<"update share table cost me:"<<t.elapsed();
}

void QShareTablewidget::setShareMarketType(int type)
{
    HQTaskMagrCenter::instance()->hqCenter()->setMktType(type);
    resetDisplayRows();
}


void QShareTablewidget::setSelfShareCodesList(const QStringList &list)
{
    HQTaskMagrCenter::instance()->hqCenter()->setSelfCodesList(list);
}

void QShareTablewidget::setSortType(int type)
{
    HQTaskMagrCenter::instance()->hqCenter()->setSortType(type);
    resetDisplayRows();
}

void QShareTablewidget::initMenu()
{
    mMktTypeList.clear();
    mMktTypeList.append(struMenu(QStringLiteral("自选"), MKT_ZXG));
    mMktTypeList.append(struMenu(QStringLiteral("沪深"), MKT_ALL));
    mMktTypeList.append(struMenu(QStringLiteral("沪市"), MKT_SH));
    mMktTypeList.append(struMenu(QStringLiteral("深市"), MKT_SZ));
    mMktTypeList.append(struMenu(QStringLiteral("中小板"), MKT_ZXB));
    mMktTypeList.append(struMenu(QStringLiteral("创业板"), MKT_CYB));
    mMktTypeList.append(struMenu(QStringLiteral("科创板"), MKT_KCB));
    mMktTypeList.append(struMenu(QStringLiteral("沪深ETF"), MKT_JJ));
    mMktTypeList.append(struMenu(QStringLiteral("可转债"), MKT_KZZ));
    mMktTypeList.append(struMenu(QStringLiteral("恒指"), MKT_HK_HSZS));
    mMktTypeList.append(struMenu(QStringLiteral("恒生国企"), MKT_HK_HSGQ));
    mMktTypeList.append(struMenu(QStringLiteral("港股通"), MKT_HK_GGT));
    mMktTypeList.append(struMenu(QStringLiteral("陆股通TOP10"), MKT_LGT_TOP10));
}


//void QShareTablewidget::setDisplayMinuteGraph()
//{

//}

//void QShareTablewidget::setDisplayDayGraph()
//{

//}

//void QShareTablewidget::setDisplayBlockDetail()
//{
//    QAction *act = qobject_cast<QAction*> (sender());
//    if(act)
//    {
//        QStringList list = act->data().toStringList();
//        emit signalSetDisplayBlockDetail(list);
//        HQTaskMagrCenter::instance()->hqCenter()->setSelfCodesList(list);
//    }
//}

//void QShareTablewidget::setDisplayHSHK()
//{
//    QAction *act = qobject_cast<QAction*> (sender());
//    if(act)
//    {
//        struMenu menu_item = act->data().value<struMenu>();
//        emit signalSetDisplayHSHK(menu_item.mKey.toString().right(6));
//    }
//}

//void QShareTablewidget::setSpecialConcer()
//{
//    QAction *act = qobject_cast<QAction*> (sender());
//    if(act)
//    {
//        struMenu menu_item = act->data().value<struMenu>();
//        emit signalSetSpecialConcern(menu_item.mKey.toString().right(6));
//        qDebug()<<"special concern code:"<<menu_item.mKey.toString().right(6);
//    }
//}

void QShareTablewidget::slotCellDoubleClicked(int row, int col)
{
    HqSingleTableWidget* w = qobject_cast<HqSingleTableWidget*>(sender());
    if(!w) return;
    QTableWidgetItem* item = w->item(row, 0);
    if(!item) return;
    QString code = item->data(Qt::UserRole).toString();
    if(code.size() > 0)
    {
        DATA_SERVICE->signalSetFavCode(code);
    }
}

//void QShareTablewidget::slotCellClicked(int row, int col)
//{
//    QTableWidgetItem *item = this->item(row, 0);
//    if(!item) return;
//    QString code = item->data(Qt::UserRole).toString();
//    emit signalDisplayDetailOfCode(code.right(6));
//}
