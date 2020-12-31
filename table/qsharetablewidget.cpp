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
    datalist.append(TableColData(QStringLiteral("股息率%"), STK_DISPLAY_SORT_TYPE_GXL, true));
    datalist.append(TableColData(QStringLiteral("送转"), STK_DISPLAY_SORT_TYPE_SZZBL, true));
    datalist.append(TableColData(QStringLiteral("总市值(亿)"), STK_DISPLAY_SORT_TYPE_TCAP));
    datalist.append(TableColData(QStringLiteral("流通市值(亿)"), STK_DISPLAY_SORT_TYPE_MCAP, true));
    datalist.append(TableColData(QStringLiteral("净资产收益率(%)"), STK_DISPLAY_SORT_TYPE_JZCSYL, true));
//    datalist.append(TableColData(QStringLiteral("盈亏"), STK_DISPLAY_SORT_TYPE_PROFIT));
    datalist.append(TableColData(QStringLiteral("外资持股(万)"), STK_DISPLAY_SORT_TYPE_FOREIGN_VOL));
    datalist.append(TableColData(QStringLiteral("外资持股△1(万)"), STK_DISPLAY_SORT_TYPE_FOREIGN_VOL_CHG));
    datalist.append(TableColData(QStringLiteral("外资持股△5(万)"), STK_DISPLAY_SORT_TYPE_FOREIGN_VOL_CHG5));
    datalist.append(TableColData(QStringLiteral("外资持股△10(万)"), STK_DISPLAY_SORT_TYPE_FOREIGN_VOL_CHG10));
    datalist.append(TableColData(QStringLiteral("持股市值(亿)"), STK_DISPLAY_SORT_TYPE_FOREIGN_CAP));
    datalist.append(TableColData(QStringLiteral("持股市值△(亿)"), STK_DISPLAY_SORT_TYPE_FOREIGN_CAP_CHG));
    datalist.append(TableColData(QStringLiteral("换手率(%)"), STK_DISPLAY_SORT_TYPE_HSL));
    datalist.append(TableColData(QStringLiteral("登记日"), STK_DISPLAY_SORT_TYPE_GQDJR));
    datalist.append(TableColData(QStringLiteral("公告日"), STK_DISPLAY_SORT_TYPE_YAGGR));

    setHeaders(datalist, datalist.size() - 1);
    initMenu();
    //开始更新数据
    connect(HQTaskMagrCenter::instance()->hqCenter(), SIGNAL(sendStkDataList(ShareDataList,qint64)),
            this, SLOT(slotRecvAllShareDateList(ShareDataList,qint64)));

}


void QShareTablewidget::slotRecvAllShareDateList(const ShareDataList& list,qint64 time)
{
//    qDebug()<<"recv time:"<<QDateTime::currentDateTime()<<QDateTime::fromMSecsSinceEpoch(time);
    QMutexLocker locker(&mDataMutex);
    mShareDataList = list;
    QTimer::singleShot(100, this, SLOT(updateTable()));
}

void QShareTablewidget::updateTable()
{
    QMutexLocker locker(&mDataMutex);
    qDebug()<<"row start:"<<mDisplayRowStart<<" count:"<<mMaxDisRow;
    ShareDataList list = mShareDataList.mid(mDisplayRowStart, mMaxDisRow);
    prepareUpdateTable(list.size());
    int i = 0;
    foreach (ShareData data, list) {
        int k =0;
//        this->setItemText(i, k++, QString::number((page-1)*pagesize + i+1));
//        this->setCodeName(i, k++, data.mCode, data.mName);
        //固定列的显示
        mFixTable->setItemText(i, k++, data.mName);
        if(data.mIsFav) mFixTable->item(i, k-1)->setTextColor(QColor(255, 100, 100));
        //移动列的显示
        k = 0;
        QColor dis_color = data.mChgPercent > 0 ? Qt::red : data.mChgPercent < 0 ? Qt::green : Qt::white;
        mMoveTable->setItemText(i, k++, HqUtils::double2Str(data.mCur), dis_color);
        double val = mShareMap[data.mCode];
        QString flag = val < data.mChgPercent ? QStringLiteral("↑") : val > data.mChgPercent ? QStringLiteral("↓") : "";
        mMoveTable->setItemText(i, k++, QString("%1%2").arg(flag).arg(QString::number(data.mChgPercent, 'f', 2)));
        mShareMap[data.mCode] = data.mChgPercent;
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f", data.mMoney / 100000000.0));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f",data.mMoneyRatio));
//        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f",data.mHistory.mLast3DaysChgPers));
//        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f",data.mHistory.mLast5DaysChgPers));
//        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f",data.mHistory.mLast10DaysChgPers));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f", data.mHistory.mChgPersFromWeek));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f",data.mHistory.mChgPersFromMonth));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f",data.mHistory.mChgPersFromYear));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.0f", data.mZJLX));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f",data.mGXL * 100));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.0f",data.mBonusData.mSZZG));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.0f",data.mTotalCap / 100000000.0 ));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.0f",data.mMutalbleCap/ 100000000.0 ));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.0f",data.mFinanceData.mROE));
//        this->setItemText(i, k++, QString("").sprintf("%.0f",data.mProfit));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.0f", data.mHsgtData.mVolTotal / 10000.0));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.0f", data.mHsgtData.mCounterMap["1"].mVolChg / 10000.0));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.0f", data.mHsgtData.mCounterMap["5"].mVolChg/ 10000.0));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.0f", data.mHsgtData.mCounterMap["10"].mVolChg / 10000.0));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f", data.mForeignCap / 100000000.0));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f", data.mForeignCapChg / 100000000.0));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f",data.mHsl * 100));
        mMoveTable->setItemText(i, k++, data.mBonusData.mGQDJR.toString());
        mMoveTable->setItemText(i, k++, data.mBonusData.mYAGGR.toString());
        mMoveTable->setItemText(i, k++, data.mTime.toString(true));
        mMoveTable->updateFavShareIconOfRow(i, data.mIsFav);
        mMoveTable->item(i, 0)->setData(Qt::UserRole, data.mCode);
//        mMoveTable->item(i, 0)->setData(Qt::UserRole+1, QVariant::fromValue(data.mBlockList));
        mMoveTable->item(i, 0)->setData(Qt::UserRole+1, data.getBlockCodesList());
        i++;

    }
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

//void QShareTablewidget::slotCellDoubleClicked(int row, int col)
//{
//    QTableWidgetItem *item = this->item(row, 0);
//    if(!item) return;
//    QString code = item->data(Qt::UserRole).toString();
//    QRect rect = this->visualItemRect(item);
//    qDebug()<<rect;
//    QSahreOptWidget* widget = new QSahreOptWidget(code, this);
//    QPoint pos = QCursor::pos();
//    QPoint target = /*this->mapFromGlobal(pos)*/pos;
//    widget->move(target);
//    widget->show();

////    emit signalDoubleClickCode(code);
//}

//void QShareTablewidget::slotCellClicked(int row, int col)
//{
//    QTableWidgetItem *item = this->item(row, 0);
//    if(!item) return;
//    QString code = item->data(Qt::UserRole).toString();
//    emit signalDisplayDetailOfCode(code.right(6));
//}
