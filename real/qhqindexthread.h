#ifndef QHQINDEXTHREAD_H
#define QHQINDEXTHREAD_H

#include <QThread>
#include "data_structure/sharedata.h"
#include <QMutex>

class QHqIndexThread : public QThread
{
    Q_OBJECT
public:
    explicit QHqIndexThread(QObject *parent = 0);

    ShareDataList getDataList();
protected:
    void run();
signals:
    void     signalSendIndexDataList(const ShareDataList& list, qint64 time);

public slots:
private:
    ShareDataList       mDataList;
    QMutex              mMutex;
};

#endif // QHQINDEXTHREAD_H
