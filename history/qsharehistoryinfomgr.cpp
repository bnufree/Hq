#include "qsharehistoryinfomgr.h"
#include "qsharehistoryinfothread.h"
#include "qsharehistoryfilework.h"
#include "qsharehistorycounterwork.h"
#include "dbservices/dbservices.h"
#include "../qeastmoneyhsgtshareamount.h"
#include <QDebug>
#include "qhkexchangevoldataprocess.h"
#include "utils/hqutils.h"
#include "utils/profiles.h"

#define     SAVE_DIR    QDir::currentPath() + "/data/"
QShareHistoryInfoMgr::QShareHistoryInfoMgr(const QStringList& codes, QObject *parent) : QObject(parent)
{
    //设定初始化的日线更新时期
    Profiles::instance()->setDefault("UPDATE", "DATE", HqUtils::date2Str(QDate(2017,3,16)));
    mCodesList = codes;
    mPool.setExpiryTimeout(-1);
    mPool.setMaxThreadCount(8);
    connect(this, SIGNAL(signalStartGetHistory()), this, SLOT(slotStartGetHistory()));
    connect(this, SIGNAL(signalUpdateAllShareFromDate(bool,QDate)), this, SLOT(slotUpdateAllShareFromDate(bool,QDate)));
    connect(DATA_SERVICE, SIGNAL(signalUpdateHistoryInfoFinished()), this, SLOT(slotDbUpdateHistoryFinished()));
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
    //开始更新日线数据，取得上次日线数据的日期
    QDate lastDate = HqUtils::dateFromStr(Profiles::instance()->value("UPDATE", "DATE").toString());
    QDate updateDate = lastDate.addDays(1);
    slotUpdateAllShareFromDate(false,updateDate);
}

void QShareHistoryInfoMgr::slotUpdateForignVolInfo(const ShareDataList &list, const QDate& date)
{
    QMutexLocker locker(&mShareInfoMutex);
    foreach (ShareData data, list) {
        ShareDataList &wklist = mShareInfoMap[HqUtils::date2Str(date)];
        ShareData &wkdata = wklist.valueOfDate(date, data.mCode);
        wkdata.mForeignVol = data.mForeignVol;
    }
    mDates.removeAll(HqUtils::date2Str(date));
    emit signalUpdateHistoryMsg(QString("%1:%2/%3").\
                                arg(QStringLiteral("外资持股数据更新完成")).\
                                arg(HqUtils::date2Str(date)).\
                                arg(mDates.size() < 10 ? mDates.join(","):""));
}

void QShareHistoryInfoMgr::slotUpdateShareHistoryProcess(const ShareDataList& list)
{
    QMutexLocker locker(&mShareHistoryMutex);
    //将历史数据更新到map
    foreach (ShareData data, list) {
        QDate date = QDateTime::fromMSecsSinceEpoch(data.mTime).date();
        ShareDataList &wklist = mShareInfoMap[HqUtils::date2Str(date)];
        wklist.append(data);

    }
    emit signalUpdateHistoryMsg(QString("%1:%2/%3").\
                                arg(QStringLiteral("更新日线数据")).\
                                arg(++mCurCnt).\
                                arg(mCodesList.size()));
}

void QShareHistoryInfoMgr::slotUpdateAllShareFromDate(bool deldb, const QDate& date)
{

    //创建文件保存的目录
    QDir wkdir(SAVE_DIR);
    if(!wkdir.exists())
    {
        if(wkdir.mkpath(SAVE_DIR))
        {
            qDebug()<<"make path "<<SAVE_DIR<<" ok.";
        } else
        {
            qDebug()<<"make path "<<SAVE_DIR<<" falied.";
        }

    }
    mCurCnt = 0;
    //开始更新日线数据
    emit signalUpdateHistoryMsg(QStringLiteral("开始更新日线数据..."));
    foreach (QString code, mCodesList) {
        QShareHistoryInfoThread* thread = new QShareHistoryInfoThread(code, date, this);
        mPool.start(thread);
    }
    mPool.waitForDone();

    emit signalUpdateHistoryMsg(QStringLiteral("开始更新外资持股数据..."));
    QDate wkDate = date;
    while(wkDate < HqUtils::latestActiveDay())
    {
        if(HqUtils::activeDay(wkDate))
        {
            mDates.append(HqUtils::date2Str(wkDate));
            QHKExchangeVolDataProcess * process = new QHKExchangeVolDataProcess(wkDate, this);
            mPool.start(process);
        }
        wkDate = wkDate.addDays(1);
    }
    mPool.waitForDone();
    emit signalUpdateHistoryMsg(QStringLiteral("开始将日线数据写入文件"));

    wkDate = date;
    mCurCnt = 0;
    //qDebug()<<"write info total size:"<<mShareInfoMap.size();
    while(wkDate < HqUtils::latestActiveDay())
    {
        QString key = HqUtils::date2Str(wkDate);
        if(mShareInfoMap.contains(key))
        {
            QString fileName = QString("%1%2.dat").arg(SAVE_DIR).arg(wkDate.toString("yyyyMMdd"));
            ShareDataList list = mShareInfoMap[key];
            if(list.size() > 0)
            {
                QShareHistoryFileWork * process = new QShareHistoryFileWork(FILE_WRITE, fileName, list, this);
                mPool.start(process);
            }
        }
        wkDate = wkDate.addDays(1);
    }
    mPool.waitForDone();
    mShareInfoMap.clear();

    Profiles::instance()->setValue("UPDATE", "DATE", HqUtils::date2Str(HqUtils::latestActiveDay().addDays(-1)));
    emit signalUpdateHistoryMsg(QStringLiteral("开始读入日线数据"));
    mCurCnt = 0;
    wkDate = HqUtils::getActiveDayBefore1HYear();
    mHistoryFileNum = HqUtils::activeDaysNum(wkDate);
    while(wkDate < HqUtils::latestActiveDay())
    {
        if(HqUtils::activeDay(wkDate))
        {
            QString fileName = QString("%1%2.dat").arg(SAVE_DIR).arg(wkDate.toString("yyyyMMdd"));
            QShareHistoryFileWork * process = new QShareHistoryFileWork(FILE_READ, fileName, ShareDataList(), this);
            mPool.start(process);
        }
        wkDate = wkDate.addDays(1);
    }
    mPool.waitForDone();

    emit signalUpdateHistoryMsg(QStringLiteral("开始进行日线数据统计"));
    mCountCodeNum = mShareInfoHistoryMap.count();
    mCurCnt = 0;
    foreach(QString code, mShareInfoHistoryMap.keys())
    {
        //qDebug()<<"code:"<<code<<mShareInfoHistoryMap[code].size();
        QShareHistoryCounterWork * process = new QShareHistoryCounterWork(code, mShareInfoHistoryMap[code], this);
        mPool.start(process);
    }
    mPool.waitForDone();

}

void QShareHistoryInfoMgr::slotUpdateReadHistoryInfo(const ShareDataList &list)
{
    foreach (ShareData data, list) {
        ShareDataList &wklist = mShareInfoHistoryMap[data.mCode];
        wklist.append(data);
    }
    mCurCnt++;
    emit signalUpdateHistoryMsg(QStringLiteral("读入日线文件：%1/%2").arg(mCurCnt).arg(mHistoryFileNum));
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
