#include "qsharehistoryinfomgr.h"
#include "qeastmoneystockhistoryinfothread.h"
#include "../dbservices.h"
#include "../qeastmonystockcodesthread.h"
#include <QDebug>

QShareHistoryInfoMgr::QShareHistoryInfoMgr(QObject *parent) : QObject(parent)
{
    //更新所有的代码
    QEastMonyStockCodesThread * codes = new QEastMonyStockCodesThread();
    connect(codes, SIGNAL(signalSendCodesList(QStringList)), this, SLOT(slotUpdateStockCodesList(QStringList)));
    connect(codes, SIGNAL(finished()), codes, SLOT(deleteLater()));
    connect(this, SIGNAL(signalStartGetHistory()), this, SLOT(slotStartGetHistory()));
    connect(this, SIGNAL(signalSetHistoryCodeList(QStringList)), this, SLOT(slotSetHistoryCodeList(QStringList)));
    connect(DATA_SERVICE, SIGNAL(signalSendShareHistoryLastDate(QString,QDate)), this, SLOT(slotRecvCodeHistoryDate(QString,QDate)));
    connect(DATA_SERVICE, SIGNAL(signalUpdateStkBaseinfoWithHistoryFinished(QString)), this, SLOT(slotUpdateHistoryFinished(QString)));
    this->moveToThread(&mWorkThread);
    mWorkThread.start();    
    codes->start();
}

QStringList QShareHistoryInfoMgr::getCodesList()
{
    return mCodesList;
}

void QShareHistoryInfoMgr::slotUpdateStockCodesList(const QStringList &list)
{
    qDebug()<<"total stk codelist:"<<list.length();
    emit signalSetHistoryCodeList(list);
}


void QShareHistoryInfoMgr::slotSetHistoryCodeList(const QStringList &list)
{
    mWorkingThreadList.clear();
    mWorkQueueThreadList.clear();
    mRecvCodeNum = 0;
    mCodesList = list;
    foreach (QString code, list) {
        DATA_SERVICE->signalQueryShareHistoryLastDate(code);
    }
}

void QShareHistoryInfoMgr::slotRecvCodeHistoryDate(const QString& code, const QDate& date)
{
    //qDebug()<<__FUNCTION__<<"code:"<<code<<" date:"<<date<<" len:"<<mWorkQueueThreadList.length();
    mRecvCodeNum++;
    QEastmoneyStockHistoryInfoThread* thread = new QEastmoneyStockHistoryInfoThread(code, date);
    if(thread)
    {
        connect(thread, SIGNAL(finished()), this, SLOT(slotSubThreadFinish()));
        mWorkQueueThreadList.append(thread);
    }
    //qDebug()<<__FUNCTION__<<"code:"<<code<<" date:"<<date<<" len:"<<mSubThreadList.length()<<" src:"<<mCodesList.length();
    if(mRecvCodeNum == mCodesList.length())
    {
        emit signalStartGetHistory();
    }
}

void QShareHistoryInfoMgr::slotStartGetHistory()
{
    if(mWorkQueueThreadList.length() == 0) return;
    int len = 100;
    if(len > mWorkQueueThreadList.length()) len = mWorkQueueThreadList.length();
    while (len > 0)
    {
        QEastmoneyStockHistoryInfoThread *thread = mWorkQueueThreadList.first();
        mWorkQueueThreadList.removeFirst();
        mWorkingThreadList.append(thread);
        thread->start();
        len --;
    }
}

void QShareHistoryInfoMgr::slotSubThreadFinish()
{
    QEastmoneyStockHistoryInfoThread* thread  = qobject_cast<QEastmoneyStockHistoryInfoThread*>(sender());

    if(thread)
    {
        //qDebug()<<__FUNCTION__<<__LINE__<<thread<<" "<<thread->getCode();
        mWorkingThreadList.removeOne(thread);
        thread->deleteLater();
    } else
    {
        //qDebug()<<__FUNCTION__<<__LINE__<<thread;
    }

    //qDebug()<<__FUNCTION__<<__LINE__<<thread;
    if(mWorkQueueThreadList.length() > 0)
    {
        //开启新的线程
        QEastmoneyStockHistoryInfoThread* worker = qobject_cast<QEastmoneyStockHistoryInfoThread*> (mWorkQueueThreadList.first());
        mWorkQueueThreadList.removeFirst();
        if(worker)
        {
            //qDebug()<<"start thread:"<<worker->getCode();
            worker->start();
        }

    }/* else
    {
        //历史信息更新完毕
        qDebug()<<"all thread finished!!!!!!!!!!!!!!!!!!";
        emit signalHistoryDataFinished();
    }*/

    //qDebug()<<__FUNCTION__<<__LINE__;
}

void QShareHistoryInfoMgr::slotUpdateHistoryFinished(const QString& code)
{
    static int finish_num = 0;
    if(code.length() > 0) finish_num++;
    //qDebug()<<__FUNCTION__<<__LINE__<<code<<"  "<<finish_num<<" total:"<<mCodesList.size();
    emit signalUpdateProcess(finish_num, mCodesList.count());
    if(finish_num == mCodesList.length())
    {
        //历史信息更新完毕
        qDebug()<<"all thread finished!!!!!!!!!!!!!!!!!!";
        emit signalHistoryDataFinished();
    }

}
