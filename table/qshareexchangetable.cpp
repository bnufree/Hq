#include "qshareexchangetable.h"
#include <QDebug>
#include "data_structure/hqutils.h"
#include "data_structure/sharedata.h"
#include "dbservices/dbservices.h"
#include "qsahreoptwidget.h"
#include "real/qsinastkresultmergethread.h"
#include "hqtaskmagrcenter.h"


QShareExchangeTablewidget::QShareExchangeTablewidget(QWidget *parent) : HqTableWidget(parent), mUpdateTimer(0), mMergeThread(0)
{
    //设定抬头
    TableColDataList datalist;
    datalist.append(TableColData(QStringLiteral("序号"), STK_DISPLAY_SORT_TYPE_NONE));
    datalist.append(TableColData(QStringLiteral("日期"), STK_DISPLAY_SORT_TYPE_NONE));
    datalist.append(TableColData(QStringLiteral("代码"), STK_DISPLAY_SORT_TYPE_NONE));
    datalist.append(TableColData(QStringLiteral("名称"), STK_DISPLAY_SORT_TYPE_NONE));
    datalist.append(TableColData(QStringLiteral("类别"), STK_DISPLAY_SORT_TYPE_NONE));
    datalist.append(TableColData(QStringLiteral("价格"), STK_DISPLAY_SORT_TYPE_NONE);
    datalist.append(TableColData(QStringLiteral("金额"), STK_DISPLAY_SORT_TYPE_NONE));
    datalist.append(TableColData(QStringLiteral("手续费"), STK_DISPLAY_SORT_TYPE_NONE));
    datalist.append(TableColData(QStringLiteral("净流入"), STK_DISPLAY_SORT_TYPE_NONE));

    setHeaders(datalist);
    initMenu();
    setAutoChangePage(true);
    //开始更新数据
    connect(DATA_SERVICE, SIGNAL(signalSendShareExchangeRecord(int, int, QList<ShareExchangeData>)), this, SLOT(slotRecvDataList(int,int, QList<ShareExchangeData>)));
    setSearchCondition("", "", "");

}


void QShareExchangeTablewidget::setSearchCondition(const QString& code, const QString& start, const QString& end)
{
    mc
    //检查条件是否发生了变化
    if(mCode != code || mStartDate != start || mEndDate != end)
    {

    }
}

void QShareExchangeTablewidget::slotUpdateTimeOut()
{
    if(!mMergeThread) return;
    int page, pagesize;

    ShareDataList list = mMergeThread->getDataList(page, pagesize);
    setDataList(page, pagesize, list, QDateTime::currentMSecsSinceEpoch());
}

void QShareExchangeTablewidget::setDataList(int page, int  pagesize, const ShareDataList &list, qint64 time)
{
//    qDebug()<<"update:"<<page<<pagesize<<list.size()<<"datatime:"<<QDateTime::fromMSecsSinceEpoch(time).toString("hh:mm:ss")<<" current:"<<QDateTime::currentDateTime().toString("hh:mm:ss");
    QTime t;
    t.start();
    prepareUpdateTable(list.size());
    int i = 0;
    foreach (ShareData data, list) {
        int k =0;
//        this->setItemText(i, k++, QString::number((page-1)*pagesize + i+1));
        this->setCodeName(i, k++, data.mCode, data.mName);
        QColor dis_color = data.mChgPercent > 0 ? Qt::red : data.mChgPercent < 0 ? Qt::green : Qt::white;
        this->setItemText(i, k++, HqUtils::double2Str(data.mCur), dis_color);
        double val = mShareMap[data.mCode];
        QString flag = val < data.mChgPercent ? QStringLiteral("↑") : val > data.mChgPercent ? QStringLiteral("↓") : "";
        this->setItemText(i, k++, QString("%1%2").arg(flag).arg(QString::number(data.mChgPercent, 'f', 2)));
        mShareMap[data.mCode] = data.mChgPercent;
        this->setItemText(i, k++, QString("").sprintf("%.2f", data.mMoney / 100000000.0));
        this->setItemText(i, k++, QString("").sprintf("%.2f",data.mMoneyRatio));
//        this->setItemText(i, k++, QString("").sprintf("%.2f",data.mHistory.mLast3DaysChgPers));
//        this->setItemText(i, k++, QString("").sprintf("%.2f",data.mHistory.mLast5DaysChgPers));
//        this->setItemText(i, k++, QString("").sprintf("%.2f",data.mHistory.mLast10DaysChgPers));
        this->setItemText(i, k++, QString("").sprintf("%.2f", data.mHistory.mChgPersFromWeek));
        this->setItemText(i, k++, QString("").sprintf("%.2f",data.mHistory.mChgPersFromMonth));
        this->setItemText(i, k++, QString("").sprintf("%.2f",data.mHistory.mChgPersFromYear));
        this->setItemText(i, k++, QString("").sprintf("%.0f", data.mZJLX));
        this->setItemText(i, k++, QString("").sprintf("%.2f",data.mGXL * 100));
        this->setItemText(i, k++, QString("").sprintf("%.0f",data.mBonusData.mSZZG));
        this->setItemText(i, k++, QString("").sprintf("%.0f",data.mTotalCap / 100000000.0 ));
        this->setItemText(i, k++, QString("").sprintf("%.0f",data.mMutalbleCap/ 100000000.0 ));
        this->setItemText(i, k++, QString("").sprintf("%.0f",data.mFinanceData.mROE));
//        this->setItemText(i, k++, QString("").sprintf("%.0f",data.mProfit));
        this->setItemText(i, k++, QString("").sprintf("%.0f", data.mHsgtData.mVolTotal / 10000.0));
        this->setItemText(i, k++, QString("").sprintf("%.0f", data.mHsgtData.mVolCh1 / 10000.0));
        this->setItemText(i, k++, QString("").sprintf("%.0f", data.mHsgtData.mVolCh5/ 10000.0));
        this->setItemText(i, k++, QString("").sprintf("%.0f", data.mHsgtData.mVolCh10 / 10000.0));
        this->setItemText(i, k++, QString("").sprintf("%.2f", data.mForeignCap / 100000000.0));
        this->setItemText(i, k++, QString("").sprintf("%.2f", data.mForeignCapChg / 100000000.0));
        this->setItemText(i, k++, QString("").sprintf("%.2f",data.mHsl * 100));
        this->setItemText(i, k++, data.mBonusData.mGQDJR.toString());
        this->setItemText(i, k++, data.mBonusData.mYAGGR.toString());
        this->setItemText(i, k++, data.mTime.toString(true));
        this->updateFavShareIconOfRow(i, data.mIsFav);
        this->item(i, 0)->setData(Qt::UserRole, data.mCode);
//        this->item(i, 0)->setData(Qt::UserRole+1, QVariant::fromValue(data.mBlockList));
        this->item(i, 0)->setData(Qt::UserRole+1, data.getBlockCodesList());
        i++;

    }
}

void QShareExchangeTablewidget::setShareMarketType(int type)
{
    if(mMergeThread) mMergeThread->setMktType(type);
}

void QShareExchangeTablewidget::setShareMarket()
{
    QAction *act = (QAction*)sender();
    if(act == NULL) return;
    qDebug()<<"mkt_type:"<<act->data().toInt();
    resetPageDisplay();
    if(mMergeThread) mMergeThread->setMktType(act->data().toInt());
}

void QShareExchangeTablewidget::setSelfShareCodesList(const QStringList &list)
{
    if(mMergeThread) mMergeThread->setSelfCodesList(list);
}

void QShareExchangeTablewidget::setSortType(int type)
{
    if(mMergeThread) mMergeThread->setSortType(type);
}

void QShareExchangeTablewidget::displayFirstPage()
{
    if(mMergeThread) mMergeThread->setDisplayPage(FIRST_PAGE);
}

void QShareExchangeTablewidget::displayNextPage()
{
    if(mMergeThread) mMergeThread->setDisplayPage(NEXT_PAGE);
}


void QShareExchangeTablewidget::displayLastPage()
{
    if(mMergeThread) mMergeThread->setDisplayPage(END_PAGE);
}

void QShareExchangeTablewidget::displayPreviousPage()
{
    if(mMergeThread) mMergeThread->setDisplayPage(PRE_PAGE);
}

void QShareExchangeTablewidget::initMenu()
{
    QMenu* hqCenterMenu = new QMenu(QStringLiteral("行情中心"), this);
    QList<QAction*> actlist;

    QList<struMenu> itemlist;
    itemlist.append(struMenu(QStringLiteral("自选"), MKT_ZXG));
    itemlist.append(struMenu(QStringLiteral("沪深"), MKT_ALL));
    itemlist.append(struMenu(QStringLiteral("沪市"), MKT_SH));
    itemlist.append(struMenu(QStringLiteral("深市"), MKT_SZ));
    itemlist.append(struMenu(QStringLiteral("中小板"), MKT_ZXB));
    itemlist.append(struMenu(QStringLiteral("创业板"), MKT_CYB));
    itemlist.append(struMenu(QStringLiteral("科创板"), MKT_KCB));
    itemlist.append(struMenu(QStringLiteral("沪深基金"), MKT_JJ));
    itemlist.append(struMenu(QStringLiteral("恒指"), MKT_HK_HSZS));
    itemlist.append(struMenu(QStringLiteral("恒生国企"), MKT_HK_HSGQ));
    itemlist.append(struMenu(QStringLiteral("港股通"), MKT_HK_GGT));
    itemlist.append(struMenu(QStringLiteral("陆股通TOP10"), MKT_LGT_TOP10));

    foreach (struMenu item, itemlist) {
        QAction *act = new QAction(this);
        act->setText(item.mDisplayText);
        act->setData(item.mCmd);
        connect(act, &QAction::triggered, this, &QShareExchangeTablewidget::setShareMarket);
        actlist.append(act);
    }

    hqCenterMenu->addActions(actlist);

    insertContextMenu(hqCenterMenu);

}

void QShareExchangeTablewidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    if(mCodesActionList.length() == 0)
    {
        QList<struMenu> itemlist;
        itemlist.append(struMenu(QStringLiteral("分时图"), INFO_MINUTE_GRAPH));
        itemlist.append(struMenu(QStringLiteral("日线图"), INFO_K_GRAPH));
        itemlist.append(struMenu(QStringLiteral("沪深港通"), INFO_HSHK));        
        itemlist.append(struMenu(QStringLiteral("所属板块"), INFO_BLOCK_LIST));
        itemlist.append(struMenu(QStringLiteral("特别关注"), INFO_SPECIAL_CONCER));

        foreach (struMenu menu_item, itemlist) {
            QAction *act = 0;
            if(menu_item.mCmd == INFO_BLOCK_LIST)
            {
                act = insertContextMenu(new QMenu(menu_item.mDisplayText, this));
            } else
            {
                act = new QAction(this);
                act->setText(menu_item.mDisplayText);
                if(menu_item.mCmd == INFO_MINUTE_GRAPH)
                {
                    connect(act, &QAction::triggered, this, &QShareExchangeTablewidget::setDisplayMinuteGraph);
                } else if(menu_item.mCmd == INFO_K_GRAPH)
                {
                    connect(act, &QAction::triggered, this, &QShareExchangeTablewidget::setDisplayDayGraph);
                } else if(menu_item.mCmd == INFO_HSHK)
                {
                    connect(act, &QAction::triggered, this, &QShareExchangeTablewidget::setDisplayHSHK);
                } else if(menu_item.mCmd == INFO_SPECIAL_CONCER)
                {
                    connect(act, &QAction::triggered, this, &QShareExchangeTablewidget::setSpecialConcer);
                }

                insertContextMenu(act);
            }
            act->setData(QVariant::fromValue(menu_item));
            mCodesActionList.append(act);
        }

    }

    //板块菜单
    QTableWidgetItem *table_item = this->itemAt(pos);
    qDebug()<<"item:"<<table_item;
    if(table_item)
    {
        table_item = this->item(table_item->row(), 0);
        QString stkCode = table_item->data(Qt::UserRole).toString();
        foreach (QAction* act, mCodesActionList) {
            struMenu menu_item = act->data().value<struMenu>();
            if(menu_item.mCmd == INFO_BLOCK_LIST)
            {
                QMenu *wk = act->menu();
                wk->clear();
                QStringList list = table_item->data(Qt::UserRole+1).toStringList();
                foreach (QString code, list)
                {
                    //qDebug()<<stkCode<<code;
                    BlockData* block = DATA_SERVICE->getBlockDataOfCode(code);
                    if(!block) continue;
                    if(block->mName.length() == 0) continue;
                    QAction *act = new QAction(this);
                    act->setText(QString("%1:%2%").arg(block->mName).arg(block->mChangePercent));
                    act->setData(block->mShareCodeList);
                    connect(act, &QAction::triggered, this, &QShareExchangeTablewidget::setDisplayBlockDetail);
                    wk->addAction(act);
                }
            } else
            {
                menu_item.mKey.setValue(stkCode);
                act->setData(QVariant::fromValue(menu_item));
            }
            act->setVisible(true);
        }

    } else
    {
        foreach (QAction* act, mCodesActionList) {
            act->setVisible(false);
        }
    }

    HqTableWidget::slotCustomContextMenuRequested(pos);

}

