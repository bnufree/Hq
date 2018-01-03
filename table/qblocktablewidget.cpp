#include "qblocktablewidget.h"
#include <QDebug>

QBlockTableWidget::QBlockTableWidget(QWidget *parent) : HqTableWidget(parent)
{
    //设定抬头
    TableColDataList datalist;
    datalist.append(TableColData(QStringLiteral("名称"), STK_DISPLAY_SORT_TYPE_NAME));
    datalist.append(TableColData(QStringLiteral("涨跌"), STK_DISPLAY_SORT_TYPE_CHGPER));
    setHeaders(datalist);
    initMenu();
}

void QBlockTableWidget::setDataList(const BlockDataVList &list)
{
    prepareUpdateTable(list.size());
    int i = 0;
    foreach (BlockData data, list) {
        int k =0;
        this->setRowHeight(i, 20);
        this->setItemText(i, k++, data.mName);
        this->setItemText(i, k++, QString("%1%").arg(data.mChangePer, 0, 'f', 2 ));
        this->updateFavShareIconOfRow(i, data.mIsFav);
        this->item(i, 0)->setData(Qt::UserRole, data.mShareCodeList);
        i++;

    }
}

void QBlockTableWidget::slotCellDoubleClicked(int row, int col)
{
    QTableWidgetItem *item = this->item(row, col);
    if(!item) return;
    if(col != 0)
    {
        item = this->item(row, 0);
        if(!item) return;
    }

    QStringList codes = item->data(Qt::UserRole).toStringList();
    emit signalDisplayBlockDetailCodesList(codes);

}

void QBlockTableWidget::initMenu()
{
    QList<QAction*> actlist;
    QList<struMenu> itemlist;
    itemlist.append(struMenu(QStringLiteral("行业"), BLOCK_HY));
    itemlist.append(struMenu(QStringLiteral("概念"), BLOCK_GN));
    itemlist.append(struMenu(QStringLiteral("地区"), BLOCK_DQ));
    itemlist.append(struMenu(QStringLiteral("自选"), BLOCK_USER));

    foreach (struMenu item, itemlist) {
        QAction *act = new QAction(this);
        act->setText(item.mDisplayText);
        act->setData(item.mCmd);
        connect(act, &QAction::triggered, this, &QBlockTableWidget::setBlockType);
        insertContextMenu(act);
    }
}

void QBlockTableWidget::slotCustomContextMenuRequested(const QPoint &pos)
{

    HqTableWidget::slotCustomContextMenuRequested(pos);

}

void QBlockTableWidget::setBlockType()
{
    QAction *act = (QAction*) sender();
    if(!act) return;
    int type = act->data().toInt();
    emit signalSetBlockType(type);

}
