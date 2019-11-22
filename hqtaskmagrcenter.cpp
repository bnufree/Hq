#include "hqtaskmagrcenter.h"
#include "dbservices/dbservices.h"
#include "basic_info/qsharebasicinfoworker.h"
#include "history/qsharehistoryinfomgr.h"
#include "real/qsinastkinfothread.h"
#include "real/qeastmoneynorthboundthread.h"
#include "real/qsinastkresultmergethread.h"
#include "block/qeastmoneyblockmangagerthread.h"
#include "basic_info/qsharecodeswork.h"
#include "basic_info/qsharehsgttop10work.h"
#include "basic_info/qsharefhspwork.h"
#include "basic_info/qsharefinancialinfowork.h"
#include "basic_info/qshareactivedateupdatethread.h"
#include "real/qhqindexthread.h"


HQTaskMagrCenter::HQTaskMagrCenter(QObject *parent) : \
    QObject(parent),\
    mShareInfoMergeThread(0),\
    mBlockMgr(0),
    mWorkDayTimeMonitorThread(0),
    mHistoryInfoMgr(0),
    mIndexThread(0)
{
    connect(this, SIGNAL(signalStart()), this, SLOT(slotStart()));
    connect(DATA_SERVICE, SIGNAL(signalDbInitFinished()), this, SLOT(slotDBInitFinished()));
    connect(DATA_SERVICE, SIGNAL(signalAllShareCodeList(QStringList)), this, SLOT(slotShareCodesListFinished(QStringList)));
    connect(this, SIGNAL(signalSearchCodesOfText(QString)), DATA_SERVICE, SIGNAL(signalSearchCodesOfText(QString)));
    connect(this, SIGNAL(signalSetFavCode(QString)), this, SLOT(slotSetFavCode(QString)));
    //开始实时指数更新
    QHqIndexThread* thread = new QHqIndexThread;
    connect(thread, SIGNAL(signalSendIndexDataList(ShareDataList)), this, SIGNAL(signalSendIndexRealDataList(ShareDataList)));
    //开始更新北向
    QEastmoneyNorthBoundThread *north = new QEastmoneyNorthBoundThread();
    connect(north, SIGNAL(signalUpdateNorthBoundList(ShareHsgtList)), this, SIGNAL(signalSendNotrhBoundDataList(ShareHsgtList)));

    this->moveToThread(&mWorkThread);
    mWorkThread.start();
    thread->start();
    north->start();
}

HQTaskMagrCenter::~HQTaskMagrCenter()
{
    if(mWorkDayTimeMonitorThread) mWorkDayTimeMonitorThread->quit();
    mWorkThread.quit();
}

void HQTaskMagrCenter::slotStart()
{
    qDebug()<<__FUNCTION__<<__LINE__;
    //开启数据库初始化
    DATA_SERVICE->signalInitDBTables();
    qDebug()<<__FUNCTION__<<__LINE__;
}

void HQTaskMagrCenter::slotDBInitFinished()
{
    mWorkDayTimeMonitorThread = new QShareActiveDateUpdateThread;
    connect(mWorkDayTimeMonitorThread, SIGNAL(signalUpdateHistoryWorkDays()), this, SLOT(slotFinishUpdateWorkDays()));
    connect(mWorkDayTimeMonitorThread, SIGNAL(signalNewWorkDateNow()), this, SLOT(slotFinishUpdateWorkDays()));
    mWorkDayTimeMonitorThread->start();

}

void HQTaskMagrCenter::slotFinishUpdateWorkDays()
{
//    emit signalWorkingDayfinished();
    qDebug()<<__FUNCTION__<<__LINE__;
    //数据库初始化完成,开始获取最新的代码列表
    QShareCodesWork *codeWorker = new QShareCodesWork(this);
    connect(codeWorker, SIGNAL(finished()), codeWorker, SLOT(deleteLater()));
    codeWorker->start();


    //开始获取当日沪深股通数据，线程常驻，直到当日数据确定获取
    QShareHsgtTop10Work* hsgtTop10 = new QShareHsgtTop10Work(this);
    connect(hsgtTop10, SIGNAL(finished()), hsgtTop10, SLOT(deleteLater()));
    hsgtTop10->start();
}

void HQTaskMagrCenter::slotNewWorDayChangeNow()
{
    if(mHistoryInfoMgr) mHistoryInfoMgr->signalStartStatic();
}


void HQTaskMagrCenter::slotSetFavCode(const QString &code)
{
    //if(mShareBasicWorker) mShareBasicWorker->signalUpdateFavCode(code);
    emit DATA_SERVICE->signalSetFavCode(code);
}

