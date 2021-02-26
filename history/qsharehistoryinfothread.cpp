#include "qsharehistoryinfothread.h"
#include <QDateTime>
#include <QDebug>
#include "utils/qhttpget.h"
#include "data_structure/hqutils.h"
#include "dbservices/dbservices.h"
#include "dbservices/qactivedate.h"
#include "utils/comdatadefines.h"
#include "utils/hqinfoparseutil.h"
#include "utils/profiles.h"

#define     UPDATE_SEC              "CodeInfo"
qint64      global_file_version = 20210224;

QShareHistoryInfoThread::QShareHistoryInfoThread(const QString& code, const QMap<QDate, ShareForignVolFileData>& list, bool counter, QObject* parent) :
    mCode(code),
    mParent(parent),
    mCounter(counter),
    QRunnable()
{
    mCode = code;
    if(mCode.length() > 6)
    {
        mCode = mCode.right(6);
    }
    mForeignList = list;
}

QShareHistoryInfoThread::~QShareHistoryInfoThread()
{

}

QString QShareHistoryInfoThread::getPath()
{
    //根据code的不同进行选择
    QString dir;
    if(ShareData::shareType(mCode) & SHARE_SH_TOTAL)
    {
        dir.append("sh");
    } else
    {
        dir.append("sz");
    }
    dir.append("/");
    dir.append(mCode.left(3));

    dir = QString("%1/%2").arg(HQ_DAY_HISTORY_DIR).arg(dir);

    QDir path(dir);
    if(!path.exists())
    {
        path.mkpath(dir);
    }

    return dir;
}

QString QShareHistoryInfoThread::getFileName()
{
    //设定保存的文件名
    return QString("%1/%2").arg(getPath()).arg(mCode);
}

bool QShareHistoryInfoThread::readFile(ShareDailyDataList& list, bool& adjust)
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
    bool version_flag = false;
    while (!file.atEnd() ) {
        if(!version_flag)
        {
            qint64 file_version = 0;
            file.read((char*)(&file_version), sizeof (qint64));
//            qDebug()<<"file version:"<<mCode<<file_version;
            if(file_version != global_file_version)
            {
                version_flag = false;
                break;
            } else
            {
                version_flag = true;
            }
        } else
        {
            ShareDailyData data;
            file.read((char*)(&data), sizeof(ShareDailyData));
            list.append(data);
        }
    }
    file.close();
    if(!version_flag)
    {
        file.remove();
    }

    return true;
}


