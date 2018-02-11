#include "qsharehistoryinfomgr.h"
#include "qeastmoneystockhistoryinfothread.h"
#include "dbservices/dbservices.h"
#include "qsinasharevolinfothread.h"
#include "../qeastmoneyhsgtshareamount.h"
#include <QDebug>
#include "qhkexchangevoldataprocess.h"
#include "utils/hqutils.h"

QShareHistoryInfoMgr::QShareHistoryInfoMgr(const QStringList& codes, QObject *parent) : QObject(parent)
{
    mCodesList = codes;
    mPool.setExpiryTimeout(-1);
    mPool.setMaxThreadCount(8);
    connect(this, SIGNAL(signalGetFianceInfo()), this, SLOT(slotGetFinanceInfo()));
    connect(this, SIGNAL(signalUpdateAllShareFrom20170317()), this, SLOT(slotUpdateAllShareFrom20170317()));
    this->moveToThread(&mWorkThread);
    mWorkThread.start();
}

QShareHistoryInfoMgr::~QShareHistoryInfoMgr()
{
    mPool.clear();
}

void QShareHistoryInfoMgr::slotGetFinanceInfo()
{
    emit signalUpdateHistoryMsg(QStringLiteral("正在更新财务信息..."));
    QSinaShareVolInfoThread * vols = new QSinaShareVolInfoThread(mCodesList);
    connect(vols, SIGNAL(finished()), this, SLOT(slotShareFinanceInfoFinished()));
    vols->start();
}

void QShareHistoryInfoMgr::slotShareFinanceInfoFinished()
{
    emit signalUpdateHistoryMsg(QStringLiteral("完成更新财务信息"));
    QSinaShareVolInfoThread *t1 = qobject_cast<QSinaShareVolInfoThread*>(sender());
    if(t1)
    {
        delete t1;
    }
    qDebug()<<QDateTime::currentDateTime();
    //开始更新日线数据
    emit signalUpdateHistoryMsg(QStringLiteral("开始更新日线数据..."));
    mCurCnt = 0;
    foreach (QString code, mCodesList) {
        QEastmoneyStockHistoryInfoThread* thread = new QEastmoneyStockHistoryInfoThread(code, StockDataList(),false, this);
        mPool.start(thread);
    }
    mPool.waitForDone();
     qDebug()<<QDateTime::currentDateTime();
     emit signalUpdateHistoryMsg("");
    emit signalHistoryDataFinished();
}

void QShareHistoryInfoMgr::slotUpdateForignVolInfo(const StockDataList &list, const QDate& date)
{
    qDebug()<<__FUNCTION__<<__LINE__<<list.size()<<date;
    QMutexLocker locker(&mShareInfoMutex);
    foreach (StockData data, list) {
        StockDataList &wklist = mShareInfoMap[data.mCode];
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

void QShareHistoryInfoMgr::slotUpdateAllShareFrom20170317()
{
    //取得自20170317的数据
    QDate date(2017,3,17);
    emit signalUpdateHistoryMsg(QStringLiteral("开始更新外资持股数据..."));
    while(date < QDate::currentDate())
    {
        if(!HqUtils::isWeekend(date))
        {
            QHKExchangeVolDataProcess * process = new QHKExchangeVolDataProcess(date, this);
            mPool.start(process);
        }
        date = date.addDays(1);
    }
    mPool.waitForDone();

    emit signalUpdateHistoryMsg(QStringLiteral("开始更新日线数据..."));
    mCurCnt = 0;
    //开始更新日线数据
    foreach (QString code, mCodesList) {
        StockDataList list = mShareInfoMap[code.right(6)];
        QEastmoneyStockHistoryInfoThread* thread = new QEastmoneyStockHistoryInfoThread(code, list, true, this, QDate(2017,03,17));
        mPool.start(thread);
    }
    mPool.waitForDone();
     qDebug()<<QDateTime::currentDateTime();
     emit signalUpdateHistoryMsg(QStringLiteral(""));
}
