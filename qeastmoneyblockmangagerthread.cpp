#include "qeastmoneyblockmangagerthread.h"
#include <QDebug>

QEastMoneyBlockMangagerThread::QEastMoneyBlockMangagerThread(QObject *parent) : QThread(parent)
{
    mCurBlockType = 2;
    //this->moveToThread(&mWorkThread);
    //connect(this, SIGNAL(signalReceiveBlockDataList(int,BlockDataList)), this,SLOT(slotReceiveBlockDataList(int,BlockDataList)), Qt::QueuedConnection);
}

QEastMoneyBlockMangagerThread::~QEastMoneyBlockMangagerThread()
{

}

void QEastMoneyBlockMangagerThread::run()
{
    //创建板块线程，开始更新板块信息
    for(int i= 1; i<=3; i++)
    {
        QEastMoneyBlockThread *hy = new QEastMoneyBlockThread(i);
        connect(hy, SIGNAL(sendBlockDataList(int,BlockDataList)), this, SLOT(slotReceiveBlockDataList(int,BlockDataList)));
        mWorkThreadList.append(hy);
        emit hy->start();
    }
    while (1) {
        emit signalBlockDataListUpdated(mBlockDataMapList[mCurBlockType]);
        sleep(3);
    }

}

void QEastMoneyBlockMangagerThread::setCurBlockType(int type)
{
    mCurBlockType = type;
}

void QEastMoneyBlockMangagerThread::slotReceiveBlockDataList(int type, const BlockDataList &list)
{
    qDebug()<<__FUNCTION__<<__LINE__;
    mBlockDataMapList[type] = list;
}

void QEastMoneyBlockMangagerThread::reverseSortRule()
{
    foreach (QEastMoneyBlockThread *t, mWorkThreadList) {
        t->reverseSortRule();
    }
}

