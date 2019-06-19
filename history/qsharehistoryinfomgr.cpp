#include "qsharehistoryinfomgr.h"
#include "qsharehistoryinfothread.h"
#include "qsharehistoryfilework.h"
#include "qsharehistorycounterwork.h"
#include "dbservices/dbservices.h"
#include <QDebug>
#include "qhkexchangevoldataprocess.h"
#include "data_structure/hqutils.h"
#include "utils/profiles.h"
#include "dbservices/qactivedate.h"
#include "utils/comdatadefines.h"

#define     SAVE_DIR                QDir::currentPath() + "/data/"
#define     UPDATE_SEC              "UPDATE"
#define     UPDATE_DATE             "DATE"
QShareHistoryInfoMgr::QShareHistoryInfoMgr(const QStringList& codes, QObject *parent) : QObject(parent)
{
    //设定初始化的日线更新时期
    PROFILES_INS ->setDefault(UPDATE_SEC, UPDATE_DATE, QActiveDateTime(QDate::currentDate().addDays(-365)).toString(DATE_FORMAT));
    mCodesList = codes;
    mPool.setExpiryTimeout(-1);
    mPool.setMaxThreadCount(8);
    connect(this, SIGNAL(signalStartGetHistory()), this, SLOT(slotStartGetHistory()));
    connect(this, SIGNAL(signalUpdateAllShareFromDate(bool,QDate)), this, SLOT(slotUpdateAllShareFromDate(bool,QDate)));
    connect(DATA_SERVICE, SIGNAL(signalSendShareHistoryUpdateDateList(QList<QDate>)),
            this, SLOT(slotRecvShareHistoryUpdateList(QList<QDate>)));
//    connect(DATA_SERVICE, SIGNAL(signalUpdateHistoryInfoFinished()), this, SLOT(slotDbUpdateHistoryFinished()));
    //connect(DATA_SERVICE, SIGNAL(signalUpdateShareHistoryFinished(QString)), this, SLOT(slotUpdateShareHistoryInfoFinished(QString)));
    this->moveToThread(&mWorkThread);
    mWorkThread.start();
    //查询当前需要补足的日期时间


}

QShareHistoryInfoMgr::~QShareHistoryInfoMgr()
{
    mPool.clear();
}


void QShareHistoryInfoMgr::slotRecvShareHistoryUpdateList(const QList<QDate> &list)
{
    mTotalDateList = list;
//    QHKExchangeVolDataProcess * process = new QHKExchangeVolDataProcess(QDate(2019,6,18), QDate(2019,6,18), this);
//    mPool.start(process);
//    mPool.waitForDone();
//    return;
    qDebug()<<"date list:"<<list;
    QTime t;
    t.start();    
    //开始更新日线数据
    for(int i=0; i<list.size(); )
    {
        ShareDate start(list[i]);
        ShareDate end = start;
        for(int k = i+1; k<list.size(); k++)
        {
            ShareDate temp(list[k]);
            i = k;
            if(temp == end.nextActiveDay())
            {
                end = temp;
            } else
            {
                break;
            }
        }

        foreach (QString code, mCodesList) {
            QShareHistoryInfoThread* thread = new QShareHistoryInfoThread(code, start,  end, this);
            mPool.start(thread);
        }
        mPool.waitForDone();

        if(i == list.size() - 1) break;
    }
    qDebug()<<tr("更新日线数据耗时:")<<t.elapsed();    
    //更新外资持股数据，然后传递给个股日线合成
    QList<QDate> dateList = mShareInfoHistoryMap.keys();
    //更新节假日数据
    foreach (QDate wk, dateList) {
        mTotalDateList.removeOne(wk);
    }
    if(mTotalDateList.size() > 0)
    {
        emit DATA_SERVICE->signalUpdateShareCloseDate(mTotalDateList);
    }
    DATA_SERVICE->setHistoryInfoCount(dateList.size());
    bool update = false;
    QDate start, end;
    if(dateList.size() > 0)
    {
        start = dateList.first();
        end = dateList.last();
        qDebug()<<"record:"<<start<<end;
        int day_count = start.daysTo(end);
        //分成10个线程处理
        int day_gap = day_count / 50;
        if(day_gap == 0) day_gap = 1;
        while(start <= end)
        {
            QDate wkend = start.addDays(day_gap);
            if(wkend > end)
            {
                wkend = end;
            }
            QHKExchangeVolDataProcess * process = new QHKExchangeVolDataProcess(start, wkend, this);
            mPool.start(process);
            start = wkend.addDays(1);
        }
        mPool.waitForDone();
        qDebug()<<tr("更新外资数据耗时:")<<t.elapsed();
        update = true;
        //开始更新到数据库
        int mode = History_Close | History_HsgtVol;
        QList<QDate> dateList = mShareInfoHistoryMap.keys();
        foreach (QDate date, dateList) {
            ShareDataList list = mShareInfoHistoryMap[date].values();
            emit DATA_SERVICE->signalRecvShareHistoryInfos(list, mode);
            mShareInfoHistoryMap.remove(date);
        }
    } else
    {
        end = end.addDays(-1);
        qDebug()<<"no data end:"<<end;
    }
    ShareDate update_date = ShareDate::currentDate();
    update_date = update_date.previousActiveDay();
    //update db base time
    emit DATA_SERVICE->signalSendShareHistoryUpdateDate(update_date, update);
    //开始进行数据统计
    //emit signalUpdateHistoryMsg(QStringLiteral("开始进行日线数据统计"));
    qDebug()<<tr("开始进行日线数据统计");
    //    mCountCodeNum = mCodesList.count();
    //    mCurCnt = 0;
    //    foreach(QString code, mCodesList)
    //    {
    //        QShareHistoryCounterWork * process = new QShareHistoryCounterWork(code,ShareDataList(), 0);
    //        mPool.start(process);
    //    }
    //    mPool.waitForDone();

}


