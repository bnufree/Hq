#ifndef QSHAREHISTORYINFOMGR_H
#define QSHAREHISTORYINFOMGR_H

#include <QObject>
#include <QThread>
#include <QStringList>

class QEastmoneyStockHistoryInfoThread;

class QShareHistoryInfoMgr : public QObject
{
    Q_OBJECT
public:
    explicit QShareHistoryInfoMgr(const QStringList& codes, QObject *parent = 0);

signals:
    void signalGetCodesFinished(const QStringList& list);
    void signalSetHistoryCodeList(const QStringList& list);
    void signalStartGetHistory();
    void signalHistoryDataFinished();
    void signalUpdateProcess(int cur, int total);
    void signalUpdateAmountProcess(const QString& date);

public slots:
    void slotShareFinanceInfoFinished();
private:
    QThread             mWorkThread;
    QStringList         mCodesList;
};

#endif // QSHAREHISTORYINFOMGR_H
