#ifndef STOCKDATATHREAD_H
#define STOCKDATATHREAD_H

#include <QObject>
#include <QThread>

class StockDataThread : public QThread
{
    Q_OBJECT
public:
    StockDataThread();
    ~StockDataThread();
};

#endif // STOCKDATATHREAD_H
