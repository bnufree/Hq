#ifndef QSHAREHSGTHISTORYCHANGETHREAD_H
#define QSHAREHSGTHISTORYCHANGETHREAD_H

#include <QObject>
#include <QMap>
#include "data_structure/sharedata.h"

enum HSGT_CHG_MODE{
    HSGT_CHG_1DAY = 0,
    HSGT_CHG_3DAY,
    HSGT_CHG_5DAY,
    HSGT_CHG_10DAY,
    HSGT_CHG_MONTH,
    HSGT_CHG_SEASON,
    HSGT_CHG_YEAR,
};

//struct ShareHSGTChgData{
//    double      mVolChg;
//    double      mVolToalChgPercent;
//    double      mVolMutalChangePercent;
//};

//struct ShareHSGTHistoryData{
//    QString     mCode;
//    QString     mName;
//    QString     mDate;
//    double      mCurPrice;
//    double      mChgPercent;
//    double      mTotalShareMktCap;
//    double      mMutalShareMktCap;
//    QString     mHYCode;
//    int         mJGNum;
//    double      mJGShareRate;
//    double      mNowHSGTTotalShare;
//    double      mNowHSGTTotalSharePercent;
//    double      mNowHSGTMutalSharePercent;

//    QMap<QString, ShareHSGTChgData> mCounterMap;
//};

class QShareHSGTHistoryChangeThread : public QObject
{
    Q_OBJECT
public:
    explicit QShareHSGTHistoryChangeThread(QObject *parent = 0);
    void run();

signals:

public slots:
};

#endif // QSHAREHSGTHISTORYCHANGETHREAD_H
