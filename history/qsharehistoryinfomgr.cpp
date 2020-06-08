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
    mPool.setMaxThreadCount(2);
#ifdef Q_OS_WIN
    mPool.setMaxThreadCount(8);
#endif
    connect(this, SIGNAL(signalStartGetHistory()), this, SLOT(slotStartGetHistoryWithAllCodes()));
    connect(this, SIGNAL(signalStartStatic()), this, SLOT(slotStartStatics()));
    this->moveToThread(&mWorkThread);
    mWorkThread.start();
    //查询当前需要补足的日期时间


}

QShareHistoryInfoMgr::~QShareHistoryInfoMgr()
{
    mPool.clear();
}

void QShareHistoryInfoMgr::slotStartGetHistoryWithAllCodes()
{
    PROFILES_INS ->setDefault(UPDATE_SEC, UPDATE_DATE, QActiveDateTime(QDate::currentDate().addDays(-365)).toString(DATE_FORMAT));
    qDebug()<<"start get hk"<<QThread::currentThread()<<((QThread*)&mWorkThread);
    //首先获取从一年前到现在的陆股通数据
    QList<QDate> historyDatesList = ShareWorkingDate::getHisWorkingDay();
    foreach (QDate start, historyDatesList) {
        if(start == QDate::currentDate()) continue;
        QHKExchangeVolDataProcess * process = new QHKExchangeVolDataProcess(start, QHKExchangeVolDataProcess::Fetch_All, this);
        mPool.start(process);
    }
    mPool.waitForDone();
    if(mCodesList.size() == 0) return;
    QDir dir(HQ_DAY_HISTORY_DIR);
    if(!dir.exists()) dir.mkpath(HQ_DAY_HISTORY_DIR);
    QTime t;
    t.start();
    qDebug()<<"start get history";
    //陆股通数据同步更新
    mCodesList.clear();
    mCodesList.append("300521");
    foreach (QString code, mCodesList) {
        if(code.size() > 6) code = code.right(6);
        QShareHistoryInfoThread* thread = new QShareHistoryInfoThread(code, &mShareForeignDataMap, this);
        mPool.start(thread);
    }
    mPool.waitForDone();

    qDebug()<<"start counter"<<t.elapsed();
    slotStartStatics();
}

void QShareHistoryInfoMgr::slotStartStatics()
{
    foreach (QString code, mCodesList) {
        if(code.size() > 6) code = code.right(6);
//        if(code.left(1) == "1" || code.left(1) == "5") continue;
        if(!mShareInfoHistoryMap.contains(code)) continue;
        ShareHistoryFileDataList list = mShareInfoHistoryMap[code];        
        QShareHistoryCounterWork* thread = new QShareHistoryCounterWork(code, list, this);
        mPool.start(thread);
    }
    mPool.waitForDone();
}


void QShareHistoryInfoMgr::slotUpdateForignVolInfo(const ShareForignVolFileDataList& list, const QDate& date)
{
    if(list.size() == 0) return;
    mShareForeignDataMap[date] = list;
}

void QShareHistoryInfoMgr::slotUpdateShareHistoryProcess(const ShareHistoryFileDataList& list, const QString& code)
{
    QMutexLocker locker(&mShareHistoryMutex);
    mShareInfoHistoryMap[code] = list;    
}

