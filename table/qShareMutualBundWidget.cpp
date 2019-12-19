#include "qshareMutualbundwidget.h"
#include <QDebug>
#include "data_structure/hqutils.h"
#include "data_structure/sharedata.h"
#include "dbservices/dbservices.h"


QShareMutualBundwidget::QShareMutualBundwidget(QWidget *parent) : HqTableWidget(parent)
{
    //设定抬头
    TableColDataList datalist;
    datalist.append(TableColData(QStringLiteral(""), STK_DISPLAY_SORT_TYPE_NONE));
    datalist.append(TableColData(QStringLiteral("名称"), STK_DISPLAY_SORT_TYPE_CODE));
    datalist.append(TableColData(QStringLiteral("总计"), STK_DISPLAY_SORT_TYPE_NONE,true));
    datalist.append(TableColData(QStringLiteral("净额"), STK_DISPLAY_SORT_TYPE_MONEYR));
    datalist.append(TableColData(QStringLiteral("日期"), STK_DISPLAY_SORT_TYPE_NONE));
    setHeaders(datalist);
}

void QShareMutualBundwidget::setDataList(const ShareHsgtList &list)
{
    prepareUpdateTable(list.size());
    int i = 0;
    foreach (ShareHsgt data, list) {
        int k =0;
        this->setItemText(i, k++, QString::number(i+1));
        this->setItemText(i, k++, data.mName);
        this->setItemText(i, k++, QString("").sprintf("%.0f",(data.mBuy + data.mSell) / 10000.0));
        this->setItemText(i, k++, QString("").sprintf("%.0f",(data.mPure)/10000.0));
        this->setItemText(i, k++, data.mDate.toString());
        this->item(i,0)->setData(Qt::UserRole,data.mCode);
        i++;
    }
}

void QShareMutualBundwidget::slotCellDoubleClicked(int row, int col)
{
    QTableWidgetItem* item = this->item(row, 0);
    if(!item) return;
    emit signalDisplayMutualBundHistory(item->data(Qt::UserRole).toString());
}
