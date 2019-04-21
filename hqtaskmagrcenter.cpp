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

HQTaskMagrCenter::HQTaskMagrCenter(QObject *parent) : \
    QObject(parent),\
    mShareInfoMergeThread(0),\
    mBlockMgr(0)
{
    connect(this, SIGNAL(signalStart()), this, SLOT(slotStart()));
    connect(DATA_SERVICE, SIGNAL(signalSendLastHSGTUpdateDate(ShareDate)), this, SLOT(slotStartUpdateHSGTTop10(ShareDate)));
    connect(DATA_SERVICE, SIGNAL(signalDbInitFinished()), this, SLOT(slotDBInitFinished()));
    connect(DATA_SERVICE, SIGNAL(signalAllShareCodeList(QStringList)), this, SLOT(slotShareCodesListFinished(QStringList)));
    connect(this, SIGNAL(signalSearchCodesOfText(QString)), DATA_SERVICE, SIGNAL(signalSearchCodesOfText(QString)));
    connect(this, SIGNAL(signalSetFavCode(QString)), this, SLOT(slotSetFavCode(QString)));
    this->moveToThread(&mWorkThread);
    mWorkThread.start();
}

HQTaskMagrCenter::~HQTaskMagrCenter()
{
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

void HQTaskMagrCenter::slotStartUpdateHSGTTop10(const ShareDate &date)
{
    if(date == ShareDate::latestActiveDay())
    {
        emit signalNewHsgtTop10Now();
        return;
    }
    QShareHsgtTop10Work* hsgtTop10 = new QShareHsgtTop10Work(this);
    connect(hsgtTop10, SIGNAL(finished()), hsgtTop10, SLOT(deleteLater()));
    hsgtTop10->start();
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
    //更新实时的指数
    QStringList indexlist;
    indexlist<<"sh000001"<<"sh000300"<<"sz399001"<<"sz399006"<<"sh000016"<<"sz399293";
    QSinaStkInfoThread* indexInfoThread = new QSinaStkInfoThread;
    mRealWorkObjList.append(indexInfoThread);
    connect(indexInfoThread, SIGNAL(sendStkDataList(ShareDataList)), this, SIGNAL(signalSendIndexRealDataList(ShareDataList)));
    indexInfoThread->signalSetStkList(indexlist);
    //更新北向的买入卖出情况
    QEastmoneyNorthBoundThread *north = new QEastmoneyNorthBoundThread();
    connect(north, SIGNAL(signalUpdateNorthBoundList(ShareHsgtList)), this, SIGNAL(signalSendNotrhBoundDataList(ShareHsgtList)));
    mRealWorkObjList.append(north);
    north->start();
    //实时全市场的行情初始化
    mShareInfoMergeThread = new QSinaStkResultMergeThread();
    mRealWorkObjList.append(mShareInfoMergeThread);
    connect(mShareInfoMergeThread, SIGNAL(sendStkDataList(ShareDataList)), this, SIGNAL(signalSendShareRealDataList(ShareDataList)));
    connect(DATA_SERVICE, SIGNAL(signalSendSearchCodesOfText(QStringList)),\
            mShareInfoMergeThread, SLOT(setSelfCodesList(QStringList)));

    mShareInfoMergeThread->setStkList(codes);
    mShareInfoMergeThread->setActive(true);
    mShareInfoMergeThread->setMktType(MKT_ZXG);
    mShareInfoMergeThread->start();
    return;
    //更新日线数据
    QShareHistoryInfoMgr* historyMgr = new QShareHistoryInfoMgr(codes);
    connect(historyMgr, SIGNAL(signalUpdateHistoryMsg(QString)), this, SIGNAL(signalUpdateHistoryMsg(QString)));
    connect(historyMgr, SIGNAL(signalUpdateHistoryFinished()), this, SLOT(slotUpdateHistoryFinished()));
    historyMgr->signalStartGetHistory();
#if 0
    return;
    //板块行情初始化
    mBlockMgr = new QEastMoneyBlockMangagerThread();
    mRealWorkObjList.append(mBlockMgr);
    connect(mBlockMgr, SIGNAL(signalBlockDataListUpdated(BlockDataVList)), this, SIGNAL(signalBlockDataListUpdated(BlockDataVList)));
    mBlockMgr->start();
    return;

#endif
}


void HQTaskMagrCenter::slotBaseDataListFinished(const QStringList& codes, const ShareDataList &list)
{
    qDebug()<<"update code finshed:"<<list.length();
    //更新后台数据信息
    DATA_SERVICE->signalUpdateShareBasicInfo(list);
    QShareBasicInfoWorker* ptr = qobject_cast<QShareBasicInfoWorker*>(sender());
    if(ptr)
    {
        ptr->deleteLater();
    }

    //更新实时的指数
    QStringList indexlist;
    indexlist<<"s_sh000001"<<"s_sh000300"<<"s_sz399001"<<"s_sz399006"<<"s_sh000016";
    QSinaStkInfoThread* indexInfoThread = new QSinaStkInfoThread;
    mRealWorkObjList.append(indexInfoThread);
    connect(indexInfoThread, SIGNAL(sendStkDataList(ShareDataList)), this, SIGNAL(signalSendIndexRealDataList(ShareDataList)));
    indexInfoThread->signalSetStkList(indexlist);
    //更新北向的买入卖出情况
    QEastmoneyNorthBoundThread *north = new QEastmoneyNorthBoundThread();
    connect(north, SIGNAL(signalUpdateNorthBoundList(QList<NS_BOUND_DATA>)), this, SIGNAL(signalSendNotrhBoundDataList(QList<NS_BOUND_DATA>)));
    mRealWorkObjList.append(north);
    north->start();
    //实时全市场的行情初始化
    mShareInfoMergeThread = new QSinaStkResultMergeThread();
    mRealWorkObjList.append(mShareInfoMergeThread);
    connect(mShareInfoMergeThread, SIGNAL(sendStkDataList(ShareDataList)), this, SIGNAL(signalSendShareRealDataList(ShareDataList)));
    connect(DATA_SERVICE, SIGNAL(signalSendSearchCodesOfText(QStringList)),\
            mShareInfoMergeThread, SLOT(setSelfCodesList(QStringList)));

    mShareInfoMergeThread->setStkList(codes);
    mShareInfoMergeThread->setActive(true);
    mShareInfoMergeThread->setMktType(MKT_ZXG);
    mShareInfoMergeThread->start();
#if 0
    return;
    //板块行情初始化
    mBlockMgr = new QEastMoneyBlockMangagerThread();
    mRealWorkObjList.append(mBlockMgr);
    connect(mBlockMgr, SIGNAL(signalBlockDataListUpdated(BlockDataVList)), this, SIGNAL(signalBlockDataListUpdated(BlockDataVList)));
    mBlockMgr->start();
    return;
#endif
    //更新日线数据
    QShareHistoryInfoMgr* historyMgr = new QShareHistoryInfoMgr(codes);
    connect(historyMgr, SIGNAL(signalUpdateHistoryMsg(QString)), this, SIGNAL(signalUpdateHistoryMsg(QString)));
    connect(historyMgr, SIGNAL(signalUpdateHistoryFinished()), this, SLOT(slotUpdateHistoryFinished()));
    historyMgr->signalStartGetHistory();
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
