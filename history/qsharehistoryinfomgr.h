#ifndef QSHAREHISTORYINFOMGR_H
#define QSHAREHISTORYINFOMGR_H

#include <QObject>
#include <QThread>
#include <QStringList>
#include <QMap>
#include <QMutex>
#include <QThreadPool>
#include <QDate>
#include "data_structure/sharedata.h"



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
    void signalStartStatic();

public slots:
    void slotUpdateForignVolInfo(const ShareForignVolFileDataList& list, const QDate& date);
    void slotUpdateShareHistoryProcess(const ShareHistoryCounter& list, const QString& code);
    void slotStartGetHistoryWithAllCodes();
    void slotStartStatics();
private:

private:
    QThread             mWorkThread;
    QStringList         mCodesList;
    QMutex              mForeignHistoryMutex;
    QMutex              mShareHistoryMutex;
    QThreadPool         mPool;
    QMap<QString,       ShareHistoryCounter>  mShareInfoHistoryMap;
    QMap<int,         QMap<QDate, ShareForignVolFileData> >         mShareForeignDataMap;
};

#endif // QSHAREHISTORYINFOMGR_H
