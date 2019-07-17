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
    connect(this, SIGNAL(signalStartGetHistory()), this, SLOT(slotStartGetHistoryWithAllCodes()));
    connect(this, SIGNAL(signalStartStatic()), this, SLOT(slotStartStatics()));
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

void QShareHistoryInfoMgr::slotStartGetHistoryWithAllCodes()
{
    //首先获取从一年前到现在的陆股通数据
    qDebug()<<"history date size:"<<ShareDate::getHisWorkingDay().size();
    QDate start = ShareDate::getHisWorkingDay().last();
    while (start < ShareDate::currentDate().date()) {
        if(ShareDate::getHisWorkingDay().contains(start))
        {
            QHKExchangeVolDataProcess * process = new QHKExchangeVolDataProcess(start, this);
            mPool.start(process);
        }
        start = start.addDays(1);
    }
    mPool.waitForDone();
    if(mCodesList.size() == 0) return;
    //通过传入的陆股通数据更新日线文件信息
    foreach (QString code, mCodesList) {
        if(code.size() > 6) code = code.right(6);
        if(code.left(1) == "1" || code.left(1) == "5") continue;
        QShareHistoryInfoThread* thread = new QShareHistoryInfoThread(code, &mShareForeignDataMap, this);
        mPool.start(thread);
    }
    mPool.waitForDone();
    slotStartStatics();
}

void QShareHistoryInfoMgr::slotStartStatics()
{
    foreach (QString code, mCodesList) {
        if(code.size() > 6) code = code.right(6);
        if(code.left(1) == "1" || code.left(1) == "5") continue;
        if(!mShareInfoHistoryMap.contains(code)) continue;
        ShareHistoryFileDataList list = mShareInfoHistoryMap[code];
        QShareHistoryCounterWork* thread = new QShareHistoryCounterWork(code, list, this);
        mPool.start(thread);
    }
    mPool.waitForDone();
}


void QShareHistoryInfoMgr::slotUpdateForignVolInfo(const ShareForignVolFileDataList& list, const QDate& date)
{
    qDebug()<<__FUNCTION__<<list.size();
    if(list.size() == 0) return;
    mShareForeignDataMap[date] = list;
}

void QShareHistoryInfoMgr::slotUpdateShareHistoryProcess(const ShareHistoryFileDataList& list, const QString& code)
{
    QMutexLocker locker(&mShareHistoryMutex);
    mShareInfoHistoryMap[code] = list;
}

