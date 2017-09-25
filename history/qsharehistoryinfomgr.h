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
    explicit QShareHistoryInfoMgr(QObject *parent = 0);
    QStringList  getCodesList();

signals:
    void signalSetHistoryCodeList(const QStringList& list);
    void signalStartGetHistory();
    void signalHistoryDataFinished();
    void signalUpdateProcess(int cur, int total);
    void signalUpdateAmountProcess(const QString& date);

public slots:
    void slotSetHistoryCodeList(const QStringList& list);
    void slotRecvCodeHistoryDate(const QString& code, const QDate& date);
    void slotSubThreadFinish();
    void slotStartGetHistory();
    void slotUpdateHistoryFinished(const QString& code);
    void slotUpdateStockCodesList(const QStringList &list);
    void slotUpdateForeignAmountFinished();
private:
    QThread     mWorkThread;
    QList<QEastmoneyStockHistoryInfoThread*>     mWorkQueueThreadList;
    QList<QEastmoneyStockHistoryInfoThread*>     mWorkingThreadList;
    QStringList         mCodesList;
    int                 mRecvCodeNum;
};

#endif // QSHAREHISTORYINFOMGR_H
