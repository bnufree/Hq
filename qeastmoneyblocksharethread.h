#ifndef QEASTMONEYBLOCKSHARETHREAD_H
#define QEASTMONEYBLOCKSHARETHREAD_H

#include <QObject>
#include <QThread>

class QEastMoneyBlockShareThread : public QThread
{
    Q_OBJECT
public:
    explicit QEastMoneyBlockShareThread(const QString& pBlockCode, QObject *parent = 0);
    ~QEastMoneyBlockShareThread();

signals:
    void    signalUpdateBlockShareCodeList(const QString& pBlockCode, const QStringList& pShareCodesList);
    void    signalUpdateShareBlock(const QString& share, const QString& block);
protected:
    void    run();
private:
    QString     mBlockCode;
};

#endif // QEASTMONEYBLOCKSHARETHREAD_H
