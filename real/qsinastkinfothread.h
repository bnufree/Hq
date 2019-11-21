#ifndef QSINASTKINFOTHREAD_H
#define QSINASTKINFOTHREAD_H

#include <QThread>
#include "data_structure/sharedata.h"

class QSinaStkInfoThread : public QThread
{
    Q_OBJECT
public:
    explicit QSinaStkInfoThread(const QStringList& list, bool send, QObject *parent = 0);
    ~QSinaStkInfoThread();
    int  getStkCount();

protected:
    void run();
signals:
    void    sendStkDataList(const ShareDataList& list);
public:
    void    setStkList(const QStringList& list);
private:
    void    slotRecvHttpContent(const QByteArray& bytes);
private:
    QStringList mStkList;
    bool        mSendResFlag;
};

#endif // QSINASTKINFOTHREAD_H