void QShareHistoryInfoMgr::slotStartGetHistory()
{
    //获取日线数据的更新日期
    DATA_SERVICE->signalQueryShareHistoryUpdateDateList();
//    ShareDate last_update_date = DATA_SERVICE->getLastUpdateDateOfHistoryInfo();
//    QDate update_date;
//    if(last_update_date.isNull())
//    {
//        //还未更新过
//        update_date = ShareDate::latestActiveDay().date().addYears(-1);
//    } else if(last_update_date < ShareDate::latestActiveDay())
//    {
//        update_date = last_update_date.date().addDays(1);
//    } else {
//        update_date = last_update_date.date();
//    }

//    slotUpdateAllShareFromDate(false, update_date);
}

void QShareHistoryInfoMgr::slotUpdateForignVolInfo(const ShareDataList& list)
{
    qDebug()<<__FUNCTION__<<list.size();
    QMutexLocker locker(&mShareInfoMutex);
    foreach (ShareData data, list)
    {
        QDate date = data.mTime.date();
        if(mShareInfoHistoryMap.contains(date))
        {
            QMap<QString, ShareData> &dateData = mShareInfoHistoryMap[date];
            if(dateData.contains(data.mCode))
            {
                ShareData& wkdata =  dateData[data.mCode];
                wkdata.mHsgtData.mVolTotal = data.mHsgtData.mVolTotal;
                wkdata.mHsgtData.mVolMutablePercent = data.mHsgtData.mVolMutablePercent;
                wkdata.mHsgtData.mVolChange = data.mHsgtData.mVolChange;
            }
        }
    }

}

void QShareHistoryInfoMgr::slotUpdateShareHistoryProcess(const ShareDataList& list)
{
    QMutexLocker locker(&mShareHistoryMutex);
    //将日线数据更新
    int i = 0;
    foreach (ShareData data, list) {
        //if(i== 0) qDebug()<<"更新日线数据:"<<data.mCode<<data.mName<<list.size();
        data.mHsgtData.mVolTotal = 0;
        data.mHsgtData.mVolMutablePercent = 0;
        i++;
        if(mShareInfoHistoryMap.contains(data.mTime.date()))
        {
            QMap<QString, ShareData> &dateData = mShareInfoHistoryMap[data.mTime.date()];
            if(dateData.contains(data.mCode))
            {
                ShareData& wkdata =  dateData[data.mCode];
                wkdata.mCode = data.mCode;
                wkdata.mTime = data.mTime;
                wkdata.mClose = data.mClose;
                wkdata.mChg = data.mChg;
                wkdata.mChgPercent = data.mChgPercent;
                wkdata.mName = data.mName;
                wkdata.mHsl = data.mHsl;
                wkdata.mVol = data.mVol;
                wkdata.mFinanceData.mTotalShare = data.mFinanceData.mTotalShare;
                wkdata.mFinanceData.mMutalShare = data.mFinanceData.mMutalShare;
                wkdata.mMoney = data.mMoney;
            } else
            {
                dateData[data.mCode] = data;
            }
        } else
        {
            QMap<QString, ShareData> dateData;
            dateData[data.mCode] = data;
            mShareInfoHistoryMap[data.mTime.date()] = dateData;
        }
    }
}

