#ifndef QEASTMONEYNORTHBOUNDTHREAD_H
#define QEASTMONEYNORTHBOUNDTHREAD_H

#include <QThread>
#include "utils/qhttpget.h"
#include "utils/sharedata.h"

class QHttpGet;
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
    void    signalUpdateNorthBoundList(const QList<NS_BOUND_DATA> &list);
    void    start();
private:
    QList<QHttpGet*> mNorthBoudProxyList;
    QThread         mWorkThread;
    QList<NS_BOUND_DATA>       mBoundDataList;
};

#endif // QEASTMONEYNORTHBOUNDTHREAD_H
