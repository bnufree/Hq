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
        connect(hy, SIGNAL(sendBlockDataList(int,BlockDataList,QMap<QString,BlockData>)), this, SLOT(slotReceiveBlockDataList(int,BlockDataList,QMap<QString,BlockData>)));
        connect(hy, SIGNAL(sendShareBlockDataMap(QMap<QString,QStringList>)), this, SIGNAL(sendShareBlockDataMap(QMap<QString,QStringList>)));
        mWorkThreadList.append(hy);
        emit hy->start();
    }
    while (1) {
        emit signalBlockDataListUpdated(mBlockDataMapList[mCurBlockType], mBlockDataMap);
        sleep(3);
    }

}

void QEastMoneyBlockMangagerThread::setCurBlockType(int type)
{
    mCurBlockType = type;
}

void QEastMoneyBlockMangagerThread::slotReceiveBlockDataList(int type, const BlockDataList &list, const QMap<QString, BlockData>& map)
{
    mBlockDataMapList[type] = list;
    foreach (QString key, map.keys()) {
        mBlockDataMap[key] = map[key];
        //qDebug()<<"key:"<<key<<" list:"<<mBlockDataMap[key].stklist;
    }
}

void QEastMoneyBlockMangagerThread::reverseSortRule()
{
    foreach (QEastMoneyBlockThread *t, mWorkThreadList) {
        t->reverseSortRule();
    }
}

