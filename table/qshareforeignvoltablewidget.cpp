#include "qshareforeignvoltablewidget.h"
#include "date/shareworkingdatetime.h"
#include <QPushButton>
#include "history/qhkexchangevoldataprocess.h"
#include <QTimer>
#include "qshareforeignvolchangecounterthread.h"

QShareForeignVolTableWidget::QShareForeignVolTableWidget(QWidget *parent) : HqMergeTableWidget(parent)
{
    //设定抬头
    TableColDataList datalist;
    datalist.append(TableColData(QStringLiteral("名称"), STK_DISPLAY_SORT_TYPE_NAME));
    datalist.append(TableColData(QStringLiteral("最新"), STK_DISPLAY_SORT_TYPE_PRICE));
    datalist.append(TableColData(QStringLiteral("涨跌(%)"), STK_DISPLAY_SORT_TYPE_CHGPER));
    datalist.append(TableColData(QStringLiteral("总市值(亿)"), STK_DISPLAY_SORT_TYPE_TCAP));
    datalist.append(TableColData(QStringLiteral("流通市值(亿)"), STK_DISPLAY_SORT_TYPE_MCAP, true));
    datalist.append(TableColData(QStringLiteral("外资持股(万)"), STK_DISPLAY_SORT_TYPE_FOREIGN_VOL));
    datalist.append(TableColData(QStringLiteral("持股市值(亿)"), STK_DISPLAY_SORT_TYPE_FOREIGN_CAP));
    datalist.append(TableColData(QStringLiteral("流通占比(%)"), STK_DISPLAY_SORT_TYPE_FOREIGN_VOL));
    datalist.append(TableColData(QStringLiteral("外资持股△1(万)"), STK_DISPLAY_SORT_TYPE_FOREIGN_VOL_CHG));
    datalist.append(TableColData(QStringLiteral("外资持股△5(万)"), STK_DISPLAY_SORT_TYPE_FOREIGN_VOL_CHG5));
    datalist.append(TableColData(QStringLiteral("外资持股△10(万)"), STK_DISPLAY_SORT_TYPE_FOREIGN_VOL_CHG10));
    setHeaders(datalist, datalist.size() - 1);

    QShareForeignVolChangeCounterThread* thread = new QShareForeignVolChangeCounterThread();
    connect(thread, SIGNAL(signalSendDataList(const QList<ShareForeignVolCounter>, const QString )),
            this, SLOT(slotRecvData(const QList<ShareForeignVolCounter>, const QString)));
    thread->start();
}

void QShareForeignVolTableWidget::slotRecvData(const QList<ShareForeignVolCounter>& list, const QString& date)
{
    mDataList = list;
    QTimer::singleShot(100, this, SLOT(updateTable()));
}

void QShareForeignVolTableWidget::updateTable()
{
    QList<ShareForeignVolCounter> list = mDataList.mid(mDisplayRowStart, mMaxDisRow);
    prepareUpdateTable(list.size());
    int i = 0;
    foreach (ShareForeignVolCounter data, list) {
        int k =0;
        //固定列的显示
        mFixTable->setItemText(i, k++, QString::number(data.mCode));
        //移动列的显示
        k = 0;
        QColor dis_color = data.mChangePercent > 0 ? Qt::red : data.mChangePercent < 0 ? Qt::green : Qt::white;
        mMoveTable->setItemText(i, k++, HqUtils::double2Str(data.mPrice), dis_color);
        mMoveTable->setItemText(i, k++, QString("%1").arg(QString::number(data.mChangePercent, 'f', 2)));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.0f", data.mZSZ / 100000000.0));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.0f",data.mLTSZ / 100000000.0));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f", data.mShareHold));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f",data.mShareSZ / 100000000.0));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f",data.mLTZB));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.0f", data.mChg1.mShareHold_Change));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f",data.mChg5.mShareHold_Change));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.0f",data.mChg10.mShareHold_Change));
        mMoveTable->item(i, 0)->setData(Qt::UserRole, data.mCode);
        i++;

    }
}
