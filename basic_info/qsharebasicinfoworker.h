#ifndef QSHAREBASICINFOWORKER_H
#define QSHAREBASICINFOWORKER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include "data_structure/sharedata.h"

class QShareBasicInfoWorker : public QObject
{
    Q_OBJECT
public:
    explicit QShareBasicInfoWorker(QObject *parent = 0);


signals:
    void signalGetBasicInfo();
public slots:
    void slotGetBasicInfo();
private:

private:
    QThread             mWorkThread;

};

#endif // QSHAREBASICINFOWORKER_H