void QShareHistoryInfoThread::run()
{
    QTime t;
    t.start();
//    QDate start = QDate::fromString(PROFILES_INS->value(LIST_DATE, mCode.right(6)).toString(), "yyyy-MM-dd");
    QDate start = QDate::fromString(DATA_SERVICE->getShareData(mCode).mListTime, "yyyy-MM-dd");

    //default is
    QDate start_update_date;
    bool adjust = false;
    ShareDailyDataList list;
    readFile(list, adjust);
    if(list.size() > 0)
    {
        start_update_date =  ShareTradeDateTime(list.last().mDate).date().addDays(1);
        //检查从开始到结束的交易日
        if(start != ShareTradeDateTime(list.first().mDate).date())
        {
            start_update_date = start;
            adjust = true;
        }
    } else
    {
        start_update_date = start;
    }
    //开始更新日线数据到今天
    int new_size = 0;
    ShareDailyDataList new_list;
    bool need_update = false;
    if(start_update_date <= TradeDateMgr::instance()->lastTradeDay())
    {
        need_update = true;
    } else
    {
        need_update = false;
    }
    if(need_update)
    {
//        new_list = HqInfoParseUtil::getShareHistoryDataFrom163(last_update_date.date(), mCode);
        if(new_list.size() == 0) new_list = HqInfoParseUtil::getShareHistoryData(start_update_date, mCode);
    }
    for(int i=0; i<new_list.size(); i++)
    {
        ShareDailyData data = new_list[i];
        data.mCloseAdjust = data.mClose;
        //检查是否有除权处理,如果有就更新前面所有的除权价格
        if(list.size() > 0 && list.last().mClose != data.mLastClose)
        {
            //前一笔的最后价格和当前获取的前一次的最后价格不相同,那么今天就是进行了除权处理,对前面的所有价格都进行除权
            double adjust_price = data.mLastClose / list.last().mClose;
            if(adjust == false) adjust = true;
            //检查股本是否发生了变化
            double share_ratio = 1;
            adjustDataList(list, adjust_price, share_ratio);
        }
//        if(list.size() > 0 && data.mForeignVolOri == 0 && list.last().mForeignVolOri != 0)
//        {
//            data.mForeignVolOri = list.last().mForeignVolOri;
//            data.mForeignMututablePercent = list.last().mForeignMututablePercent;
//        }
        list.append(data);
        new_size++;
    }
    //对新的数据更新陆股痛数据处理
    if(adjust)
    {
        //删除文件,重新写入
        writeFile(list, "wb+");
    } else
    {
        //没有除权,只添加本次新获取的部分
        if(new_size > 0)
        {
            writeFile(list.mid(list.size() - new_size , new_size), "ab+");
        }
    }
    if(mCounter)
    {
        //获取当前交易日的日期
        QDate now = TradeDateMgr::instance()->currentTradeDay();
        QDate last_day = TradeDateMgr::instance()->lastTradeDay();
        //获取对应的年,月,周对应的参考基准日
        //周参考日对应上周的星期五
        QDate week = now.addDays(-1*(now.dayOfWeek()) - 2);
        //月参考日对应上月的最后一个交易日
        QDate month = QDate(now.year(), now.month(), 1).addDays(-1);
        //年参考日
        QDate year = QDate(now.year(), 1, 1).addDays(-1);

        //开始获取参考日对应的复权价
        int size = list.size();
        if(size == 0) return;
        double week_p = 0.0, month_p = 0.0, year_p = 0.0;
        bool week_found = false, month_found = false, year_found = false;
        double last_money = list.last().mMoney;
        //从后开始往前找,找到对应日期或者最靠近日期
        QDate real_week, real_month, real_year;
        for(int i = size - 1; i >= 0; i--)
        {
            ShareDailyData data = list[i];
            QDate wkDate = QDateTime::fromTime_t(data.mDate).date();
            if(wkDate == last_day) last_money = data.mMoney;
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
            week_p = list.first().mLastClose;
            real_week = QDateTime::fromTime_t(list.first().mDate).date();
        }

        if(!month_found)
        {
            month_p = list.first().mLastClose;
            real_month = QDateTime::fromTime_t(list.first().mDate).date();
        }

        if(!year_found)
        {
            year_p = list.first().mLastClose;
            real_year = QDateTime::fromTime_t(list.first().mDate).date();
        }



        ShareHistoryCounter counter;
        counter.code = mCode;
        counter.info.mLastMoney = last_money;
        counter.info.mMonthDay = real_month;
        counter.info.mMonthDayPrice = month_p;
        counter.info.mWeekDay = real_week;
        counter.info.mWeekDayPrice = week_p;
        counter.info.mYearDay = real_year;
        counter.info.mYearDayPrice = year_p;
//        qDebug()<<mCode<<"week:"<<real_week.toString("yyyyMMdd")<<week_p
//                <<"month:"<<real_month.toString("yyyyMMdd")<<month_p
//                <<"year:"<<real_year.toString("yyyyMMdd")<<year_p
//                <<"last_day:"<<last_day.toString("yyyy-MM-dd")<<QString::number(last_money/10000.0, 'f', 0);

//        emit DATA_SERVICE->signalUpdateShareCounter(counter);
        if(mParent)
        {
            QMetaObject::invokeMethod(mParent,\
                                      "slotUpdateShareHistoryProcess",\
                                      Qt::DirectConnection,
                                      Q_ARG(ShareHistoryCounter, counter),
                                      Q_ARG(QString, mCode)
                                      );
        }
    }

//    qDebug()<<"update history "<<mCode<< t.elapsed();
    return;
}

QString QShareHistoryInfoThread::getCode()
{
    return mCode;
}



void QShareHistoryInfoThread::writeFile(const ShareDailyDataList& list, const char* mode)
{
    if(list.size() == 0) return;
    FILE *fp = fopen(getFileName().toStdString().data(), /*"ab+"*/mode);
    if(fp)
    {
        if(QString::fromLatin1(mode).contains("w"))
        {
            fwrite(&global_file_version, sizeof(qint64), 1, fp);
        }
        foreach (ShareDailyData data, list) {
            fwrite(&data, sizeof(ShareDailyData), 1, fp);
//            qDebug()<<QDateTime::fromTime_t(data.mDate).date()<<data.mClose<<data.mMoney / 10000.0;
        }
        fclose(fp);
    }
}

void QShareHistoryInfoThread::adjustDataList(ShareDailyDataList &list, double price, double ratio)
{
    for(int i=0; i<list.size(); i++)
    {
        ShareDailyData &data = list[i];
#if 0
        data.mCloseAdjust += price;
        data.mLastClose += price;
#else
        data.mCloseAdjust *= price;
        data.mLastClose *= price;
#endif
//        data.mForeignVolAdjust = qint64(floor(data.mForeignVolAdjust * ratio));
    }
}



