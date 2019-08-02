#include "qsharehistorycounterwork.h"
#include "dbservices/dbservices.h"
#include "dbservices/qactivedate.h"

QShareHistoryCounterWork::QShareHistoryCounterWork(const QString& code,const ShareHistoryFileDataList& list, QObject* parent)
    :mCode(code),mList(list),mParent(parent),QRunnable()
{

}

QShareHistoryCounterWork::~QShareHistoryCounterWork()
{

}

QString QShareHistoryCounterWork::getFileName()
{
    //设定保存的文件名
    return QString("%1/%2").arg(HQ_DAY_HISTORY_DIR).arg(mCode);
}

bool QShareHistoryCounterWork::readFile(ShareHistoryFileDataList& list)
{
    QString fileName = getFileName();
    if(!QFile::exists(fileName))
    {
        qDebug()<<__FUNCTION__<<__LINE__<<fileName<<" not exist";
        return false;
    }
    //读取文件
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<__FUNCTION__<<__LINE__<<fileName<<" open error";
        return false;
    }
    while (!file.atEnd() ) {
        ShareHistoryFileData data;
        file.read((char*)(&data), sizeof(ShareHistoryFileData));
        list.append(data);
    }
    file.close();

    return true;
}

void QShareHistoryCounterWork::run()
{
    if(mList.size() == 0) readFile(mList);
    //获取当前交易日的日期
    QDate now = ShareWorkingDate::getCurWorkDay().date();
    QDate yesterday = ShareWorkingDate::getLastWorkDay().date();
    //获取对应的年,月,周对应的参考基准日
    //周参考日对应上周的星期五
    QDate week = now.addDays(-1*(now.dayOfWeek()) - 2);
    //月参考日对应上月的最后一个交易日
    QDate month = QDate(now.year(), now.month(), 1).addDays(-1);
    //年参考日
    QDate year = QDate(now.year(), 1, 1).addDays(-1);

    //开始获取参考日对应的复权价
    int size = mList.size();
    if(size == 0) return;
    double week_p = 0.0, month_p = 0.0, year_p = 0.0;
    bool week_found = false, month_found = false, year_found = false;
    double last_money = mList.last().mMoney;
    //从后开始往前找,找到对应日期或者最靠近日期
    QDate real_week, real_month, real_year;
    for(int i = size - 1; i >= 0; i--)
    {
        ShareHistoryFileData data = mList[i];
        QDate wkDate = QDateTime::fromTime_t(data.mDate).date();
        if(wkDate == yesterday) last_money = data.mMoney;
        if((!week_found) && (wkDate <= week))
        {
            week_found = true;
            week_p = data.mCloseAdjust;
            real_week = wkDate;
        }
        if((!month_found) &&(wkDate <= month))
        {
            month_found = true;
            month_p = data.mCloseAdjust;
            real_month = wkDate;
        }

        if((!year_found) && (wkDate <= year))
        {
            year_found = true;
            year_p = data.mCloseAdjust;
            real_year = wkDate;
        }
        if(week_found && month_found && year_found) break;
    }
    //最后仍然没有找到
    if(!week_found)
    {        
        week_p = mList.first().mLastClose;
        real_week = QDateTime::fromTime_t(mList.first().mDate).date();
    }

    if(!month_found)
    {
        month_p = mList.first().mLastClose;
        real_month = QDateTime::fromTime_t(mList.first().mDate).date();
    }

    if(!year_found)
    {
        year_p = mList.first().mLastClose;
        real_year = QDateTime::fromTime_t(mList.first().mDate).date();
    }


    //获取当前陆股通持股信息的变化情况 1日 5日 10日
    double foreign_now = mList.last().mForeignVolAdjust;
    double foreign_mutaul = mList.last().mForeignMututablePercent;
    double foreign_chg1 = 0.0, foreign_chg5 = 0.0, foreign_chg10 = 0.0;
//    if(mList.size() > 2) qDebug()<<mList[size-1].mForeignVol<<mList[size-1].mForeignMututablePercent<<mList[size-2].mForeignVol<<mList[size-2].mForeignMututablePercent;
    if(size >= 2)
    {
        foreign_chg1 = mList[size-1].mForeignVolAdjust - mList[size-2].mForeignVolAdjust;
    } else
    {
        foreign_chg1 = mList[size-1].mForeignVolAdjust - mList[0].mForeignVolAdjust;
    }
    if(size >= 5)
    {
        foreign_chg5 = mList[size-1].mForeignVolAdjust - mList[size-5].mForeignVolAdjust;
    } else
    {
        foreign_chg5 = mList[size-1].mForeignVolAdjust - mList[0].mForeignVolAdjust;
    }

    if(size >= 10)
    {
        foreign_chg10 = mList[size-1].mForeignVolAdjust - mList[size-10].mForeignVolAdjust;
    } else
    {
        foreign_chg10 = mList[size-1].mForeignVolAdjust - mList[0].mForeignVolAdjust;
    }

    ShareHistoryCounter counter;
    counter.code = mCode;
    counter.foreign_percent = foreign_mutaul;
    counter.foreign_ch1 = foreign_chg1;
    counter.foreign_ch5 = foreign_chg5;
    counter.foreign_ch10 = foreign_chg10;
    counter.foreign_vol = foreign_now;
    counter.lastMoney = last_money;
    counter.monthDay = real_month;
    counter.monthP = month_p;
    counter.weekDay = real_week;
    counter.weekP = week_p;
    counter.yearDay = real_year;
    counter.yearP = year_p;
    qDebug()<<mCode<<real_week.toString("yyyyMMdd")<<week_p<<real_month.toString("yyyyMMdd")<<month_p<<real_year.toString("yyyyMMdd")<<year_p<<foreign_chg1<<foreign_chg5<<foreign_chg10;

    emit DATA_SERVICE->signalUpdateShareCounter(counter);




}
