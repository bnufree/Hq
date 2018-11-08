#ifndef QEASTMONEYZJLXTHREAD_H
#define QEASTMONEYZJLXTHREAD_H

#include <QThread>
//#include "dbservices/sharedata.h"
//#include "qhttpget.h"

struct  zjlxData;
class   QHttpGet;
class QEastMoneyZjlxThread : public QObject
{
    Q_OBJECT
public:
    explicit QEastMoneyZjlxThread(QObject *parent = 0);
    ~QEastMoneyZjlxThread();
signals:
public slots:
    void slotRecvHttpContent(const QByteArray& bytes);
private:
    QThread     mWorkThread;
    QHttpGet    *mHttp;

};

#endif // QEASTMONEYZJLXTHREAD_H
