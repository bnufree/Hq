#ifndef QEASTMONEYNORTHBOUNDTHREAD_H
#define QEASTMONEYNORTHBOUNDTHREAD_H

#include <QThread>
#include "stockdata.h"

class QEastmoneyNorthBoundThread : public QThread
{
    Q_OBJECT
public:
    explicit QEastmoneyNorthBoundThread(QObject *parent = 0);
protected:
    void run();
    double  changeRMBString(const QString& src);
signals:
    void signalUpdateNorthBoundList(const StockDataList& list);
};

#endif // QEASTMONEYNORTHBOUNDTHREAD_H
