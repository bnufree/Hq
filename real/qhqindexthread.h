#ifndef QHQINDEXTHREAD_H
#define QHQINDEXTHREAD_H

#include <QThread>
#include "data_structure/sharedata.h"

class QHqIndexThread : public QThread
{
    Q_OBJECT
public:
    explicit QHqIndexThread(QObject *parent = 0);

protected:
    void run();
signals:
    void     signalSendIndexDataList(const ShareDataList& list);

public slots:
};

#endif // QHQINDEXTHREAD_H