void   HQTaskMagrCenter::setMktType(int type)
{
    if(mShareInfoMergeThread) mShareInfoMergeThread->setMktType(type);
}

void   HQTaskMagrCenter::setSortType(int type)
{
    if(mShareInfoMergeThread) mShareInfoMergeThread->setSortType(type);
}

void   HQTaskMagrCenter::setActive(bool active)
{
    if(mShareInfoMergeThread) mShareInfoMergeThread->setActive(active);
}

void   HQTaskMagrCenter::setSelfCodesList(const QStringList& list )
{
    if(mShareInfoMergeThread) mShareInfoMergeThread->setSelfCodesList(list);
}

void   HQTaskMagrCenter::setDisplayPage(int val)
{
    if(mShareInfoMergeThread) mShareInfoMergeThread->setDisplayPage(val);
}

void HQTaskMagrCenter::slotUpdateHSGTOfCode(const QString &code)
{

}

void HQTaskMagrCenter::setDisplayChinaTop10()
{
    if(mShareInfoMergeThread) mShareInfoMergeThread->setDisplayChinaTop10();
}

void HQTaskMagrCenter::addSpecialConcern(const QString &code)
{
    if(mIndexThread) mIndexThread->setStkList(QStringList()<<code);
}

void HQTaskMagrCenter::reverseSortRule()
{
    if(mBlockMgr)   mBlockMgr->reverseSortRule();
}

void HQTaskMagrCenter::setCurBlockType(int type)
{
    if(mBlockMgr)   mBlockMgr->setCurBlockType(type);
}

void HQTaskMagrCenter::slotShareCodesListFinished(const QStringList& codes)
{
    qDebug()<<"update code finshed:"<<codes.length();
    //获取财务信息
    QShareFinancialInfoWork* finance = new QShareFinancialInfoWork(codes, this);
    connect(finance, SIGNAL(finished()), finance, SLOT(deleteLater()));
    finance->start();
    //获取分配信息
    QShareFHSPWork* fhsp = new QShareFHSPWork(this);
    connect(fhsp, SIGNAL(finished()), fhsp, SLOT(deleteLater()));
    fhsp->start();


    //实时全市场的行情初始化
    int nthread = 10;
    int thread_code = (codes.length() + nthread-1 ) / nthread;
    for(int i=0; i<nthread; i++)
    {
        QStringList wklist = codes.mid(i*thread_code, thread_code);
        QSinaStkInfoThread *wkthread = new QSinaStkInfoThread(wklist, false);
        connect(wkthread, SIGNAL(finished()), wkthread, SLOT(deleteLater()));
        wkthread->start();
    }
    mShareInfoMergeThread = new QSinaStkResultMergeThread();
    mRealWorkObjList.append(mShareInfoMergeThread);
    connect(mShareInfoMergeThread, SIGNAL(sendStkDataList(ShareDataList)), this, SIGNAL(signalSendShareRealDataList(ShareDataList)));
    connect(DATA_SERVICE, SIGNAL(signalSendSearchCodesOfText(QStringList)),\
            mShareInfoMergeThread, SLOT(setSelfCodesList(QStringList)));

    mShareInfoMergeThread->setActive(true);
    mShareInfoMergeThread->setMktType(MKT_ZXG);
    mShareInfoMergeThread->start();
    return;
    //更新日线数据
    mHistoryInfoMgr = new QShareHistoryInfoMgr(codes);
    connect(mHistoryInfoMgr, SIGNAL(signalUpdateHistoryMsg(QString)), this, SIGNAL(signalUpdateHistoryMsg(QString)));
    connect(mHistoryInfoMgr, SIGNAL(signalUpdateHistoryFinished()), this, SLOT(slotUpdateHistoryFinished()));
    mHistoryInfoMgr->signalStartGetHistory();
    //板块行情初始化
    mBlockMgr = new QEastMoneyBlockMangagerThread();
    mRealWorkObjList.append(mBlockMgr);
    connect(mBlockMgr, SIGNAL(signalBlockDataListUpdated(BlockDataList)), this, SIGNAL(signalBlockDataListUpdated(BlockDataList)));
    mBlockMgr->start();
    return;
}

void HQTaskMagrCenter::slotUpdateHistoryFinished()
{
    QShareHistoryInfoMgr* ptr = qobject_cast<QShareHistoryInfoMgr*>(sender());
    if(ptr)
    {
        ptr->deleteLater();
    }

    emit signalUpdateHistoryMsg("");
}
