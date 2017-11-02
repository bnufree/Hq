#ifndef QSINASTKINFOTHREAD_H
#define QSINASTKINFOTHREAD_H

#include <QThread>
#include <QList>
#include <QMap>
#include "stockdata.h"
#include "utils/comdatadefines.h"
#include <QStringList>
#include <QTimer>
#include "qhttpget.h"


class QSinaStkInfoThread : public QObject
{
    Q_OBJECT
public:
    explicit QSinaStkInfoThread(QObject *parent = 0);
    ~QSinaStkInfoThread();
    int  getStkCount();
signals:
    void    sendStkDataList(const StockDataList& list);
    void    signalSetStkList(const QStringList& list);
public slots:
    void    setStkList(const QStringList& list);
    void    slotRecvHttpContent(const QByteArray& bytes);
    void    slotUpdateInfo();
    void    slotRecvError(QNetworkReply::NetworkError);
private:
    QStringList mStkList;
    QThread     mWorkThread;
    QMap<QString, StockData>    mDataMap;
    //QTimer      *mUpdateTimer;
    QHttpGet    *mHttp;
};

#endif // QSINASTKINFOTHREAD_H
