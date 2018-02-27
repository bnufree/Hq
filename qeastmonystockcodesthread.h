#ifndef QEASTMONYSTOCKCODESTHREAD_H
#define QEASTMONYSTOCKCODESTHREAD_H

#include <QThread>
#include "qhttpget.h"

class StockDataList;
class QEastMonyStockCodesThread : public QObject
{
    Q_OBJECT
public:
    explicit QEastMonyStockCodesThread(QObject *parent = 0);
    ~QEastMonyStockCodesThread();

protected:
    bool getCodesFromFile(StockDataList& codes);
    bool writeCodes(const StockDataList& codes);


signals:
    void start();
    void signalSendCodesList(const QStringList& list);

public slots:
    void run();
    void slotRecvHttpContent(const QByteArray& bytes);
    void slotRecvAllCodes(const StockDataList& list);
    void slotDBInitFinished();
private:
    QHttpGet    *mHttp;
    QThread     mThread;
    QStringList mCodesList;
};

#endif // QEASTMONYSTOCKCODESTHREAD_H
