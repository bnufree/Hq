#include "qshareMutualbundwidget.h"
#include <QDebug>
#include "utils/hqutils.h"
#include "utils/sharedata.h"
#include "dbservices/dbservices.h"


QShareMutualBundwidget::QShareMutualBundwidget(QWidget *parent) : HqTableWidget(parent)
{
    //设定抬头
    TableColDataList datalist;
    datalist.append(TableColData(QStringLiteral("名称"), STK_DISPLAY_SORT_TYPE_CODE));
    datalist.append(TableColData(QStringLiteral("买入"), STK_DISPLAY_SORT_TYPE_NONE,true));
    datalist.append(TableColData(QStringLiteral("卖出"), STK_DISPLAY_SORT_TYPE_NONE,true));
    datalist.append(TableColData(QStringLiteral("净额"), STK_DISPLAY_SORT_TYPE_MONEYR));
    setHeaders(datalist);
}

void QShareMutualBundwidget::setDataList(const ShareBaseDataList &list)
{
    prepareUpdateTable(list.size());
    int i = 0;
    foreach (ShareBaseData data, list) {
        int k =0;
        this->setItemText(i, k++, data.mName);
        this->setItemText(i, k++, QString("").sprintf("%.2f",data.mTop10Buy / 10000.0));
        this->setItemText(i, k++, QString("").sprintf("%.2f",data.mTop10Sell/ 10000.0));
        this->setItemText(i, k++, QString("").sprintf("%.2f",(data.mTop10Buy-data.mTop10Sell)/10000.0));
        this->item(i,0)->setData(Qt::UserRole,QString::fromStdString(data.mCode));
        i++;
    }
}
