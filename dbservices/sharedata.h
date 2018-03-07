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

//定义基础的数据类型
typedef enum data_type{
    DATA_UNDEFINED = 0x00,
    DATA_SHARE = 0x10,
    DATA_BLOCK = 0x20,
    DATA_RESERVED = 0x30,
}DATA_TYPE;

class BaseData
{
public:
    inline BaseData()
    {
        mIsFav = 0;
        mDataType = DATA_UNDEFINED;
    }
    inline BaseData(const QString& code, const QString& name, const QString& abbr, int type)
    {
        mIsFav = 0;
        mDataType = type;
        memcpy(mCode, code.toStdString().data(), 10);
        memcpy(mName, name.toStdString().data(), 20);
        memcpy(mPY, abbr.toStdString().data(), 10);
    }

    void setFav(bool fav)
    {
        mIsFav = fav;
    }

    void setCode(const QString& code)
    {
        memcpy(mCode, code.toStdString().data(), 10);
    }

    void setName(const QString& name)
    {
        memcpy(mName, name.toStdString().data(), 20);
    }

    void setPY(const QString& abbr)
    {
        memcpy(mPY, abbr.toStdString().data(), 10);
    }

public:
    bool        mIsFav;
    int         mDataType;
    char        mCode[10];
    char        mName[20];
    char        mPY[10];
};

typedef    enum     share_type
{
    SHARE_UNDEFINED = DATA_SHARE,
    SHARE_CHINA_SH = DATA_SHARE + 1,
    SHARE_CHINA_SZ,
    SHARE_CHINA_FUND_SH,
    SHARE_CHINA_FUND_SZ,
    SHARE_INDEX_SH,
    SHARE_INDEX_SZ,
    SHARE_INDEX_HK,
    SHARE_INDEX_US,
    SHARE_HK,
    SHARE_US,
}SHARE_TYPE;

class ShareBaseData : public BaseData
{
public:
    inline ShareBaseData(const QString& code = QString(), const QString& name= QString(), const QString& abbr= QString(), int type = SHARE_CHINA_SH):BaseData(code, name, abbr, type)
    {
        mIsTop10 = 0; //CHINA HK
        mTotalShare = 0;
        mMutalShare = 0;
        mMGSY = 0.0;
        mMGJZC = 0.0;
        mJZCSYL = 0.0;
        mSZZG = 0.0;
        mXJFH = 0.0;
        mGQDJR = 0;
        mYAGGR = 0;
        mProfit = 0.0;
    }

public:
    //基本信息
    bool            mIsTop10;
    double          mProfit;
    qint64          mTotalShare;
    qint64          mMutalShare;
    //财务信息
    double          mMGSY;      //每股收益
    double          mMGJZC;     //每股净资产
    double          mJZCSYL;    //净资产收益率
    //分红信息
    double          mSZZG; //送转股比例
    double          mXJFH;  //现金分红
    qint64          mGQDJR; //股权登记日
    qint64          mYAGGR; //预案公告日
};

typedef QList<ShareBaseData>     ShareBaseDataList;

class ShareData : public ShareBaseData
{
public:
    ShareData():ShareBaseData()
    {

    }

    ShareData(const QString& code, const QDate& date):ShareBaseData(code)
    {
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
    double          mZJLX;      //资金流向
    double          mGXL;   //股息率
    double          mTotalCap;
    double          mMutalbleCap;
    qint64          mForeignVol;
    qint64          mForeignVolChg;
    double          mForeignCap;
    double          mForeignCapChg;
    double          mCur;
    double          mChg;
    double          mChgPercent;
    double          mHigh;
    double          mLow;
    double          mOpen;
    double          mClose;
    double          mMoney;
    double          mRZRQ;
    qint64          mVol;
    qint64          mUpdateTime;
    QDate           mDate;
    BlockDataList     mBlockList;
    QStringList       mBlockCodeList;

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
            ShareData data("UNDEF", date);
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
