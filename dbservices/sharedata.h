#ifndef SHAREDATA_H
#define SHAREDATA_H
#include <QStringList>
#include <QDate>
#include <QList>
#include <QObject>
#include <QMap>
#include <QDebug>
#include "block/blockdata.h"

struct zjlxData{
    QString code;
    double zjlx;
};

class BaseData
{
public:
    inline BaseData()
    {
        isfav = 0;
        ishsgt = 0;
        total = 0;
        mutal = 0;
        code = 0;
    }

public:
    bool     isfav;
    bool     ishsgt;
    int      code;
    qint64   total;
    qint64   mutal;
    char     name[20];
    char     abbr[10];
};

typedef QList<BaseData>     BaseDataList;

class ShareBaseData
{
public:
    ShareBaseData()
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
    QString         mPY;
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

class ShareData : public ShareBaseData
{
public:
    ShareData();
    ShareData(const QString& code, const QDate& date):ShareBaseData()
    {
        mCode = code;
        mDate = date;
    }

    ~ShareData();
    static bool sortByPerDesc(const ShareData& d1, const ShareData& d2);
    static bool sortByPerAsc(const ShareData& d1, const ShareData& d2);
    static bool sortByCurDesc(const ShareData& d1, const ShareData& d2);
    static bool sortByCurAsc(const ShareData& d1, const ShareData& d2);
    static bool sortByMonDesc(const ShareData& d1, const ShareData& d2);
    static bool sortByMonAsc(const ShareData& d1, const ShareData& d2);
    static bool sortByHslDesc(const ShareData& d1, const ShareData& d2);
    static bool sortByHslAsc(const ShareData& d1, const ShareData& d2);
    static bool sortByMonRatioDesc(const ShareData& d1, const ShareData& d2);
    static bool sortByMonRatioAsc(const ShareData& d1, const ShareData& d2);
    static bool sortBy3DayChgDesc(const ShareData& d1, const ShareData& d2);
    static bool sortBy3DayChgAsc(const ShareData& d1, const ShareData& d2);

    static bool sortBy5DayChgDesc(const ShareData& d1, const ShareData& d2);
    static bool sortBy5DayChgAsc(const ShareData& d1, const ShareData& d2);

    static bool sortBy10DayChgDesc(const ShareData& d1, const ShareData& d2);
    static bool sortBy10DayChgAsc(const ShareData& d1, const ShareData& d2);

    static bool sortByMonthChgDesc(const ShareData& d1, const ShareData& d2);
    static bool sortByMonthChgAsc(const ShareData& d1, const ShareData& d2);

    static bool sortByYearChgDesc(const ShareData& d1, const ShareData& d2);
    static bool sortByYearChgAsc(const ShareData& d1, const ShareData& d2);

    static bool sortBy3DayZjlxDesc(const ShareData& d1, const ShareData& d2);
    static bool sortBy3DayZjlxAsc(const ShareData& d1, const ShareData& d2);
    static bool sortByGxlDesc(const ShareData& d1, const ShareData& d2);
    static bool sortByGxlAsc(const ShareData& d1, const ShareData& d2);
    static bool sortBySzzblDesc(const ShareData& d1, const ShareData& d2);
    static bool sortBySzzblAsc(const ShareData& d1, const ShareData& d2);
    static bool sortByGqdjrDesc(const ShareData& d1, const ShareData& d2);
    static bool sortByGqdjrAsc(const ShareData& d1, const ShareData& d2);
    static bool sortByTcapDesc(const ShareData& d1, const ShareData& d2);
    static bool sortByTcapAsc(const ShareData& d1, const ShareData& d2);
    static bool sortByMcapDesc(const ShareData& d1, const ShareData& d2);
    static bool sortByMcapAsc(const ShareData& d1, const ShareData& d2);
    static bool sortByProfitDesc(const ShareData& d1, const ShareData& d2);
    static bool sortByProfitAsc(const ShareData& d1, const ShareData& d2);
    static bool sortByForVolDesc(const ShareData& d1, const ShareData& d2);
    static bool sortByForVolAsc(const ShareData& d1, const ShareData& d2);
    static bool sortByForCapDesc(const ShareData& d1, const ShareData& d2);
    static bool sortByForCapAsc(const ShareData& d1, const ShareData& d2);

    static bool sortByForVolChgDesc(const ShareData& d1, const ShareData& d2);
    static bool sortByForVolChgAsc(const ShareData& d1, const ShareData& d2);
    static bool sortByForCapChgDesc(const ShareData& d1, const ShareData& d2);
    static bool sortByForCapChgAsc(const ShareData& d1, const ShareData& d2);

    bool operator ==(const ShareData& data)
    {
        return this->mCode == data.mCode && this->mDate == data.mDate;
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
    double          mMGSY;      //每股收益
    double          mMGJZC;     //每股净资产
    double          mJZCSYL;    //净资产收益率
};

Q_DECLARE_METATYPE(ShareData)

class ShareDataList : public QList<ShareData>
{
public:
    inline ShareDataList() {}
    inline ShareDataList(const QList<ShareData>& list)
    {
        foreach (ShareData data, list) {
            append(data);
        }
    }

    ShareData &valueOfDate(const QDate& date)
    {
        if(!mDataIndexMap.contains(date))
        {
            ShareData data;
            data.mDate = date;
            data.mCode = "UNDEF";
            append(data);
        }
        //qDebug()<<date<<mDataIndexMap.value(date)<<this->size();
        return (*this)[mDataIndexMap.value(date)];
    }

    void append(const ShareData& data)
    {
        if(this->contains(data)) return;
        QList<ShareData>::append(data);
        mDataIndexMap[data.mDate] = this->size() - 1;
    }

    ShareDataList& operator =(const QList<ShareData>& list)
    {
        foreach (ShareData data, list) {
            append(data);
        }

        return (*this);
    }

private:
    QMap<QDate, int>        mDataIndexMap;
};

#endif // SHAREDATA_H
