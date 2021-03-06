﻿#ifndef QEASTMONEYBLOCKMANGAGERTHREAD_H
#define QEASTMONEYBLOCKMANGAGERTHREAD_H

#include <QObject>
#include <QThread>
#include "data_structure/hqblockdata.h"

class QEastMoneyBlockThread;
class QTimer;

class QEastMoneyBlockMangagerThread : public QObject
{
    Q_OBJECT
public:
    explicit QEastMoneyBlockMangagerThread(QObject *parent = 0);
    ~QEastMoneyBlockMangagerThread();


signals:
    void signalBlockDataListUpdated(const BlockDataList& list);
    void start();
public slots:
    void slotRecvBlockDataList(const BlockDataList& list);
    void slotStartRunMgr();
    void slotUpdateBlockInfo();    
    void    reverseSortRule();
    void    setCurBlockType(int type);
private:
    QList<QEastMoneyBlockThread*>  mWorkThreadList;
    int             mCurBlockType;
    QThread         mWorkThread;
    QTimer          *mWorkTimer;
    BlockDataList   mBlockDataList;
    int             mBlockRule;
};

#endif // QEASTMONEYBLOCKMANGAGERTHREAD_H
