#ifndef QEASTMONEYBLOCKMANGAGERTHREAD_H
#define QEASTMONEYBLOCKMANGAGERTHREAD_H

#include <QObject>
#include <QThread>
#include <QMap>
#include "qeastmoneyblockthread.h"

class QEastMoneyBlockMangagerThread : public QThread
{
    Q_OBJECT
public:
    explicit QEastMoneyBlockMangagerThread(QObject *parent = 0);
    ~QEastMoneyBlockMangagerThread();
    void    setCurBlockType(int type);
    void    reverseSortRule();
protected:
    void run();

signals:
    void signalBlockDataListUpdated(const BlockDataList& list);
    void signalReceiveBlockDataList(int type, const BlockDataList& list);
public slots:
    void slotReceiveBlockDataList(int type, const BlockDataList& list);
private:
    //QThread         mWorkThread;
    QMap<int, BlockDataList>    mBlockDataMapList;
    QList<QEastMoneyBlockThread*>  mWorkThreadList;
    int             mCurBlockType;
};

#endif // QEASTMONEYBLOCKMANGAGERTHREAD_H
