#include "qsharetablewidget.h"
#include <QDebug>
#include "data_structure/hqutils.h"
#include "data_structure/sharedata.h"
#include "dbservices/dbservices.h"


QShareTablewidget::QShareTablewidget(QWidget *parent) : HqTableWidget(parent)
{
    //设定抬头
    TableColDataList datalist;
//    datalist.append(TableColData(QStringLiteral("代码"), STK_DISPLAY_SORT_TYPE_CODE));
//    datalist.append(TableColData(QStringLiteral("名称"), STK_DISPLAY_SORT_TYPE_NAME));
    datalist.append(TableColData(QStringLiteral(""), STK_DISPLAY_SORT_TYPE_CODE));
    datalist.append(TableColData(QStringLiteral("现价"), STK_DISPLAY_SORT_TYPE_PRICE));
    datalist.append(TableColData(QStringLiteral("涨跌(%)"), STK_DISPLAY_SORT_TYPE_CHGPER));
    datalist.append(TableColData(QStringLiteral("成交(亿)"), STK_DISPLAY_SORT_TYPE_CJE));
    datalist.append(TableColData(QStringLiteral("资金比"), STK_DISPLAY_SORT_TYPE_MONEYR));
//    datalist.append(TableColData(QStringLiteral("3日变动"), STK_DISPLAY_SORT_TYPE_LAST3));
//    datalist.append(TableColData(QStringLiteral("5日变动"), STK_DISPLAY_SORT_TYPE_LAST5));
//    datalist.append(TableColData(QStringLiteral("10日变动"), STK_DISPLAY_SORT_TYPE_LAST10));
    datalist.append(TableColData(QStringLiteral("周变动"), STK_DISPLAY_SORT_TYPE_LAST_WEEK));
    datalist.append(TableColData(QStringLiteral("月变动"), STK_DISPLAY_SORT_TYPE_LAST_MONTH));
    datalist.append(TableColData(QStringLiteral("年变动"), STK_DISPLAY_SORT_TYPE_LAST_YEAR,true));
    datalist.append(TableColData(QStringLiteral("资金流(万)"), STK_DISPLAY_SORT_TYPE_ZJLX));
    datalist.append(TableColData(QStringLiteral("股息率%"), STK_DISPLAY_SORT_TYPE_GXL, true));
    datalist.append(TableColData(QStringLiteral("送转"), STK_DISPLAY_SORT_TYPE_SZZBL, true));
    datalist.append(TableColData(QStringLiteral("总市值(亿)"), STK_DISPLAY_SORT_TYPE_TCAP));
    datalist.append(TableColData(QStringLiteral("流通市值(亿)"), STK_DISPLAY_SORT_TYPE_MCAP, true));
    datalist.append(TableColData(QStringLiteral("净资产收益率(%)"), STK_DISPLAY_SORT_TYPE_JZCSYL, true));
    datalist.append(TableColData(QStringLiteral("盈亏"), STK_DISPLAY_SORT_TYPE_PROFIT));
    datalist.append(TableColData(QStringLiteral("外资持股(万)"), STK_DISPLAY_SORT_TYPE_FOREIGN_VOL));
    datalist.append(TableColData(QStringLiteral("外资持股△(万)"), STK_DISPLAY_SORT_TYPE_FOREIGN_VOL_CHG));
    datalist.append(TableColData(QStringLiteral("持股市值(亿)"), STK_DISPLAY_SORT_TYPE_FOREIGN_CAP));
    datalist.append(TableColData(QStringLiteral("持股市值△(亿)"), STK_DISPLAY_SORT_TYPE_FOREIGN_CAP_CHG));
    datalist.append(TableColData(QStringLiteral("换手率%"), STK_DISPLAY_SORT_TYPE_HSL));
    datalist.append(TableColData(QStringLiteral("登记日"), STK_DISPLAY_SORT_TYPE_GQDJR));
    datalist.append(TableColData(QStringLiteral("公告日"), STK_DISPLAY_SORT_TYPE_YAGGR));
    datalist.append(TableColData(QStringLiteral("时间"), STK_DISPLAY_SORT_TYPE_NONE));

    setHeaders(datalist);
    initMenu();
    setAutoChangePage(true);
}

void QShareTablewidget::setDataList(const ShareDataList &list)
{
    prepareUpdateTable(list.size());
    int i = 0;
    foreach (ShareData data, list) {
        int k =0;
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
        this->setItemText(i, k++, QString("").sprintf("%.0f",data.mProfit));
        this->setItemText(i, k++, QString("").sprintf("%.0f", data.mHsgtData.mVolTotal / 10000.0));
        this->setItemText(i, k++, QString("").sprintf("%.0f", data.mHsgtData.mVolChange / 10000.0));
        this->setItemText(i, k++, QString("").sprintf("%.0f", data.mForeignCap / 100000000.0));
        this->setItemText(i, k++, QString("").sprintf("%.0f", data.mForeignCapChg / 100000000.0));
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

void QShareTablewidget::setShareMarket()
{
    QAction *act = (QAction*)sender();
    if(act == NULL) return;
    qDebug()<<"mkt_type:"<<act->data().toInt();
    emit signalSetShareMarket(act->data().toInt());
}

void QShareTablewidget::initMenu()
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
        connect(act, &QAction::triggered, this, &QShareTablewidget::setShareMarket);
        actlist.append(act);
    }

    hqCenterMenu->addActions(actlist);

    insertContextMenu(hqCenterMenu);

}

void QShareTablewidget::slotCustomContextMenuRequested(const QPoint &pos)
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
                    connect(act, &QAction::triggered, this, &QShareTablewidget::setDisplayMinuteGraph);
                } else if(menu_item.mCmd == INFO_K_GRAPH)
                {
                    connect(act, &QAction::triggered, this, &QShareTablewidget::setDisplayDayGraph);
                } else if(menu_item.mCmd == INFO_HSHK)
                {
                    connect(act, &QAction::triggered, this, &QShareTablewidget::setDisplayHSHK);
                } else if(menu_item.mCmd == INFO_SPECIAL_CONCER)
                {
                    connect(act, &QAction::triggered, this, &QShareTablewidget::setSpecialConcer);
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
                    connect(act, &QAction::triggered, this, &QShareTablewidget::setDisplayBlockDetail);
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

void QShareTablewidget::setDisplayMinuteGraph()
{

}

void QShareTablewidget::setDisplayDayGraph()
{

}

void QShareTablewidget::setDisplayBlockDetail()
{
    QAction *act = qobject_cast<QAction*> (sender());
    if(act)
    {
        emit signalSetDisplayBlockDetail(act->data().toStringList());
    }
}

void QShareTablewidget::setDisplayHSHK()
{
    QAction *act = qobject_cast<QAction*> (sender());
    if(act)
    {
        struMenu menu_item = act->data().value<struMenu>();
        emit signalSetDisplayHSHK(menu_item.mKey.toString().right(6));
    }
}

void QShareTablewidget::setSpecialConcer()
{
    QAction *act = qobject_cast<QAction*> (sender());
    if(act)
    {
        struMenu menu_item = act->data().value<struMenu>();
        emit signalSetSpecialConcern(menu_item.mKey.toString().right(6));
        qDebug()<<"special concern code:"<<menu_item.mKey.toString().right(6);
    }
}

void QShareTablewidget::slotCellDoubleClicked(int row, int col)
{
    if(col == 0)
    {
        QTableWidgetItem *item = this->item(row, 0);
        if(!item) return;
        QString code = item->data(Qt::UserRole).toString();
        emit signalSetFavCode(code);
    }
    return;
}