void QShareExchangeTablewidget::setDisplayMinuteGraph()
{

}

void QShareExchangeTablewidget::setDisplayDayGraph()
{

}

void QShareExchangeTablewidget::setDisplayBlockDetail()
{
    QAction *act = qobject_cast<QAction*> (sender());
    if(act)
    {
        QStringList list = act->data().toStringList();
        emit signalSetDisplayBlockDetail(list);
        if(mMergeThread) mMergeThread->setSelfCodesList(list);
    }
}

void QShareExchangeTablewidget::setDisplayHSHK()
{
    QAction *act = qobject_cast<QAction*> (sender());
    if(act)
    {
        struMenu menu_item = act->data().value<struMenu>();
        emit signalSetDisplayHSHK(menu_item.mKey.toString().right(6));
    }
}

void QShareExchangeTablewidget::setSpecialConcer()
{
    QAction *act = qobject_cast<QAction*> (sender());
    if(act)
    {
        struMenu menu_item = act->data().value<struMenu>();
        emit signalSetSpecialConcern(menu_item.mKey.toString().right(6));
        qDebug()<<"special concern code:"<<menu_item.mKey.toString().right(6);
    }
}

void QShareExchangeTablewidget::slotCellDoubleClicked(int row, int col)
{
    QTableWidgetItem *item = this->item(row, 0);
    if(!item) return;
    QString code = item->data(Qt::UserRole).toString();
    QRect rect = this->visualItemRect(item);
    qDebug()<<rect;
    QSahreOptWidget* widget = new QSahreOptWidget(code, this);
    QPoint pos = QCursor::pos();
    QPoint target = this->mapFromGlobal(pos);
    widget->move(target.x() - widget->width() / 2, target.y());
    widget->show();

//    emit signalDoubleClickCode(code);
}

void QShareExchangeTablewidget::slotCellClicked(int row, int col)
{
    QTableWidgetItem *item = this->item(row, 0);
    if(!item) return;
    QString code = item->data(Qt::UserRole).toString();
    emit signalDisplayDetailOfCode(code.right(6));
}
