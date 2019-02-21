#ifndef QSINASTKRESULTMERGETHREAD_H
#define QSINASTKRESULTMERGETHREAD_H

#include <QThread>
//#include "dbservices/sharedata.h"
#include <QStringList>
#include <QWaitCondition>
#include <QMap>
#include <QMutex>
#include "qeastmoneyzjlxthread.h"
#include "qsinastkinfothread.h"

class ShareData;


class QSinaStkResultMergeThread : public QThread
{
    Q_OBJECT
public:
    explicit QSinaStkResultMergeThread(QObject *parent = 0);
    ~QSinaStkResultMergeThread();
public:
    bool isActive();
    MktType getMktType();
    void setCurPage(int page);
    void displayFirst();
    void displayNext();
    void displayPrevious();
    void displayLast();
signals:

protected:
    void run();

public slots:
    void        setStkList(const QStringList& list);
    void        stopUpdate();
    void        setMktType(int type);
    void        setSortType(int type);
    void        setActive(bool active);
    void        setSelfCodesList(const QStringList& list );
    void        setDisplayPage(int val);
    void        setDisplayChinaTop10();
private slots:
    void        updateStkCodes(MKT_TYPE type);
    void        updateStkInfoList(const QList<QStringList>& pStkSectionList);
    //StockDataList   RealtimeInfo(const QStringList& codes);
    void            SortResultList(ShareDataList& result, const ShareDataList& mid);
    void         slotRevResList(const ShareDataList& mid);

signals:
    void    sendStkDataList(const ShareDataList& list);
    void    sendStkinfoUpdateProgress(int cur, int total);
private:
    QStringList                                             mStkCodesList;
    int                                                     mTotalPage;
    int                                                     mCurPage;
    int                                                     mPageSize;
    int                                                     mSortType;
    int                                                     mSortRule;
    MKT_TYPE                                                mMktType;
    QList<QSinaStkInfoThread*>                              mThreadList;
    QList<QStringList>                                      mSecCodesList;
    QMutex                                                  mSecCodesLock;
    QMap<QString, ShareData>                                mMidStkDataMapList;
    bool                                                    mActive;
    QMutex                                                  mListMutex;
    QStringList                                             mSelfCodesList;
    QMap<QString, double>                                   mZjlxMaplist;
    int                                                     mStkCntPerTrd;
    bool                                                    mCodeChangeFlag;
};

#endif // QSINASTKRESULTMERGETHREAD_H
