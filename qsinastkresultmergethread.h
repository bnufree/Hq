#ifndef QSINASTKRESULTMERGETHREAD_H
#define QSINASTKRESULTMERGETHREAD_H

#include <QThread>
#include "stockdata.h"
#include <QStringList>
#include <QWaitCondition>
#include <QMap>
#include <QMutex>
#include "qeastmoneyzjlxthread.h"

typedef enum MktType
{
    MKT_ALL,        //沪深全部
    MKT_SH,         //上海
    MKT_SZ,         //深圳
    MKT_ZXB,        //中小板
    MKT_CYB,        //创业板
    MKT_JJ,         //沪深基金
    MKT_HK_HSZS,    //恒生指数
    MKT_HK_HSGQ,    //恒生国企
    MKT_HK_GGT,        //港股通（）
    MKT_ZXG,        //自选股
    MKT_OTHER,      //保留
}MKT_TYPE;


class QSinaStkResultMergeThread : public QThread
{
    Q_OBJECT
public:
    explicit QSinaStkResultMergeThread(QObject *parent = 0);
    ~QSinaStkResultMergeThread();
    bool isActive();
    MktType getMktType();
signals:

protected:
    void run();

public slots:
    void        setStkList(const QStringList& list);
    void        stopUpdate();
    void        setMktType(MKT_TYPE type);
    void        setSortType(int type);
    void        setActive(bool active);
    void        setSelfCodesList(const QStringList& list );
private slots:
    void        updateStkCodes(MKT_TYPE type);
    void        updateStkInfoList(const QList<QStringList>& pStkSectionList);
    StockDataList   RealtimeInfo(const QStringList& codes);
    void            SortResultList(StockDataList& result, const StockDataList& mid);
    void         slotRevResList(const StockDataList& mid);
    void         slotRevZjlxData(const QList<zjlxData>& zjlist);

signals:
    void    sendStkDataList(const StockDataList& list);
    void    sendStkinfoUpdateProgress(int cur, int total);
private:
    QStringList         mStkCodesList;
    int                 mTotalPage;
    int                 mCurPage;
    int                 mPageSize;
    int                 mSortType;
    int                 mSortRule;
    MKT_TYPE            mMktType;
    QList<QThread*>     mThreadList;
    QList<QStringList>  mSecCodesList;
    QMutex              mSecCodesLock;
    QMap<QString, StockData>    mMidStkDataMapList;
    bool                mActive;
    QMutex              mListMutex;
    QStringList         mSelfCodesList;
    QMap<QString, double> mZjlxMaplist;
};

#endif // QSINASTKRESULTMERGETHREAD_H
