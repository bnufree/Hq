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

struct ShareData;


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

protected slots:
    void run();

public slots:
    void        setMktType(int type);
    void        setSortType(int type);
    void        setActive(bool active);
    void        setSelfCodesList(const QStringList& list );
    void        setDisplayPage(int val);
    void        setDisplayChinaTop10();
    void        setShareCodes(const QStringList& list);

signals:
    void    sendStkDataList(const ShareDataList& list, qint64 time);
    void    sendStkinfoUpdateProgress(int cur, int total);
private:
    MKT_TYPE                                                mMktType;
    QList<QStringList>                                      mSecCodesList;
    QMutex                                                  mSecCodesLock;
    bool                                                    mActive;
    QMutex                                                  mListMutex;
    QStringList                                             mSelfCodesList;
    QMap<QString, double>                                   mZjlxMaplist;
    int                                                     mStkCntPerTrd;
    bool                                                    mIsCodeChg;
    QStringList                                             mAllShareCodesList;
    QMutex                                                  mCodeMutex;
    bool                                                    mParamChange;
    QList<QSinaStkInfoThread*>                              mSubThreadList;
//    ShareDataList                                           mResDataList;
};

#endif // QSINASTKRESULTMERGETHREAD_H
