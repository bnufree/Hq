#ifndef QEASTMONYSTOCKCODESTHREAD_H
#define QEASTMONYSTOCKCODESTHREAD_H

#include <QThread>
#include "qhttpget.h"

class QEastMonyStockCodesThread : public QObject
{
    Q_OBJECT
public:
    explicit QEastMonyStockCodesThread(QObject *parent = 0);
    ~QEastMonyStockCodesThread();

protected:
    bool getCodesFromFile(QStringList& codes);
    bool writeCodes(const QStringList& codes);

signals:
    void start();
    void signalSendCodesList(const QStringList& list);

public slots:
    void run();
    void slotRecvHttpContent(const QByteArray& bytes);
    void slotRecvAllCodes(const QStringList& list);
private:
    QHttpGet    *mHttp;
    QThread     mThread;
};

#endif // QEASTMONYSTOCKCODESTHREAD_H
