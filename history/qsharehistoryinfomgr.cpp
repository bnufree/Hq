#include "qsharehistoryinfomgr.h"
#include "qeastmoneystockhistoryinfothread.h"
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
    connect(DATA_SERVICE, SIGNAL(signalUpdateShareHistoryFinished(QString)), this, SLOT(slotUpdateShareHistoryInfoFinished(QString)));
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
    qDebug()<<__FUNCTION__<<__LINE__<<list.size()<<date;
    QMutexLocker locker(&mShareInfoMutex);
    foreach (ShareData data, list) {
        ShareDataList &wklist = mShareInfoMap[data.mCode];
        wklist.append(data);
    }
}

void QShareHistoryInfoMgr::slotUpdateShareHistoryProcess(const QString &code)
{
    QMutexLocker locker(&mShareHistoryMutex);
    emit signalUpdateHistoryMsg(QString("%1:%2/%3").\
                                arg(QStringLiteral("更新日线数据")).\
                                arg(++mCurCnt).\
                                arg(mCodesList.size()));
    //qDebug()<<"thread count:"<<mPool.activeThreadCount();
}

void QShareHistoryInfoMgr::slotUpdateAllShareFromDate(bool deldb, const QDate& date)
{
    emit signalUpdateHistoryMsg(QStringLiteral("开始更新外资持股数据..."));
    QDate wkDate = date;
    while(wkDate < HqUtils::latestActiveDay())
    {
        if(HqUtils::activeDay(wkDate))
        {
            QHKExchangeVolDataProcess * process = new QHKExchangeVolDataProcess(wkDate, this);
            mPool.start(process);
        }
        wkDate = wkDate.addDays(1);
    }
    mPool.waitForDone();

    emit signalUpdateHistoryMsg(QStringLiteral("开始更新日线数据..."));
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
    foreach (QString code, mCodesList) {
        ShareDataList *list = (ShareDataList*)(&mShareInfoMap[code.right(6)]);
        QEastmoneyShareHistoryInfoThread* thread = new QEastmoneyShareHistoryInfoThread(code, date, SAVE_DIR, deldb, list, this);
        mPool.start(thread);
    }
    mPool.waitForDone();
    emit signalUpdateHistoryMsg(QStringLiteral("开始将日线数据写入到数据库..."));
    ShareDataList list;
    foreach (QString key, mShareInfoMap.keys()) {
        list.append(mShareInfoMap.value(key));
    }
    DATA_SERVICE->signalRecvAllShareHistoryInfos(list, deldb);

}

void QShareHistoryInfoMgr::slotDbUpdateHistoryFinished()
{
    QStringList keys(mShareInfoMap.keys());
    foreach (QString key, keys) {
        emit DATA_SERVICE->signalUpdateShareinfoWithHistory(key);
    }
    Profiles::instance()->setValue("UPDATE", "DATE", HqUtils::date2Str(HqUtils::latestActiveDay().addDays(-1)));
}

void QShareHistoryInfoMgr::slotUpdateShareHistoryInfoFinished(const QString &code)
{
    static int num = 0;
    num++;
    emit signalUpdateHistoryMsg(QString("%1:%2/%3").\
                                arg(QStringLiteral("更新开始信息")).\
                                arg(num).\
                                arg(mShareInfoMap.count()));


    if(num == mShareInfoMap.count())
    {
        emit signalUpdateHistoryFinished();
    }
}
