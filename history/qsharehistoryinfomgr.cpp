#include "qsharehistoryinfomgr.h"
#include "qeastmoneystockhistoryinfothread.h"
#include "dbservices/dbservices.h"
#include "qsinasharevolinfothread.h"
#include "../qeastmoneyhsgtshareamount.h"
#include <QDebug>
#include <QThreadPool>
#include "qhkexchangevoldataprocess.h"
#include "utils/hqutils.h"

QShareHistoryInfoMgr::QShareHistoryInfoMgr(const QStringList& codes, QObject *parent) : QObject(parent)
{
    mCodesList = codes;
    connect(this, SIGNAL(signalGetFianceInfo()), this, SLOT(slotGetFinanceInfo()));
    connect(this, SIGNAL(signalUpdateAllShareFrom20170317()), this, SLOT(slotUpdateAllShareFrom20170317()));
    this->moveToThread(&mWorkThread);
    mWorkThread.start();
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
    QThreadPool pool;
    pool.setExpiryTimeout(-1);
    pool.setMaxThreadCount(8);

    //开始更新日线数据
    emit signalUpdateHistoryMsg(QStringLiteral("开始更新日线数据..."));
    foreach (QString code, mCodesList) {
        QEastmoneyStockHistoryInfoThread* thread = new QEastmoneyStockHistoryInfoThread(code, StockDataList(),false, this);
        pool.start(thread);
    }
    pool.waitForDone();
     qDebug()<<QDateTime::currentDateTime();
     emit signalUpdateHistoryMsg("");
    //开始更新持股信息
//    QEastMoneyHSGTShareAmount * thread = new QEastMoneyHSGTShareAmount;
//    connect(thread, SIGNAL(finished()), this, SLOT(slotUpdateForeignAmountFinished()));
//    connect(thread, SIGNAL(signalAmountFinshedAtDate(QString)), this, SIGNAL(signalUpdateAmountProcess(QString)));
//    thread->start();
    emit signalHistoryDataFinished();
}

void QShareHistoryInfoMgr::slotUpdateForignVolInfo(const StockDataList &list, const QDate& date)
{
    qDebug()<<__FUNCTION__<<__LINE__<<list.size()<<date;
    foreach (StockData data, list) {
        StockDataList &wklist = mShareInfoMap[data.mCode];
        wklist.append(data);
    }
}

void QShareHistoryInfoMgr::slotUpdateAllShareFrom20170317()
{
    QThreadPool pool;
    pool.setExpiryTimeout(-1);
    pool.setMaxThreadCount(8);
    //取得自20170317的数据
    QDate date(2017,3,17);
    emit signalUpdateHistoryMsg(QStringLiteral("开始更新外资持股数据..."));
    while(date < QDate::currentDate())
    {
        if(!HqUtils::isWeekend(date))
        {
            QHKExchangeVolDataProcess * process = new QHKExchangeVolDataProcess(date, this);
            pool.start(process);
        }
        date = date.addDays(1);
    }
    pool.waitForDone();

    emit signalUpdateHistoryMsg(QStringLiteral("开始更新日线数据..."));
    //开始更新日线数据
    foreach (QString code, mCodesList) {
        QEastmoneyStockHistoryInfoThread* thread = new QEastmoneyStockHistoryInfoThread(code, mShareInfoMap[code], true, this, QDate(2017,03,17));
        pool.start(thread);
    }
    pool.waitForDone();
     qDebug()<<QDateTime::currentDateTime();
//    //开始更新持股信息
//    QEastMoneyHSGTShareAmount * thread = new QEastMoneyHSGTShareAmount;
//    connect(thread, SIGNAL(finished()), this, SLOT(slotUpdateForeignAmountFinished()));
//    connect(thread, SIGNAL(signalAmountFinshedAtDate(QString)), this, SIGNAL(signalUpdateAmountProcess(QString)));
//    thread->start();
//    emit signalHistoryDataFinished();
     emit signalUpdateHistoryMsg(QStringLiteral(""));
}
