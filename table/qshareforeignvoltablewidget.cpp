#include "qshareforeignvoltablewidget.h"
#include "date/shareworkingdatetime.h"
#include <QPushButton>
#include <QTimer>
#include "qshareforeignvolchangecounterthread.h"
#include "dbservices/dbservices.h"
#include <QVBoxLayout>
#include "hqtaskmagrcenter.h"


int global_foreign_counter_sort_type = STK_DISPLAY_SORT_TYPE_FOREIGN_JMR_CHG;
int global_foreign_counter_sort_rule = 1;

bool cmp_foreign_counter(const ShareForeignVolCounter& p1, const ShareForeignVolCounter& p2)
{
    int result = 0;
    switch (global_foreign_counter_sort_type)
    {
    case STK_DISPLAY_SORT_TYPE_PRICE:
        result = (p1.mPrice > p2.mPrice ? 1 : -1);
        break;
    case STK_DISPLAY_SORT_TYPE_CHGPER:
        result = (p1.mChangePercent > p2.mChangePercent ? 1 : -1);
        break;
    case STK_DISPLAY_SORT_TYPE_FOREIGN_CAP:
        result = (p1.mShareSZ > p2.mShareSZ ? 1 : -1);
        break;
    case STK_DISPLAY_SORT_TYPE_TCAP:
        result = (p1.mZSZ > p2.mZSZ ? 1 : -1);
        break;
    case STK_DISPLAY_SORT_TYPE_MCAP:
        result = (p1.mLTSZ > p2.mLTSZ ? 1 : -1);
        break;
//    case STK_DISPLAY_SORT_TYPE_FOREIGN_VOL:
//        result = (p1.mShareHold > p2.mShareHold ? 1 : -1);
//        break;
    case STK_DISPLAY_SORT_TYPE_LTZB:
        result = (p1.mLTZB > p2.mLTZB ? 1 : -1);
        break;
    case STK_DISPLAY_SORT_TYPE_FOREIGN_JMR_CHG:
        result = (p1.mChg1.mShareSZ_Change > p2.mChg1.mShareSZ_Change ? 1 : -1);
        break;
    case STK_DISPLAY_SORT_TYPE_FOREIGN_JMR_CHG5:
        result = (p1.mChg5.mShareSZ_Change > p2.mChg5.mShareSZ_Change ? 1 : -1);
        break;
    case STK_DISPLAY_SORT_TYPE_FOREIGN_JMR_CHG10:
        result = (p1.mChg10.mShareSZ_Change > p2.mChg10.mShareSZ_Change ? 1 : -1);
        break;
    default:
        result = (p1.mCode > p2.mCode ? 1 : -1);
        break;
    }

    int sts =  result * global_foreign_counter_sort_rule;

    return sts > 0;
}


QShareForeignVolTableWidget::QShareForeignVolTableWidget(QShareForeignVolChangeCounterThread* thread, QWidget *parent)
    : HqMergeTableWidget(parent)
{
    //设定抬头
    TableColDataList datalist;
    datalist.append(TableColData(QStringLiteral("代码"), STK_DISPLAY_SORT_TYPE_CODE));
    datalist.append(TableColData(QStringLiteral("名称"), STK_DISPLAY_SORT_TYPE_NAME));
//    datalist.append(TableColData(QStringLiteral("收盘价"), STK_DISPLAY_SORT_TYPE_PRICE));
    datalist.append(TableColData(QStringLiteral("涨跌(%)"), STK_DISPLAY_SORT_TYPE_CHGPER));
//    datalist.append(TableColData(QStringLiteral("总市值(亿)"), STK_DISPLAY_SORT_TYPE_TCAP));
//    datalist.append(TableColData(QStringLiteral("流通市值(亿)"), STK_DISPLAY_SORT_TYPE_MCAP, true));
//    datalist.append(TableColData(QStringLiteral("外资持股(亿)"), STK_DISPLAY_SORT_TYPE_FOREIGN_VOL));
    datalist.append(TableColData(QStringLiteral("流通占比(%)"), STK_DISPLAY_SORT_TYPE_LTZB));
    datalist.append(TableColData(QStringLiteral("持股市值(亿)"), STK_DISPLAY_SORT_TYPE_FOREIGN_CAP));    
    datalist.append(TableColData(QStringLiteral("净买入△1(亿)"), STK_DISPLAY_SORT_TYPE_FOREIGN_JMR_CHG));
    datalist.append(TableColData(QStringLiteral("净买入△5(亿)"), STK_DISPLAY_SORT_TYPE_FOREIGN_JMR_CHG5));
    datalist.append(TableColData(QStringLiteral("净买入△10(亿)"), STK_DISPLAY_SORT_TYPE_FOREIGN_JMR_CHG10));
    setHeaders(datalist, datalist.size() - 2);
}

