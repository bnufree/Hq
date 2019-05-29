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

class QEastmoneyShareHistoryInfoThread;

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
    void slotUpdateForignVolInfo(const ShareDataList& list);
    void slotUpdateShareHistoryProcess(const ShareDataList& list);
    void slotDbUpdateHistoryFinished();
    void slotUpdateShareHistoryInfoFinished(const QString& code);
    void slotUpdateReadHistoryInfo(const ShareDataList& list);
    void slotUpdateWriteHistoryInfo(const ShareDataList& list);

private:
    QThread             mWorkThread;
    QStringList         mCodesList;
    QMap<int, QMap<qint64, qint64> >    mShareInfoMap;  //code   date   vol
    QMutex              mShareInfoMutex;
    QMutex              mShareHistoryMutex;
    int                 mCurCnt;
    QThreadPool         mPool;
    QDate               mLastUpdateDate;
    QMap<QString,       ShareData>  mShareInfoHistoryMap;
    int                 mHistoryFileNum;
    int                 mCountCodeNum;
    QStringList         mDates;
};

#endif // QSHAREHISTORYINFOMGR_H
