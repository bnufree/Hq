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
//    connect(DATA_SERVICE, SIGNAL(signalUpdateHistoryInfoFinished()), this, SLOT(slotDbUpdateHistoryFinished()));
    //connect(DATA_SERVICE, SIGNAL(signalUpdateShareHistoryFinished(QString)), this, SLOT(slotUpdateShareHistoryInfoFinished(QString)));
    this->moveToThread(&mWorkThread);
    mWorkThread.start();

}

QShareHistoryInfoMgr::~QShareHistoryInfoMgr()
{
    mPool.clear();
}


void QShareHistoryInfoMgr::slotStartGetHistory()
{
    //获取日线数据的更新日期
    ShareDate last_update_date = DATA_SERVICE->getLastUpdateDateOfHistoryInfo();
    QDate update_date;
    if(last_update_date.isNull())
    {
        //还未更新过
        update_date = ShareDate::latestActiveDay().date().addYears(-1);
    } else if(last_update_date < ShareDate::latestActiveDay())
    {
        update_date = last_update_date.date().addDays(1);
    } else {
        update_date = last_update_date.date();
    }

    slotUpdateAllShareFromDate(false, update_date);
}

void QShareHistoryInfoMgr::slotUpdateForignVolInfo(const ShareDataList& list)
{
    QMutexLocker locker(&mShareInfoMutex);
    foreach (ShareData data, list) {
        ShareData& wkdata =  mShareInfoHistoryMap[data.keyOfCodeTime()];
        wkdata.mCode = data.mCode;
        wkdata.mTime = data.mTime;
        wkdata.mHsgtData.mVol = data.mHsgtData.mVol;
    }
//    emit signalUpdateHistoryMsg(QString("%1:%2/%3").\
//                                arg(QStringLiteral("外资持股数据更新完成")).\
//                                arg(date.toString(DATE_FORMAT)).\
//                                arg(mDates.size() < 10 ? mDates.join(","):""));
}

void QShareHistoryInfoMgr::slotUpdateShareHistoryProcess(const ShareDataList& list)
{
    QMutexLocker locker(&mShareHistoryMutex);
    //将日线数据更新
    int i = 0;
    foreach (ShareData data, list) {
        //if(i== 0) qDebug()<<"更新日线数据:"<<data.mCode<<data.mName<<list.size();
        i++;
        ShareData& wkdata =  mShareInfoHistoryMap[data.keyOfCodeTime()];
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
    }
}

void QShareHistoryInfoMgr::slotUpdateAllShareFromDate(bool deldb, const QDate& date)
{
    qDebug()<<__func__<<date;
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
    qDebug()<<tr("更新日线数据耗时:")<<t.elapsed()<<date<<ShareDate::latestActiveDay().date();
    //更新外资持股数据，然后传递给个股日线合成
    t.start();
    int day_count = start.daysTo(end);
    //分成10个线程处理
    int day_gap = day_count / 10;
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

    //数据更新完成,开始更新到数据库
    int mode = Share_History_Mode::History_Close | Share_History_Mode::History_HsgtVol;
    emit DATA_SERVICE->signalRecvShareHistoryInfos(mShareInfoHistoryMap.values(), mode);
    //等到数据更新完成,开始进行数据的整理和统计
    QEventLoop subloop;
    connect(DATA_SERVICE, SIGNAL(signalUpdateHistoryInfoFinished()), &subloop, SLOT(quit()));
    subloop.exec();
    //开始进行数据统计
    //emit signalUpdateHistoryMsg(QStringLiteral("开始进行日线数据统计"));
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