void QShareHistoryInfoMgr::slotUpdateAllShareFromDate(bool deldb, const QDate& date)
{
    QDate start = date;
    QDate end = ShareDate::latestActiveDay().date();
    QTime t;
    t.start();
    //开始更新日线数据
    foreach (QString code, mCodesList) {
        QShareHistoryInfoThread* thread = new QShareHistoryInfoThread(code, start,  end, this);
        mPool.start(thread);
    }
    mPool.waitForDone();
    qDebug()<<tr("更新日线数据耗时:")<<t.elapsed()<<start<<end;
    //更新外资持股数据，然后传递给个股日线合成
    QList<QDate> dateList = mShareInfoHistoryMap.keys();
    DATA_SERVICE->setHistoryInfoCount(dateList.size());
    bool update = false;
    if(dateList.size() > 0)
    {
        start = dateList.first();
        end = dateList.last();
        qDebug()<<"record:"<<start<<end;
        int day_count = start.daysTo(end);
        //分成10个线程处理
        int day_gap = day_count / 50;
        if(day_gap == 0) day_gap = 1;
        while(start <= end)
        {
            QDate wkend = start.addDays(day_gap);
            if(wkend > end)
            {
                wkend = end;
            }
            QHKExchangeVolDataProcess * process = new QHKExchangeVolDataProcess(start, wkend, this);
            mPool.start(process);
            start = wkend.addDays(1);
        }
        mPool.waitForDone();
        qDebug()<<tr("更新外资数据耗时:")<<t.elapsed();
        update = true;
    } else
    {
        end = end.addDays(-1);        
        qDebug()<<"no data end:"<<end;
    }
    //history data check

    //update db base time
    emit DATA_SERVICE->signalSendShareHistoryUpdateDate(ShareDate(end), update);
    //开始进行数据统计
    //emit signalUpdateHistoryMsg(QStringLiteral("开始进行日线数据统计"));
    qDebug()<<tr("开始进行日线数据统计");
//    mCountCodeNum = mCodesList.count();
//    mCurCnt = 0;
//    foreach(QString code, mCodesList)
//    {
//        QShareHistoryCounterWork * process = new QShareHistoryCounterWork(code,ShareDataList(), 0);
//        mPool.start(process);
//    }
//    mPool.waitForDone();

}

void QShareHistoryInfoMgr::slotUpdateReadHistoryInfo(const ShareDataList &list)
{
//    foreach (ShareData data, list) {
//        ShareDataList &wklist = mShareInfoHistoryMap[data.mCode];
//        wklist.append(data);
//    }
//    mCurCnt++;
//    emit signalUpdateHistoryMsg(QStringLiteral("读入日线文件：%1/%2").arg(mCurCnt).arg(mHistoryFileNum));
}

void QShareHistoryInfoMgr::slotUpdateWriteHistoryInfo(const ShareDataList &list)
{
    mCurCnt++;
    emit signalUpdateHistoryMsg(QStringLiteral("写入日线文件：%1/%2").arg(mCurCnt).arg(mShareInfoMap.size()));
}

void QShareHistoryInfoMgr::slotDbUpdateHistoryFinished()
{
//    QStringList keys(mShareInfoMap.keys());
//    foreach (QString key, keys) {
//        emit DATA_SERVICE->signalUpdateShareinfoWithHistory(key);
//    }
//    Profiles::instance()->setValue("UPDATE", "DATE", HqUtils::date2Str(HqUtils::latestActiveDay().addDays(-1)));
}

void QShareHistoryInfoMgr::slotUpdateShareHistoryInfoFinished(const QString &code)
{
    mCurCnt++;
    emit signalUpdateHistoryMsg(QString("%1:%2/%3").\
                                arg(QStringLiteral("统计历史信息")).\
                                arg(mCurCnt).\
                                arg(mCountCodeNum));


    if(mCurCnt == mCountCodeNum)
    {
        emit signalUpdateHistoryFinished();
    }
}
