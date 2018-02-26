#ifndef QSHAREHISTORYINFOMGR_H
#define QSHAREHISTORYINFOMGR_H

#include <QObject>
#include <QThread>
#include <QStringList>
#include <stockdata.h>
#include <QMap>
#include <QMutex>
#include <QThreadPool>

class QEastmoneyStockHistoryInfoThread;

class QShareHistoryInfoMgr : public QObject
{
    Q_OBJECT
public:
    explicit QShareHistoryInfoMgr(const QStringList& codes, QObject *parent = 0);
    bool isWorking();
    ~QShareHistoryInfoMgr();

signals:
    void signalGetFianceInfo();
    void signalGetCodesFinished(const QStringList& list);
    void signalSetHistoryCodeList(const QStringList& list);
    void signalStartGetHistory();
    void signalHistoryDataFinished();
    void signalUpdateProcess(int cur, int total);
    void signalUpdateAmountProcess(const QString& date);
    void signalUpdateAllShareFrom20170317();
    void signalUpdateHistoryMsg(const QString& msg);

public slots:
    void slotGetFinanceInfo();
    void slotShareFinanceInfoFinished();
    void slotUpdateAllShareFromLastUpdateDate();
    void slotUpdateForignVolInfo(const StockDataList& list, const QDate& date);
    void slotUpdateShareHistoryProcess(const QString& code);
private:
    QThread             mWorkThread;
    QStringList         mCodesList;
    QMap<QString, StockDataList>    mShareInfoMap;
    QMutex              mShareInfoMutex;
    QMutex              mShareHistoryMutex;
    int                 mCurCnt;
    QThreadPool         mPool;
    QDate               mLastUpdateDate;
};

#endif // QSHAREHISTORYINFOMGR_H
