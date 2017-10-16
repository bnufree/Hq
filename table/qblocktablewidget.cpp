#include "qblocktablewidget.h"
#include <QDebug>

QBlockTableWidget::QBlockTableWidget(QWidget *parent) : HqTableWidget(parent)
{
    //设定抬头
    TableColDataList datalist;
    datalist.append(TableColData(QStringLiteral("名称"), STK_DISPLAY_SORT_TYPE_NAME));
    datalist.append(TableColData(QStringLiteral("涨跌"), STK_DISPLAY_SORT_TYPE_CHGPER));
    setHeaders(datalist);
}

void QBlockTableWidget::setDataList(const BlockDataList &list)
{
    prepareUpdateTable(list.size());
    int i = 0;
    foreach (BlockData* data, list) {
        int k =0;
        this->setRowHeight(i, 20);
        this->setItemText(i, k++, data->mName);
        this->setItemText(i, k++, QString("%1%").arg(data->mChangePer, 0, 'f', 2 ));
        this->updateFavShareIconOfRow(i, data->mIsFav);
        this->item(i, 0)->setData(Qt::UserRole, data->mShareCodeList);
        i++;

    }
}

void QBlockTableWidget::slotCellDoubleClicked(int row, int col)
{

}

void QBlockTableWidget::setStockMarket()
{
    QAction *act = (QAction*)sender();
    if(act == NULL) return;
    qDebug()<<"mkt_type:"<<act->data().toInt();
    emit signalSetStockMarket(act->data().toInt());
}

void QBlockTableWidget::initMenu()
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
        connect(act, &QAction::triggered, this, &QBlockTableWidget::setStockMarket);
        actlist.append(act);
    }

    hqCenterMenu->addActions(actlist);

    insertContextMenu(hqCenterMenu);

}

void QBlockTableWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    //自选股编辑
    QTableWidgetItem *table_item = this->itemAt(pos);
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
                    connect(act, &QAction::triggered, this, &QBlockTableWidget::setDisplayBlockDetail);
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
                    connect(act, &QAction::triggered, this, &QBlockTableWidget::setDisplayMinuteGraph);
                } else if(menu_item.mCmd == INFO_K_GRAPH)
                {
                    connect(act, &QAction::triggered, this, &QBlockTableWidget::setDisplayDayGraph);
                } else if(menu_item.mCmd == INFO_HSHK)
                {
                    connect(act, &QAction::triggered, this, &QBlockTableWidget::setDisplayHSHK);
                }

                insertContextMenu(act);
            }
        }

    }

    HqTableWidget::slotCustomContextMenuRequested(pos);

}

void QBlockTableWidget::setDisplayMinuteGraph()
{

}

void QBlockTableWidget::setDisplayDayGraph()
{

}

void QBlockTableWidget::setDisplayBlockDetail()
{

}

void QBlockTableWidget::setDisplayHSHK()
{

}
