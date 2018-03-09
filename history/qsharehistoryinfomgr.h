#ifndef QSHAREHISTORYINFOMGR_H
#define QSHAREHISTORYINFOMGR_H

#include <QObject>
#include <QThread>
#include <QStringList>
#include <QMap>
#include <QMutex>
#include <QThreadPool>
#include <QDate>

class QEastmoneyShareHistoryInfoThread;
class ShareDataList;

class QShareHistoryInfoMgr : public QObject
{
    Q_OBJECT
public:
    explicit QShareHistoryInfoMgr(const QStringList& codes, QObject *parent = 0);
    bool isWorking();
    ~QShareHistoryInfoMgr();

signals:
    void signalGetCodesFinished(const QStringList& list);
    void signalSetHistoryCodeList(const QStringList& list);
    void signalStartGetHistory();
    void signalHistoryDataFinished();
    void signalUpdateProcess(int cur, int total);
    void signalUpdateAmountProcess(const QString& date);
    void signalUpdateAllShareFromDate(bool deldb, const QDate& date);
    void signalUpdateHistoryMsg(const QString& msg);
    void signalUpdateHistoryFinished();

public slots:
    void slotStartGetHistory();
    void slotUpdateAllShareFromDate(bool deldb, const QDate& date = QDate(2017,3,17));
    void slotUpdateForignVolInfo(const ShareDataList& list, const QDate& date);
    void slotUpdateShareHistoryProcess(const QString& code);
    void slotDbUpdateHistoryFinished();
    void slotUpdateShareHistoryInfoFinished(const QString& code);
private:
    QThread             mWorkThread;
    QStringList         mCodesList;
    QMap<QString, ShareDataList>    mShareInfoMap;
    QMutex              mShareInfoMutex;
    QMutex              mShareHistoryMutex;
    int                 mCurCnt;
    QThreadPool         mPool;
    QDate               mLastUpdateDate;
};

#endif // QSHAREHISTORYINFOMGR_H
