#include "qsharetablewidget.h"
#include <QDebug>

QShareTablewidget::QShareTablewidget(QWidget *parent) : HqTableWidget(parent)
{
    //设定抬头
    TableColDataList datalist;
    datalist.append(TableColData(QStringLiteral("代码"), STK_DISPLAY_SORT_TYPE_CODE));
    datalist.append(TableColData(QStringLiteral("名称"), STK_DISPLAY_SORT_TYPE_NAME));
    datalist.append(TableColData(QStringLiteral("现价"), STK_DISPLAY_SORT_TYPE_PRICE));
    datalist.append(TableColData(QStringLiteral("涨跌"), STK_DISPLAY_SORT_TYPE_CHGPER));
    datalist.append(TableColData(QStringLiteral("成交"), STK_DISPLAY_SORT_TYPE_CJE));
    datalist.append(TableColData(QStringLiteral("资金比"), STK_DISPLAY_SORT_TYPE_MONEYR));
    datalist.append(TableColData(QStringLiteral("3日"), STK_DISPLAY_SORT_TYPE_LAST3));
    datalist.append(TableColData(QStringLiteral("资金流"), STK_DISPLAY_SORT_TYPE_ZJLX));
    datalist.append(TableColData(QStringLiteral("股息率"), STK_DISPLAY_SORT_TYPE_GXL));
    datalist.append(TableColData(QStringLiteral("送转"), STK_DISPLAY_SORT_TYPE_SZZBL));
    datalist.append(TableColData(QStringLiteral("总市值"), STK_DISPLAY_SORT_TYPE_TCAP));
    datalist.append(TableColData(QStringLiteral("流通市值"), STK_DISPLAY_SORT_TYPE_MCAP));
    datalist.append(TableColData(QStringLiteral("盈亏"), STK_DISPLAY_SORT_TYPE_PROFIT));
    datalist.append(TableColData(QStringLiteral("外资持股"), STK_DISPLAY_SORT_TYPE_FOREIGN_VOL));
    datalist.append(TableColData(QStringLiteral("外资持股△"), STK_DISPLAY_SORT_TYPE_FOREIGN_VOL_CHG));
    datalist.append(TableColData(QStringLiteral("持股市值"), STK_DISPLAY_SORT_TYPE_FOREIGN_CAP));
    datalist.append(TableColData(QStringLiteral("持股市值△"), STK_DISPLAY_SORT_TYPE_FOREIGN_CAP_CHG));
    datalist.append(TableColData(QStringLiteral("登记日"), STK_DISPLAY_SORT_TYPE_GQDJR));
    datalist.append(TableColData(QStringLiteral("公告日"), STK_DISPLAY_SORT_TYPE_YAGGR));
    datalist.append(TableColData(QStringLiteral("换手率"), STK_DISPLAY_SORT_TYPE_HSL));

    setHeaders(datalist);
    initMenu();
}

void QShareTablewidget::setDataList(const StockDataList &list)
{
    prepareUpdateTable(list.size());
    int i = 0;
    foreach (StockData data, list) {
        int k =0;
        this->setRowHeight(i, 20);
        this->setItemText(i, k++, data.mCode, Qt::AlignRight);
        this->setItemText(i, k++, data.mName);
        this->setItemText(i, k++, QString("").sprintf("%.2f", data.mCur));
        double val = mStockMap[data.mCode];
        QString flag = val > data.mChgPercent ? QStringLiteral("↑") : val < data.mChgPercent ? QStringLiteral("↓") : "";
        this->setItemText(i, k++, QString("%1%2%").arg(flag).arg(QString::number(data.mChgPercent, 'f', 2)));
        mStockMap[data.mCode] = data.mChgPercent;
        if(data.mMoney >= 1000){
            this->setItemText(i, k++, QString("").sprintf("%.2f", data.mMoney / 10000.0) + QStringLiteral("亿"));
        } else {
            this->setItemText(i, k++, QString("").sprintf("%.0f", data.mMoney) + QStringLiteral("万"));
        }
        this->setItemText(i, k++, QString("").sprintf("%.2f",data.mMoneyRatio));
        this->setItemText(i, k++, QString("").sprintf("%.2f",data.mLast3DaysChgPers));
        if(fabs(data.mZJLX) >= 1000){
            this->setItemText(i, k++, QString("").sprintf("%.2f", data.mZJLX / 10000.0) + QStringLiteral("亿"));
        } else {
            this->setItemText(i, k++, QString("").sprintf("%.0f", data.mZJLX) + QStringLiteral("万"));
        }
        this->setItemText(i, k++, QString("").sprintf("%.2f%(%.2f)",data.mGXL * 100, data.mXJFH));
        this->setItemText(i, k++, QString("").sprintf("%.0f",data.mSZZG));
        this->setItemText(i, k++, QString("").sprintf("%.0f",data.mTotalCap / 100000000.0 ) + QStringLiteral("亿"));
        this->setItemText(i, k++, QString("").sprintf("%.0f",data.mMutalbleCap/ 100000000.0 )+ QStringLiteral("亿"));

        this->setItemText(i, k++, QString("").sprintf("%.0f",data.mProfit));
        if(data.mForeignVol >= 10000000){
            this->setItemText(i, k++, QString("").sprintf("%.2f", data.mForeignVol / 100000000.0) + QStringLiteral("亿"));
        } else {
            this->setItemText(i, k++, QString("").sprintf("%.2f", data.mForeignVol / 10000.0)  + QStringLiteral("万"));
        }
        if(data.mForeignVolChg >= 10000000){
            this->setItemText(i, k++, QString("").sprintf("%.2f", data.mForeignVolChg / 100000000.0) + QStringLiteral("亿"));
        } else {
            this->setItemText(i, k++, QString("").sprintf("%.2f", data.mForeignVolChg / 10000.0)  + QStringLiteral("万"));
        }
        if(data.mForeignCap >= 1000000){
            this->setItemText(i, k++, QString("").sprintf("%.2f", data.mForeignCap / 100000000.0) + QStringLiteral("亿"));
        } else {
            this->setItemText(i, k++, QString("").sprintf("%.0f", data.mForeignCap / 10000.0) + QStringLiteral("万"));
        }

        if(data.mForeignCapChg >= 1000000){
            this->setItemText(i, k++, QString("").sprintf("%.2f", data.mForeignCapChg / 100000000.0) + QStringLiteral("亿"));
        } else {
            this->setItemText(i, k++, QString("").sprintf("%.0f", data.mForeignCapChg / 10000.0) + QStringLiteral("万"));
        }
        this->setItemText(i, k++, data.mGQDJR.toString("yyyy-MM-dd"));
        this->setItemText(i, k++, data.mYAGGR.toString("yyyy-MM-dd"));
        this->updateFavShareIconOfRow(i, data.mIsFavCode);
        this->item(i, 0)->setData(Qt::UserRole, data.mCode);
        this->item(i, 0)->setData(Qt::UserRole+1, QVariant::fromValue(data.mBlockList));
        i++;

    }
}

