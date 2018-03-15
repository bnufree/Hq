#ifndef QEASTMONEYBLOCKSHARETHREAD_H
#define QEASTMONEYBLOCKSHARETHREAD_H

#include <QObject>
#include <QRunnable>

class QEastMoneyBlockShareThread : public QRunnable
{
public:
    explicit QEastMoneyBlockShareThread(const QString& pBlockCode, QObject *parent = 0);
    ~QEastMoneyBlockShareThread();
public:
    void    run();
private:
    QString     mBlockCode;
    QObject*    mParent;
};

#endif // QEASTMONEYBLOCKSHARETHREAD_H
