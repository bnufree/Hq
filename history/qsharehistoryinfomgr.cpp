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
    //QDate lastDate = QDate::fromString(PROFILES_INS->value(UPDATE_SEC, UPDATE_DATE).toString(), DATE_FORMAT);
    //QDate updateDate = lastDate.addDays(1);
    slotUpdateAllShareFromDate(false,QActiveDateTime::latestActiveDay().addYears(-1));
}

void QShareHistoryInfoMgr::slotUpdateForignVolInfo(const QStringList& list, const QDate& date)
{
    QMutexLocker locker(&mShareInfoMutex);
    foreach (QString key, list) {
        QStringList wk = key.split("|");
        if(wk.size() >= 2)
        {
            int code = wk[0].toInt();
            qint64 val = wk[1].toLongLong();
            QMap<qint64, qint64> &wklist = mShareInfoMap[code];
            wklist[date.toJulianDay()] = val;
        }
    }
    qDebug()<<__func__<<date<<list.size()<<mDates.size();
    mDates.removeAll(date.toString(DATE_FORMAT));
//    emit signalUpdateHistoryMsg(QString("%1:%2/%3").\
//                                arg(QStringLiteral("外资持股数据更新完成")).\
//                                arg(date.toString(DATE_FORMAT)).\
//                                arg(mDates.size() < 10 ? mDates.join(","):""));
}

void QShareHistoryInfoMgr::slotUpdateShareHistoryProcess(const ShareDataList& list)
{
//    QMutexLocker locker(&mShareHistoryMutex);
//    //将历史数据更新到map
//    foreach (ShareData data, list) {
//        QDate date = QDateTime::fromMSecsSinceEpoch(data.mTime).date();
//        ShareDataList &wklist = mShareInfoMap[date.toString(DATE_FORMAT)];
//        wklist.append(data);

//    }
    emit signalUpdateHistoryMsg(QString("%1:%2/%3").\
                                arg(QStringLiteral("更新日线数据")).\
                                arg(++mCurCnt).\
                                arg(mCodesList.size()));
}

void QShareHistoryInfoMgr::slotUpdateAllShareFromDate(bool deldb, const QDate& date)
{
    qDebug()<<__func__<<date;
    //创建文件保存的目录
    HqUtils::makePath(HQ_SHARE_HISTORY_DIR);
    HqUtils::makePath(HQ_LGTHISTORY_DIR);
    mCurCnt = 0;
    emit signalUpdateHistoryMsg(QStringLiteral("开始更新外资持股数据..."));
    //更新外资持股数据，然后传递给个股日线合成
    QDate wkDate = date;
    qDebug()<<__func__<<wkDate<<QActiveDateTime::latestActiveDay();
    while(wkDate < QActiveDateTime::latestActiveDay())
    {
        if(QActiveDateTime(wkDate).isDayActive())
        {
            mDates.append(wkDate.toString(DATE_FORMAT));
            QHKExchangeVolDataProcess * process = new QHKExchangeVolDataProcess(wkDate, this);
            mPool.start(process);
            break;
        }
        wkDate = QActiveDateTime(wkDate).nextActiveDay();
    }
    mPool.waitForDone();

    //开始更新日线数据
    emit signalUpdateHistoryMsg(QStringLiteral("开始更新日线数据..."));
    foreach (QString code, mCodesList) {
        //取得个股对应的外资持股数据
        QMap<qint64, qint64> foreign_Map = mShareInfoMap[code.right(6).toInt()];
        QShareHistoryInfoThread* thread = new QShareHistoryInfoThread(code, foreign_Map, 0);
        mPool.start(thread);
    }
    mPool.waitForDone();

#if 0
    emit signalUpdateHistoryMsg(QStringLiteral("开始将日线数据写入文件"));

    wkDate = date;
    mCurCnt = 0;
    //qDebug()<<"write info total size:"<<mShareInfoMap.size();
    while(wkDate < QActiveDateTime::latestActiveDay())
    {
        QString key = wkDate.toString(DATE_FORMAT);
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

    PROFILES_INS->setValue(UPDATE_SEC, UPDATE_DATE, QActiveDateTime::latestActiveDay().toString(DATE_FORMAT));
    emit signalUpdateHistoryMsg(QStringLiteral("开始读入日线数据"));
    mCurCnt = 0;
    wkDate = QActiveDateTime::getActiveDayBefore1HYear();
    mHistoryFileNum = QActiveDateTime::activeDaysNum(wkDate);
    while(wkDate < QActiveDateTime::latestActiveDay())
    {
        if(QActiveDateTime(wkDate).isDayActive())
        {
            QString fileName = QString("%1%2.dat").arg(SAVE_DIR).arg(wkDate.toString("yyyyMMdd"));
            QShareHistoryFileWork * process = new QShareHistoryFileWork(FILE_READ, fileName, ShareDataList(), this);
            mPool.start(process);
        }
        wkDate = wkDate.addDays(1);
    }
    mPool.waitForDone();
#endif
    emit signalUpdateHistoryMsg(QStringLiteral("开始进行日线数据统计"));
    mCountCodeNum = mCodesList.count();
    mCurCnt = 0;
    foreach(QString code, mCodesList)
    {
        QShareHistoryCounterWork * process = new QShareHistoryCounterWork(code,ShareDataList(), 0);
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
