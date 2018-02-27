#include "qsharehistoryinfomgr.h"
#include "qeastmoneystockhistoryinfothread.h"
#include "dbservices/dbservices.h"
#include "qsinasharevolinfothread.h"
#include "../qeastmoneyhsgtshareamount.h"
#include <QDebug>
#include "qhkexchangevoldataprocess.h"
#include "utils/hqutils.h"
#include "utils/profiles.h"

#define     SAVE_DIR    QDir::currentPath() + "/data/"
QShareHistoryInfoMgr::QShareHistoryInfoMgr(const QStringList& codes, QObject *parent) : QObject(parent)
{
    mCodesList = codes;
    mPool.setExpiryTimeout(-1);
    mPool.setMaxThreadCount(8);
    connect(this, SIGNAL(signalGetFianceInfo()), this, SLOT(slotGetFinanceInfo()));
    connect(this, SIGNAL(signalUpdateAllShareFrom20170317()), this, SLOT(slotUpdateAllShareFromLastUpdateDate()));
    this->moveToThread(&mWorkThread);
    mWorkThread.start();
    Profiles::instance()->setDefault("UPDATE", "DATE", QDate(2017,3,16));
    mLastUpdateDate = Profiles::instance()->value("UPDATE", "DATE").toDate();
}

QShareHistoryInfoMgr::~QShareHistoryInfoMgr()
{
    mPool.clear();
}

void QShareHistoryInfoMgr::slotGetFinanceInfo()
{
    emit signalUpdateHistoryMsg(QStringLiteral("正在更新财务信息..."));
    QSinaShareVolInfoThread * vols = new QSinaShareVolInfoThread(mCodesList);
    connect(vols, SIGNAL(finished()), this, SLOT(slotUpdateAllShareFromLastUpdateDate()));
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
        QEastmoneyStockHistoryInfoThread* thread = new QEastmoneyStockHistoryInfoThread(code, QDate(),0, this);
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

void QShareHistoryInfoMgr::slotUpdateAllShareFromLastUpdateDate()
{
    QDate date = mLastUpdateDate.addDays(1);
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
        QEastmoneyStockHistoryInfoThread* thread = new QEastmoneyStockHistoryInfoThread(code, mLastUpdateDate, &(mShareInfoMap[code.right(6)]), this);
        mPool.start(thread);
    }
    mPool.waitForDone();
     qDebug()<<QDateTime::currentDateTime();
     emit signalUpdateHistoryMsg(QStringLiteral(""));
     //开始保存到文件
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
#if 0
     QString fileName = QString("%1%2.dat").arg(SAVE_DIR).arg(mDate.toString("yyyyMMdd"));
     //将数据写入到文件
     if(list.length() > 0)
     {
         FILE *fp = fopen(fileName.toStdString().data(), "wb+");
         if(fp)
         {
             QDateTime wkDateTime;
             wkDateTime.setDate(mDate);
             qint64 cur =wkDateTime.addDays(-1).toMSecsSinceEpoch();
             fwrite(&cur, sizeof(cur), 1, fp);
             for(int i=0; i<list.size(); i++){
                 fwrite(&(list[i].mCode), sizeof(QString), 1, fp);
                 fwrite(&(list[i].mForeignVol), sizeof(qint64), 1, fp);
             }
             //然后在移动到开头写入时间，保证是最新的
             fseek(fp, 0, SEEK_SET);
             cur = wkDateTime.toMSecsSinceEpoch();
             fwrite(&cur, sizeof(cur), 1, fp);
             fclose(fp);
         }
     }
#endif
     Profiles::instance()->setValue("UPDATE", "DATE", QDate::currentDate().addDays(-1));
}
