#include "qsharehistoryinfomgr.h"
#include "qeastmoneystockhistoryinfothread.h"
#include "dbservices/dbservices.h"
#include "qsinasharevolinfothread.h"
#include "../qeastmoneyhsgtshareamount.h"
#include <QDebug>
#include <QThreadPool>
#include "qhkexchangevoldataprocess.h"

QShareHistoryInfoMgr::QShareHistoryInfoMgr(const QStringList& codes, QObject *parent) : QObject(parent)
{
    mCodesList = codes;
    QSinaShareVolInfoThread * vols = new QSinaShareVolInfoThread(mCodesList);
    connect(vols, SIGNAL(finished()), this, SLOT(slotShareFinanceInfoFinished()));
    this->moveToThread(&mWorkThread);
    mWorkThread.start();
    vols->start();
}

void QShareHistoryInfoMgr::slotShareFinanceInfoFinished()
{
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
    foreach (QString code, mCodesList) {
        QEastmoneyStockHistoryInfoThread* thread = new QEastmoneyStockHistoryInfoThread(code);
        pool.start(thread);
    }
    pool.waitForDone();
     qDebug()<<QDateTime::currentDateTime();
    //开始更新持股信息
    QEastMoneyHSGTShareAmount * thread = new QEastMoneyHSGTShareAmount;
    connect(thread, SIGNAL(finished()), this, SLOT(slotUpdateForeignAmountFinished()));
    connect(thread, SIGNAL(signalAmountFinshedAtDate(QString)), this, SIGNAL(signalUpdateAmountProcess(QString)));
    thread->start();
    emit signalHistoryDataFinished();
}

void QShareHistoryInfoMgr::slotUpdateAllShareFrom20170317()
{
    QThreadPool pool;
    pool.setExpiryTimeout(-1);
    pool.setMaxThreadCount(8);
    //取得自20170317的数据


    //开始更新日线数据
    foreach (QString code, mCodesList) {
        QEastmoneyStockHistoryInfoThread* thread = new QEastmoneyStockHistoryInfoThread(code);
        pool.start(thread);
    }
    pool.waitForDone();
     qDebug()<<QDateTime::currentDateTime();
    //开始更新持股信息
    QEastMoneyHSGTShareAmount * thread = new QEastMoneyHSGTShareAmount;
    connect(thread, SIGNAL(finished()), this, SLOT(slotUpdateForeignAmountFinished()));
    connect(thread, SIGNAL(signalAmountFinshedAtDate(QString)), this, SIGNAL(signalUpdateAmountProcess(QString)));
    thread->start();
    emit signalHistoryDataFinished();
}