void QShareTablewidget::setStockMarket()
{
    QAction *act = (QAction*)sender();
    if(act == NULL) return;
    qDebug()<<"mkt_type:"<<act->data().toInt();
    emit signalSetStockMarket(act->data().toInt());
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
    itemlist.append(struMenu(QStringLiteral("沪深基金"), MKT_JJ));
    itemlist.append(struMenu(QStringLiteral("恒指"), MKT_HK_HSZS));
    itemlist.append(struMenu(QStringLiteral("恒生国企"), MKT_HK_HSGQ));
    itemlist.append(struMenu(QStringLiteral("港股通"), MKT_HK_GGT));

    foreach (struMenu item, itemlist) {
        QAction *act = new QAction(this);
        act->setText(item.mDisplayText);
        act->setData(item.mCmd);
        connect(act, &QAction::triggered, this, &QShareTablewidget::setStockMarket);
        actlist.append(act);
    }

    hqCenterMenu->addActions(actlist);

    insertContextMenu(hqCenterMenu);

}

void QShareTablewidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    //自选股编辑
    QTableWidgetItem *table_item = this->itemAt(pos);
    qDebug()<<"item:"<<table_item;
    if(table_item)
    {
        table_item = this->item(table_item->row(), 0);
        QString stkCode = table_item->data(Qt::UserRole).toString();
        QList<struMenu> itemlist;
        itemlist.append(struMenu(QStringLiteral("分时图"), INFO_MINUTE_GRAPH));
        itemlist.append(struMenu(QStringLiteral("日线图"), INFO_K_GRAPH));
        itemlist.append(struMenu(QStringLiteral("沪深港通"), INFO_HSHK));
        itemlist.append(struMenu(QStringLiteral("所属板块"), INFO_BLOCK_LIST));

        foreach (struMenu menu_item, itemlist) {
            if(menu_item.mCmd == INFO_BLOCK_LIST)
            {
                QMenu *submenu = new QMenu(menu_item.mDisplayText, this);
                QList<BlockData*> blocklist = table_item->data(Qt::UserRole+1).value<QList<BlockData*>>();
                //qDebug()<<"blocklist:"<<blocklist<<" code:"<<item->data(Qt::UserRole).toString();
                foreach (BlockData* block, blocklist) {
                    QAction *act = new QAction(this);
                    act->setText(QString("%1:%2%").arg(block->mName).arg(block->mChangePer));
                    act->setData(block->mShareCodeList);
                    connect(act, &QAction::triggered, this, &QShareTablewidget::setDisplayBlockDetail);
                    submenu->addAction(act);
                }
                insertContextMenu(submenu);
            } else
            {
                QAction *act = new QAction(this);
                act->setText(menu_item.mDisplayText);
                act->setData(stkCode);
                if(menu_item.mCmd == INFO_MINUTE_GRAPH)
                {
                    connect(act, &QAction::triggered, this, &QShareTablewidget::setDisplayMinuteGraph);
                } else if(menu_item.mCmd == INFO_K_GRAPH)
                {
                    connect(act, &QAction::triggered, this, &QShareTablewidget::setDisplayDayGraph);
                } else if(menu_item.mCmd == INFO_HSHK)
                {
                    connect(act, &QAction::triggered, this, &QShareTablewidget::setDisplayHSHK);
                }

                insertContextMenu(act);
            }
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

}

void QShareTablewidget::setDisplayHSHK()
{

}

void QShareTablewidget::slotCellDoubleClicked(int row, int col)
{
    return;
}
