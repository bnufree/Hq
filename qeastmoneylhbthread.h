#ifndef QEASTMONEYLHBTHREAD_H
#define QEASTMONEYLHBTHREAD_H

#include <QObject>

class QEastMoneyLHBThread : public QObject
{
    Q_OBJECT
public:
    explicit QEastMoneyLHBThread(QObject *parent = 0);
    ~QEastMoneyLHBThread();

signals:

public slots:
};

#endif // QEASTMONEYLHBTHREAD_H
