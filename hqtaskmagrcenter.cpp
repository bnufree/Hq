#include "hqtaskmagrcenter.h"
#include "dbservices/dbservices.h"
#include "basic_info/qsharebasicinfoworker.h"
#include "history/qsharehistoryinfomgr.h"
#include "qsinastkinfothread.h"
#include "qeastmoneynorthboundthread.h"
#include "qsinastkresultmergethread.h"
#include "block/qeastmoneyblockmangagerthread.h"

HQTaskMagrCenter::HQTaskMagrCenter(QObject *parent) : \
    QObject(parent),\
    mShareInfoMergeThread(0),\
    mBlockMgr(0)
{
    connect(this, SIGNAL(signalStart()), this, SLOT(slotStart()));
    connect(DATA_SERVICE, SIGNAL(signalDbInitFinished()), this, SLOT(slotDBInitFinished()));
    connect(this, SIGNAL(signalSearchCodesOfText(QString)), DATA_SERVICE, SIGNAL(signalSearchCodesOfText(QString)));
    connect(this, SIGNAL(signalSetFavCode(QString)), DATA_SERVICE, SIGNAL(signalSetFavCode(QString)));
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
    //数据库初始化完成，开始取得基本的信息
    QShareBasicInfoWorker *basic_info = new QShareBasicInfoWorker;
    connect(basic_info, SIGNAL(signalBaseDataListFinished(QStringList,ShareBaseDataList)),\
            this, SLOT(slotBaseDataListFinished(QStringList,ShareBaseDataList)));
    basic_info->signalGetBasicInfo();

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

void HQTaskMagrCenter::reverseSortRule()
{
    if(mBlockMgr)   mBlockMgr->reverseSortRule();
}

void HQTaskMagrCenter::setCurBlockType(int type)
{
    if(mBlockMgr)   mBlockMgr->setCurBlockType(type);
}

void HQTaskMagrCenter::slotBaseDataListFinished(const QStringList& codes, const ShareBaseDataList &list)
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
    indexlist<<"s_sh000001"<<"s_sh000300"<<"s_sz399001"<<"s_sz399006"<<"s_sh000016"<<"s_sh000010";
    QSinaStkInfoThread* indexInfoThread = new QSinaStkInfoThread;
    mRealWorkObjList.append(indexInfoThread);
    connect(indexInfoThread, SIGNAL(sendStkDataList(ShareDataList)), this, SIGNAL(signalSendIndexRealDataList(ShareDataList)));
    indexInfoThread->signalSetStkList(indexlist);
    //更新北向的买入卖出情况
    QEastmoneyNorthBoundThread *north = new QEastmoneyNorthBoundThread();
    connect(north, SIGNAL(signalUpdateNorthBoundList(ShareDataList)), this, SIGNAL(signalSendIndexRealDataList(ShareDataList)));
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
    mShareInfoMergeThread->setMktType(MKT_ALL);
    mShareInfoMergeThread->start();
    //板块行情初始化
    mBlockMgr = new QEastMoneyBlockMangagerThread();
    mRealWorkObjList.append(mBlockMgr);
    connect(mBlockMgr, SIGNAL(signalBlockDataListUpdated(BlockDataVList)), this, SIGNAL(signalBlockDataListUpdated(BlockDataVList)));
    mBlockMgr->start();

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
