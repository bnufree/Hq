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
    void signalBlockDataListUpdated(const BlockDataList& list, const QMap<QString, BlockData>& map);
    void signalReceiveBlockDataList(int type, const BlockDataList& list);
    void sendShareBlockDataMap(const QMap<QString, QStringList>& map);
public slots:
    void slotReceiveBlockDataList(int type, const BlockDataList& list, const QMap<QString, BlockData>& map);
private:
    //QThread         mWorkThread;
    QMap<int, BlockDataList>    mBlockDataMapList;      //分行业，概念，地区显示。int为类别码
    QMap<QString, BlockData>       mBlockDataMap;
    QList<QEastMoneyBlockThread*>  mWorkThreadList;
    int             mCurBlockType;
};

#endif // QEASTMONEYBLOCKMANGAGERTHREAD_H
