#ifndef QSHAREHISTORYCOUNTERWORK_H
#define QSHAREHISTORYCOUNTERWORK_H

#include <QRunnable>
#include <QObject>
#include "data_structure/sharedata.h"


enum LGT_DATA_TYPE{
    LGT_ALL = 0,
    LGT_DAY_1 = 1,
    LGT_DAY_3,
    LGT_DAY_5,
    LGT_DAY_10,
    LGT_MONTH,
    LGT_SEASON,
    LGT_YEAR,
};

class QShareLGTHistoryCounterWork : public QRunnable
{
public:
    QShareLGTHistoryCounterWork(const QDate& date, int type, QObject* parent = 0);
    ~QShareLGTHistoryCounterWork() {}
    void run();
    QString getFileName();
    bool readFromHisrotyFile();

private:
    QMap<int,   ShareHsgtCounter> mDataMap;
    QObject*    mParent;
    QDate       mDate;
    int         mType;
    QString     mFilePath;
    QString     mFileName;
};

class QShareHistoryCounterWork : public QRunnable
{
public:
    QShareHistoryCounterWork(const QString& code, const ShareDailyDataList& list = ShareDailyDataList(), QObject* parent = 0);
    ~QShareHistoryCounterWork();
    void run();
    QString getFileName();
    bool readFile(ShareDailyDataList& list);
private:
    ShareDailyDataList mList;
    QObject*    mParent;
    QString     mCode;
};

#endif // QSHAREHISTORYCOUNTERWORK_H
