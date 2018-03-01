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
    connect(this, SIGNAL(signalUpdateAllShareFromDate(bool,QDate)), this, SLOT(slotUpdateAllShareFromDate(bool,QDate)));
    this->moveToThread(&mWorkThread);
    mWorkThread.start();
    Profiles::instance()->setDefault("UPDATE", "DATE", HqUtils::date2Str(QDate(2017,3,16)));
    mLastUpdateDate = HqUtils::dateFromStr(Profiles::instance()->value("UPDATE", "DATE").toString());
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
    QDate date = mLastUpdateDate.addDays(1);
    slotUpdateAllShareFromDate(false, date);
    Profiles::instance()->setValue("UPDATE", "DATE", HqUtils::date2Str(QDate::currentDate().addDays(-1)));
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

void QShareHistoryInfoMgr::slotUpdateAllShareFromDate(bool deldb, const QDate& date)
{
    emit signalUpdateHistoryMsg(QStringLiteral("开始更新外资持股数据..."));
    QDate wkDate = date;
    while(wkDate < QDate::currentDate())
    {
        if(!HqUtils::isWeekend(wkDate))
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
        StockDataList *list = (StockDataList*)(&mShareInfoMap[code.right(6)]);
        QEastmoneyStockHistoryInfoThread* thread = new QEastmoneyStockHistoryInfoThread(code, date, SAVE_DIR, deldb, list, this);
        mPool.start(thread);
    }
    mPool.waitForDone();
     qDebug()<<QDateTime::currentDateTime();
     emit signalUpdateHistoryMsg(QStringLiteral(""));
}
