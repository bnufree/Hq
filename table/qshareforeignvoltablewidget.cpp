#include "qshareforeignvoltablewidget.h"
#include "date/shareworkingdatetime.h"
#include <QPushButton>
#include <QTimer>
#include "qshareforeignvolchangecounterthread.h"
#include "dbservices/dbservices.h"

QShareForeignVolTableWidget::QShareForeignVolTableWidget(QWidget *parent) : HqMergeTableWidget(parent)
{
    //设定抬头
    TableColDataList datalist;
    datalist.append(TableColData(QStringLiteral("名称"), STK_DISPLAY_SORT_TYPE_NAME));
    datalist.append(TableColData(QStringLiteral("最新"), STK_DISPLAY_SORT_TYPE_PRICE));
    datalist.append(TableColData(QStringLiteral("涨跌(%)"), STK_DISPLAY_SORT_TYPE_CHGPER));
    datalist.append(TableColData(QStringLiteral("总市值(亿)"), STK_DISPLAY_SORT_TYPE_TCAP));
    datalist.append(TableColData(QStringLiteral("流通市值(亿)"), STK_DISPLAY_SORT_TYPE_MCAP, true));
    datalist.append(TableColData(QStringLiteral("外资持股(亿)"), STK_DISPLAY_SORT_TYPE_FOREIGN_VOL));
    datalist.append(TableColData(QStringLiteral("持股市值(亿)"), STK_DISPLAY_SORT_TYPE_FOREIGN_CAP));
    datalist.append(TableColData(QStringLiteral("流通占比(%)"), STK_DISPLAY_SORT_TYPE_LTZB));
    datalist.append(TableColData(QStringLiteral("外资持股△1(万)"), STK_DISPLAY_SORT_TYPE_FOREIGN_VOL_CHG));
    datalist.append(TableColData(QStringLiteral("外资持股△5(万)"), STK_DISPLAY_SORT_TYPE_FOREIGN_VOL_CHG5));
    datalist.append(TableColData(QStringLiteral("外资持股△10(万)"), STK_DISPLAY_SORT_TYPE_FOREIGN_VOL_CHG10));
    setHeaders(datalist, datalist.size() - 1);

    mDataThread = new QShareForeignVolChangeCounterThread();
    connect(mDataThread, SIGNAL(signalSendDataList(const QList<ShareForeignVolCounter>, const QString )),
            this, SLOT(slotRecvData(const QList<ShareForeignVolCounter>, const QString)));
    connect(DATA_SERVICE, SIGNAL(signalAllShareCodeList(QStringList)), mDataThread, SLOT(start()));
}

void QShareForeignVolTableWidget::slotRecvData(const QList<ShareForeignVolCounter>& list, const QString& date)
{
    qDebug()<<"recv lgt vol:"<<list.size();
    mDataList = list;
    setTotalRowCount(list.size());
    QTimer::singleShot(100, this, SLOT(updateTable()));
}

void QShareForeignVolTableWidget::setSortType(int type)
{
    if(mDataThread) mDataThread->setSortType(type);
}

void QShareForeignVolTableWidget::updateTable()
{
    QList<ShareForeignVolCounter> list = mDataList.mid(mDisplayRowStart, mMaxDisRow);
//    qDebug()<<"row :"<<mDisplayRowStart<<mMaxDisRow<<list.size();
    prepareUpdateTable(list.size());
    int i = 0;
    foreach (ShareForeignVolCounter data, list) {
        int k =0;
        //固定列的显示
        QString code = QString("").sprintf("%06d", data.mCode);
        mFixTable->setItemText(i, k++, DATA_SERVICE->getShareData(code).mName);;
//        qDebug()<<mFixTable->item(0, 0)->text();
        //移动列的显示
        k = 0;
        QColor dis_color = data.mChangePercent > 0 ? Qt::red : data.mChangePercent < 0 ? Qt::green : Qt::white;
        mMoveTable->setItemText(i, k++, HqUtils::double2Str(data.mPrice), dis_color);
        mMoveTable->setItemText(i, k++, QString("%1").arg(QString::number(data.mChangePercent, 'f', 2)));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.0f", data.mZSZ / 100000000.0));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.0f",data.mLTSZ / 100000000.0));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f", data.mShareHold / 100000000.0));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f",data.mShareSZ / 100000000.0));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f",data.mLTZB * 100));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.0f", data.mChg1.mShareHold_Change / 10000));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.0f",data.mChg5.mShareHold_Change / 10000));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.0f",data.mChg10.mShareHold_Change / 10000));
        mMoveTable->item(i, 0)->setData(Qt::UserRole, data.mCode);
        i++;

    }
}
