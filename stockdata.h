#ifndef STOCKDATA_H
#define STOCKDATA_H
#include <QStringList>
#include <QDate>
#include <QList>
#include <QObject>
#include "block/blockdata.h"

struct zjlxData{
    QString code;
    double zjlx;
};

class StockBaseData
{
public:
    StockBaseData()
    {
        mName = "";
        mCode = "";
        mCur = 0;
        mChg = 0;
        mChgPercent = 0;
        mHigh = 0;
        mLow = 0.0;
        mOpen = 0.0;
        mClose = 0.0;
        mVol = 0.0;
        mMoney = 0.0;
        mTotalShare = 0;
        mMutableShare = 0;
        mZJLX = 0.0;
        mRZRQ = 0.0;
        mHSGTTop10Money = 0.0;
        mHSGTTop10Vol = 0;
        mHSGTTop10 = false;
        mHSGTEstimateFlag = false;
        mDate = QDate(2017,1,1);
        mIsFavCode = false;
    }

public:
    QString         mName;
    QString         mCode;
    double          mCur;
    double          mChg;
    double          mChgPercent;
    double          mHigh;
    double          mLow;
    double          mOpen;
    double          mClose;
    qint64          mVol;
    double          mMoney;
    qint64          mTotalShare;
    qint64          mMutableShare;
    QDate           mDate;
    double          mZJLX;
    double          mRZRQ;
    double          mHSGTTop10Money;
    qint64          mHSGTTop10Vol;
    bool            mHSGTTop10;
    bool            mHSGTEstimateFlag;
    bool            mIsFavCode;
    QTime           mUpdateTime;
};

class StockData : public StockBaseData
{
public:
    StockData();
    ~StockData();
    static bool sortByPerDesc(const StockData& d1, const StockData& d2);
    static bool sortByPerAsc(const StockData& d1, const StockData& d2);
    static bool sortByCurDesc(const StockData& d1, const StockData& d2);
    static bool sortByCurAsc(const StockData& d1, const StockData& d2);
    static bool sortByMonDesc(const StockData& d1, const StockData& d2);
    static bool sortByMonAsc(const StockData& d1, const StockData& d2);
    static bool sortByHslDesc(const StockData& d1, const StockData& d2);
    static bool sortByHslAsc(const StockData& d1, const StockData& d2);
    static bool sortByMonRatioDesc(const StockData& d1, const StockData& d2);
    static bool sortByMonRatioAsc(const StockData& d1, const StockData& d2);
    static bool sortBy3DayChgDesc(const StockData& d1, const StockData& d2);
    static bool sortBy3DayChgAsc(const StockData& d1, const StockData& d2);
    static bool sortBy3DayZjlxDesc(const StockData& d1, const StockData& d2);
    static bool sortBy3DayZjlxAsc(const StockData& d1, const StockData& d2);
    static bool sortByGxlDesc(const StockData& d1, const StockData& d2);
    static bool sortByGxlAsc(const StockData& d1, const StockData& d2);
    static bool sortBySzzblDesc(const StockData& d1, const StockData& d2);
    static bool sortBySzzblAsc(const StockData& d1, const StockData& d2);
    static bool sortByGqdjrDesc(const StockData& d1, const StockData& d2);
    static bool sortByGqdjrAsc(const StockData& d1, const StockData& d2);
    static bool sortByTcapDesc(const StockData& d1, const StockData& d2);
    static bool sortByTcapAsc(const StockData& d1, const StockData& d2);
    static bool sortByMcapDesc(const StockData& d1, const StockData& d2);
    static bool sortByMcapAsc(const StockData& d1, const StockData& d2);
    static bool sortByProfitDesc(const StockData& d1, const StockData& d2);
    static bool sortByProfitAsc(const StockData& d1, const StockData& d2);
    static bool sortByForVolDesc(const StockData& d1, const StockData& d2);
    static bool sortByForVolAsc(const StockData& d1, const StockData& d2);
    static bool sortByForCapDesc(const StockData& d1, const StockData& d2);
    static bool sortByForCapAsc(const StockData& d1, const StockData& d2);

    static bool sortByForVolChgDesc(const StockData& d1, const StockData& d2);
    static bool sortByForVolChgAsc(const StockData& d1, const StockData& d2);
    static bool sortByForCapChgDesc(const StockData& d1, const StockData& d2);
    static bool sortByForCapChgAsc(const StockData& d1, const StockData& d2);

    bool operator ==(const StockData& data)
    {
        return this->mCode == data.mCode;
    }

public:
    double          mMax;
    double          mMin;
    double          mMoney;
    double          mHsl;
    double          mLastMoney;
    qint64          mLastVol;
    double          mLastChgPer;
    double          mLast3DaysChgPers;
    double          mLast5DaysChgPers;
    double          mLast10DaysChgPers;
    double          mLastMonthChgPers;
    double          mChgPersFromYear;
    double          mChgPersFromWeek;
    double          mChgPersFromMonth;
    double          mLastClose;
    double          mMoneyRatio;
    BlockDataList     mBlockList;
    QStringList       mBlockCodeList;
    double          mZJLX;      //资金流向
    double          mSZZG; //送转股比例
    double          mXJFH;  //现金分红
    double          mGXL;   //股息率
    QDate           mGQDJR; //股权登记日
    QDate           mYAGGR; //预案公告日
    double          mTotalCap;
    double          mMutalbleCap;
    bool            mIndexFlag;
    double          mProfit;
    qint64          mForeignVol;
    qint64          mForeignVolChg;
    double          mForeignCap;
    double          mForeignCapChg;
};

Q_DECLARE_METATYPE(StockData)

typedef QList<StockData> StockDataList;
#endif // STOCKDATA_H
