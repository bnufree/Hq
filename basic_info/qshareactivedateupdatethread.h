﻿#ifndef QSHAREACTIVEDATEUPDATETHREAD_H
#define QSHAREACTIVEDATEUPDATETHREAD_H

#include <QThread>

class QShareActiveDateUpdateThread : public QThread
{
    Q_OBJECT
public:
    explicit QShareActiveDateUpdateThread(QObject *parent = 0);
public:
    void run();

signals:

public slots:
};

#endif // QSHAREACTIVEDATEUPDATETHREAD_H