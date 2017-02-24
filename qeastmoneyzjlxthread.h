#ifndef QEASTMONEYZJLXTHREAD_H
#define QEASTMONEYZJLXTHREAD_H

#include <QThread>
#include "stockdata.h"


class QEastMoneyZjlxThread : public QThread
{
    Q_OBJECT
public:
    explicit QEastMoneyZjlxThread(QObject *parent = 0);
    ~QEastMoneyZjlxThread();
protected:
    void run();

signals:
    void sendZjlxDataList(const QList<zjlxData>& zjlxDataList);
public slots:

};

#endif // QEASTMONEYZJLXTHREAD_H
