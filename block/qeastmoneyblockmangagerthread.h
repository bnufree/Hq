#ifndef QEASTMONEYBLOCKMANGAGERTHREAD_H
#define QEASTMONEYBLOCKMANGAGERTHREAD_H

#include <QObject>
#include <QThread>
#include "blockdata.h"

class QEastMoneyBlockThread;
class QTimer;

class QEastMoneyBlockMangagerThread : public QObject
{
    Q_OBJECT
public:
    explicit QEastMoneyBlockMangagerThread(QObject *parent = 0);
    ~QEastMoneyBlockMangagerThread();
    void    setCurBlockType(int type);
    void    reverseSortRule();

signals:
    void signalBlockDataListUpdated(const BlockDataList& list);
    void start();
public slots:
    void slotRecvBlockDataList(const BlockDataList& list);
    void slotStartRunMgr();
    void slotUpdateBlockInfo();
private:
    QList<QEastMoneyBlockThread*>  mWorkThreadList;
    int             mCurBlockType;
    QThread         mWorkThread;
    QTimer          *mWorkTimer;
    BlockDataList   mBlockDataList;
};

#endif // QEASTMONEYBLOCKMANGAGERTHREAD_H