void QShareForeignVolTableWidget::slotRecvData(const QList<ShareForeignVolCounter>& list)
{
    mDataList = list;
    setTotalRowCount(list.size());
    qStableSort(mDataList.begin(), mDataList.end(), cmp_foreign_counter);
//    QTimer::singleShot(100, this, SLOT(updateTable()));
}

void QShareForeignVolTableWidget::setSortType(int type)
{
    if(global_foreign_counter_sort_type == type)
    {
        global_foreign_counter_sort_rule *= (-1);
    } else
    {
        global_foreign_counter_sort_type = type;
        global_foreign_counter_sort_rule = 1;
    }

    slotRecvData(mDataList);
//    HQTaskMagrCenter::instance()->getForeignVolThread()->setSortType(type);
}

void QShareForeignVolTableWidget::updateTableInfo()
{
    if(isHidden()) return;
    QList<ShareForeignVolCounter> list = mDataList.mid(mDisplayRowStart, mMaxDisRow);
//    qDebug()<<"row :"<<mDisplayRowStart<<mMaxDisRow<<list.size();
    prepareUpdateTable(list.size());
    int i = 0;
    foreach (ShareForeignVolCounter data, list) {
        int k =0;
        //固定列的显示
        QString code = QString("").sprintf("%06d", data.mCode);
        mFixTable->setItemText(i, k++, code);
        mFixTable->setItemText(i, k++, DATA_SERVICE->getShareData(code).mName);
//        qDebug()<<mFixTable->item(0, 0)->text();
        //移动列的显示
        k = 0;
        QColor dis_color = data.mChangePercent > 0 ? Qt::red : data.mChangePercent < 0 ? Qt::green : Qt::white;
//        mMoveTable->setItemText(i, k++, HqUtils::double2Str(data.mPrice), dis_color);
        mMoveTable->setItemText(i, k++, QString("%1").arg(QString::number(data.mChangePercent, 'f', 2)));
//        mMoveTable->setItemText(i, k++, QString("").sprintf("%.0f", data.mZSZ / 100000000.0));
//        mMoveTable->setItemText(i, k++, QString("").sprintf("%.0f",data.mLTSZ / 100000000.0));
//        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f", data.mShareHold / 100000000.0));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f",data.mLTZB * 100));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.2f",data.mShareSZ / 100000000.0));        
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.1f", data.mChg1.mShareSZ_Change / 100000000.0));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.1f", data.mChg5.mShareSZ_Change / 100000000.0));
        mMoveTable->setItemText(i, k++, QString("").sprintf("%.1f", data.mChg10.mShareSZ_Change / 100000000.0));
        mMoveTable->item(i, 0)->setData(Qt::UserRole, data.mCode);
        i++;

    }
}


LGTVolDisplayWidget::LGTVolDisplayWidget(QWidget *parent) : QWidget(parent)
{
    mCommonStr = QString::fromUtf8("北向持股数据更新：");
    mTimeLabel = new QLabel(this);
    mTimeLabel->setAlignment(Qt::AlignLeft);
    mTimeLabel->setFixedHeight(HqUtils::convertMM2Pixel(8));
    QFont font1 = mTimeLabel->font();
    font1.setPixelSize(HqUtils::convertMM2Pixel(6));
    mTimeLabel->setFont(font1);
    QVBoxLayout *layout1 = new QVBoxLayout;
    this->setLayout(layout1);
    layout1->setMargin(1);
    layout1->setSpacing(1);
    mTable = new QShareForeignVolTableWidget(0, this);
    layout1->addWidget(mTimeLabel);
    layout1->addWidget(mTable);

    connect(DATA_SERVICE, SIGNAL(signalSendForeignDataList(QList<ShareForeignVolCounter>, QString )),
            this, SLOT(slotRecvData(QList<ShareForeignVolCounter>, QString)));

}

void LGTVolDisplayWidget::slotRecvData(const QList<ShareForeignVolCounter>& list, const QString& date)
{
    mTimeLabel->setText(mCommonStr + date);
    mTable->slotRecvData(list);
}

