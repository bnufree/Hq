#ifndef QEASTMONEYNORTHBOUNDTHREAD_H
#define QEASTMONEYNORTHBOUNDTHREAD_H

#include <QThread>
#include "stockdata.h"

class QEastmoneyNorthBoundThread : public QThread
{
    Q_OBJECT
public:
    explicit QEastmoneyNorthBoundThread(QObject *parent = 0);
    ~QEastmoneyNorthBoundThread();
protected:
    void run();
signals:
    void signalUpdateNorthBoundList(const StockDataList& list);
};

#endif // QEASTMONEYNORTHBOUNDTHREAD_H
