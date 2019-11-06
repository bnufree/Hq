#ifndef QSINASTKINFOTHREAD_H
#define QSINASTKINFOTHREAD_H

#include <QThread>
#include <QList>
#include <QMap>
#include "utils/comdatadefines.h"
//#include <QStringList>
#include <QTimer>
#include "utils/qhttpget.h"
#include "data_structure/sharedata.h"

class QSinaStkInfoThread : public QObject
{
    Q_OBJECT
public:
    explicit QSinaStkInfoThread(bool send, QObject *parent = 0);
    ~QSinaStkInfoThread();
    int  getStkCount();
signals:
    void    sendStkDataList(const ShareDataList& list);
    void    signalSetStkList(const QStringList& list);
public slots:
    void    setStkList(const QStringList& list);
    void    slotRecvHttpContent(const QByteArray& bytes);
    void    slotUpdateInfo();
    void    slotRecvError(QNetworkReply::NetworkError);
private:
    QStringList mStkList;
    QThread     mWorkThread;
    QHttpGet    *mHttp;
    bool        mSendResFlag;
};

#endif // QSINASTKINFOTHREAD_H
