#include "qsharehistoryinfomgr.h"
#include "qeastmoneystockhistoryinfothread.h"
#include "dbservices.h"
#include <QDebug>

QShareHistoryInfoMgr::QShareHistoryInfoMgr(QObject *parent) : QObject(parent)
{
    connect(this, SIGNAL(signalStartGetHistory()), this, SLOT(slotStartGetHistory()));
    connect(this, SIGNAL(singalSetHistoryCodeList(QStringList)), this, SLOT(slotSetHistoryCodeList(QStringList)));
    connect(DATA_SERVICE, SIGNAL(signalSendShareHistoryLastDate(QString,QDate)), this, SLOT(slotRecvCodeHistoryDate(QString,QDate)));
    this->moveToThread(&mWorkThread);
    mWorkThread.start();
}


void QShareHistoryInfoMgr::slotSetHistoryCodeList(const QStringList &list)
{
    mSubThreadList.clear();
    mCodesList = list;
    foreach (QString code, list) {
        DATA_SERVICE->signalQueryShareHistoryLastDate(code);
    }
}

void QShareHistoryInfoMgr::slotRecvCodeHistoryDate(const QString& code, const QDate& date)
{
    //qDebug()<<__FUNCTION__<<"code:"<<code<<" date:"<<date<<" len:"<<mSubThreadList.length();
    QEastmoneyStockHistoryInfoThread* thread = new QEastmoneyStockHistoryInfoThread(code, date);
    if(thread)
    {
        connect(thread, SIGNAL(finished()), this, SLOT(slotSubThreadFinish()));
        mSubThreadList.append(thread);
    }
    //qDebug()<<__FUNCTION__<<"code:"<<code<<" date:"<<date<<" len:"<<mSubThreadList.length()<<" src:"<<mCodesList.length();
    if(mSubThreadList.length() == mCodesList.length())
    {
        emit signalStartGetHistory();
    }
}

void QShareHistoryInfoMgr::slotStartGetHistory()
{
    if(mSubThreadList.length() == 0) return;
    int len = 10;
    if(len < mSubThreadList.length()) len = mSubThreadList.length();
    for(int i=0; i<len; i++)
    {
        mSubThreadList[i]->start();
    }
}

void QShareHistoryInfoMgr::slotSubThreadFinish()
{
    QEastmoneyStockHistoryInfoThread* thread  = qobject_cast<QEastmoneyStockHistoryInfoThread*>(sender());
    if(thread)
    {
        mSubThreadList.removeOne(thread);
        thread->deleteLater();
    }

    qDebug()<<__FUNCTION__<<__LINE__<<thread;
    if(mSubThreadList.length() > 0)
    {
        //开启新的线程
        QEastmoneyStockHistoryInfoThread* worker = qobject_cast<QEastmoneyStockHistoryInfoThread*> (mSubThreadList.first());
        if(worker) worker->start();
    } else
    {
        //历史信息更新完毕
        qDebug()<<"all thread finished!!!!!!!!!!!!!!!!!!";
    }

    qDebug()<<__FUNCTION__<<__LINE__;
}
