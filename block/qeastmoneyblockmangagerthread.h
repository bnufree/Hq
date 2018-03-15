#ifndef QEASTMONEYBLOCKMANGAGERTHREAD_H
#define QEASTMONEYBLOCKMANGAGERTHREAD_H

#include <QObject>
#include <QThread>
#include "utils/blockdata.h"

class QEastMoneyBlockThread;
class QTimer;

class QEastMoneyBlockMangagerThread : public QObject
{
    Q_OBJECT
public:
    explicit QEastMoneyBlockMangagerThread(QObject *parent = 0);
    ~QEastMoneyBlockMangagerThread();


signals:
    void signalBlockDataListUpdated(const BlockDataVList& list);
    void start();
public slots:
    void slotRecvBlockDataList(const BlockDataPList& list);
    void slotStartRunMgr();
    void slotUpdateBlockInfo();    
    void    reverseSortRule();
    void    setCurBlockType(int type);
private:
    QList<QEastMoneyBlockThread*>  mWorkThreadList;
    int             mCurBlockType;
    QThread         mWorkThread;
    QTimer          *mWorkTimer;
    BlockDataPList   mBlockDataList;
    int             mBlockRule;
};

#endif // QEASTMONEYBLOCKMANGAGERTHREAD_H
