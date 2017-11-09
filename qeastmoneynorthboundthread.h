#ifndef QEASTMONEYNORTHBOUNDTHREAD_H
#define QEASTMONEYNORTHBOUNDTHREAD_H

#include <QThread>
#include "stockdata.h"
#include "qhttpget.h"

class QEastmoneyNorthBoundThread : public QObject
{
    Q_OBJECT
public:
    explicit QEastmoneyNorthBoundThread(QObject *parent = 0);
    double  changeRMBString(const QString& src);
    ~QEastmoneyNorthBoundThread();
public slots:
    void    run();
    void    slotRecvHttpContent(const QByteArray& bytes);
signals:
    void    signalUpdateNorthBoundList(const StockDataList& list);
    void    start();
private:
    QHttpGet        *mHttp;
    QThread         mWorkThread;
};

#endif // QEASTMONEYNORTHBOUNDTHREAD_H
