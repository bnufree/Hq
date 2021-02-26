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
QShareHistoryInfoMgr::QShareHistoryInfoMgr(const QStringList& codes, QObject *parent) : QObject(parent)
{
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
    //首先获取从一年前到现在的陆股通数据
    QDate start = QDate::currentDate().addYears(-1);
    while (1) {
        if(start == QDate::currentDate()) break;
        if(TradeDateMgr::instance()->isTradeDay(start))
        {
            QHKExchangeVolDataProcess * process = new QHKExchangeVolDataProcess(start, QHKExchangeVolDataProcess::Fetch_All, this);
            process->setAutoDelete(true);
            mPool.start(process);
        }
        start = start.addDays(1);
    }
    mPool.waitForDone();
    if(mCodesList.size() == 0) return;
    QDir dir(HQ_DAY_HISTORY_DIR);
    if(!dir.exists()) dir.mkpath(HQ_DAY_HISTORY_DIR);
    //陆股通数据同步更新
    foreach (QString code, mCodesList) {
        if(code.size() > 6) code = code.right(6);
//        QMap<QDate, ShareForignVolFileData> list = mShareForeignDataMap[code.toInt()];
        bool counter = false;
        counter = true;
        QShareHistoryInfoThread* thread = new QShareHistoryInfoThread(code, QMap<QDate, ShareForignVolFileData>(), true, this);
        thread->setAutoDelete(true);
        mPool.start(thread);
    }
    mPool.waitForDone();

    emit DATA_SERVICE->signalUpdateShareCounter(mShareInfoHistoryMap.values());
}

void QShareHistoryInfoMgr::slotStartStatics()
{
//    foreach (QString code, mCodesList) {
//        if(code.size() > 6) code = code.right(6);
////        if(code.left(1) == "1" || code.left(1) == "5") continue;
//        if(!mShareInfoHistoryMap.contains(code)) continue;
//        ShareDailyDataList list = mShareInfoHistoryMap[code];
//        QShareHistoryCounterWork* thread = new QShareHistoryCounterWork(code, list, this);
//        mPool.start(thread);
//    }
//    mPool.waitForDone();
}


void QShareHistoryInfoMgr::slotUpdateForignVolInfo(const ShareForignVolFileDataList& list, const QDate& date)
{
    QMutexLocker locker(&mForeignHistoryMutex);
    if(list.size() == 0) return;
    foreach (ShareForignVolFileData data, list) {
        mShareForeignDataMap[data.mCode][date] = data;
    }

}

void QShareHistoryInfoMgr::slotUpdateShareHistoryProcess(const ShareHistoryCounter& list, const QString& code)
{
    QMutexLocker locker(&mShareHistoryMutex);
    mShareInfoHistoryMap[code] = list;    
}

