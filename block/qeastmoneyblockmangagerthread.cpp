#include "qeastmoneyblockmangagerthread.h"
#include "qeastmoneyblockthread.h"
#include <QDebug>
#include <QTimer>

QEastMoneyBlockMangagerThread::QEastMoneyBlockMangagerThread(QObject *parent) : QObject(parent)
{
    mCurBlockType = BLOCK_HY;
    mWorkTimer = new QTimer;
    mWorkTimer->setInterval(3000);
    connect(mWorkTimer, SIGNAL(timeout()), this, SLOT(slotUpdateBlockInfo()));
    connect(this, SIGNAL(start()), this, SLOT(slotStartRunMgr()));
    this->moveToThread(&mWorkThread);
    mWorkThread.start();
    mWorkTimer->start();
}

QEastMoneyBlockMangagerThread::~QEastMoneyBlockMangagerThread()
{
    if(mWorkTimer)
    {
        mWorkTimer->stop();
        mWorkTimer->deleteLater();
    }
    foreach (QEastMoneyBlockThread* thread, mWorkThreadList) {
        thread->stop();
        thread->deleteLater();
    }
    mWorkThread.wait();
    mWorkThread.quit();
}

void QEastMoneyBlockMangagerThread::slotStartRunMgr()
{
    //创建板块线程，开始更新板块信息
    for(int i= BLOCK_GN; i<=BLOCK_DQ; i++)
    {
        if(i== BLOCK_GN || i==BLOCK_HY || i== BLOCK_DQ)
        {
            QEastMoneyBlockThread *hy = new QEastMoneyBlockThread(i);
            connect(hy, SIGNAL(sendBlockDataList(BlockDataList)), this, SLOT(slotRecvBlockDataList(BlockDataList)));
            mWorkThreadList.append(hy);
            hy->start();
        }
    }
}

void QEastMoneyBlockMangagerThread::setCurBlockType(int type)
{
    mCurBlockType = type;
}

void QEastMoneyBlockMangagerThread::reverseSortRule()
{
    foreach (QEastMoneyBlockThread *t, mWorkThreadList) {
        t->reverseSortRule();
    }
}

void QEastMoneyBlockMangagerThread::slotRecvBlockDataList(const BlockDataList &list)
{
    foreach (BlockData* data, list) {
        if(mBlockDataList.contains(data)) continue;
        mBlockDataList.append(data);
    }

}

void QEastMoneyBlockMangagerThread::slotUpdateBlockInfo()
{
    BlockDataList wklist;
    foreach (BlockData *data, mBlockDataList) {
        if(data->mBlockType & mCurBlockType)
        {
            wklist.append(data);
        }
    }

    emit signalBlockDataListUpdated(wklist);
}

