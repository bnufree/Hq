#ifndef SHAREDATA_H
#define SHAREDATA_H
#include <QStringList>
#include <QDate>
#include <QList>
#include <QObject>
#include <QMap>
#include <QDebug>
#include <QRegExp>
#include "blockdata.h"

struct zjlxData{
    QString code;
    double zjlx;
};

#define         SH_FUND_REG         "(sh){0,1}5[0-9]{5}"
#define         SH_INDEX_REG        "sh0[0-9]{5}"
#define         SZ_FUND_REG         "(sz){0,1}1[0-9]{5}"
#define         SZ_INDEX_REG        "sz399[0-9]{3}"
#define         SH_SHARE_REG        "(sh){0,1}6[0-9]{5}"
#define         SZZB_SHARE_REG        "(sz){0,1}(00[01]{1}[0-9]{3})"
#define         SZZXB_SHARE_REG        "(sz){0,1}(002[0-9]{3})"
#define         SZCYB_SHARE_REG        "(sz){0,1}(30[0-9]{4})"
#define         HK_SHARE_REG        "(hk){0,1}\\d{5}"
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
    inline BaseData(bool isfav, int dataType, const char* code, const char* name, const char* py)
    {
        this->mIsFav = isfav;
        this->mDataType = dataType;
        memcpy(this->mCode, code, 10);
        memcpy(this->mName, name, 20);
        memcpy(this->mPY, py, 10);
    }

    inline BaseData(const BaseData& data)
    {
        this->mIsFav = data.mIsFav;
        this->mDataType = data.mDataType;
        memcpy(this->mCode, data.mCode, 10);
        memcpy(this->mName, data.mName, 20);
        memcpy(this->mPY, data.mPY, 10);
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
    SHARE_UNDEFINED = 0,
    SHARE_CHINA_SH = 1,
    SHARE_CHINA_SZ_ZB,
    SHARE_CHINA_SZ_ZXB,
    SHARE_CHINA_SZ_CYB,
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
    inline ShareBaseData(const ShareBaseData& data):\
        BaseData(data.mIsFav, data.mDataType, data.mCode, data.mName, data.mPY)
    {
        mShareType = data.mShareType;
        mIsTop10 = data.mIsTop10;
        mTotalShare = data.mTotalShare;
        mMutalShare = data.mMutalShare;
        mMGSY = data.mMGSY;
        mMGJZC = data.mMGJZC;
        mJZCSYL = data.mJZCSYL;
        mSZZG = data.mSZZG;
        mXJFH = data.mXJFH;
        mGQDJR = data.mGQDJR;
        mYAGGR = data.mYAGGR;
        mProfit = data.mProfit;
        mTop10Buy = data.mTop10Buy;
        mTop10Sell = data.mTop10Sell;
    }

    inline ShareBaseData(const QString& code = QString(), \
                         const QString& name= QString(), \
                         const QString& abbr= QString())\
        :BaseData(code, name, abbr, DATA_SHARE)
    {
        mShareType = SHARE_UNDEFINED;
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
        mTop10Buy = 0;
        mTop10Sell = 0;
    }

    void setShareType(SHARE_TYPE type)
    {
        mShareType = type;
    }

    QString shareTypeString()
    {
        QString res = QStringLiteral("未定义");
        switch (mShareType) {
        case SHARE_CHINA_SH:
            res = QStringLiteral("上证A股");
            break;
        case SHARE_CHINA_SZ_ZB:
            res = QStringLiteral("深证主板");
            break;
        case SHARE_CHINA_SZ_ZXB:
            res = QStringLiteral("深证中小");
            break;
        case SHARE_CHINA_SZ_CYB:
            res = QStringLiteral("深证创业");
            break;
        case SHARE_INDEX_SH:
            res = QStringLiteral("上证指数");
            break;
        case SHARE_INDEX_SZ:
            res = QStringLiteral("深圳指数");
            break;
        case SHARE_CHINA_FUND_SH:
            res = QStringLiteral("上证基金");
            break;
        case SHARE_CHINA_FUND_SZ:
            res = QStringLiteral("深圳基金");
            break;
        case SHARE_HK:
            res = QStringLiteral("港股");
            break;
        default:
            break;
        }

        return res;
    }

    //通过证券的代码来获取证券的类型(不包括指数,指数需要强制指定)
    static SHARE_TYPE shareType(const QString &src)
    {
        QRegExp shShare(SH_SHARE_REG);
        QRegExp szZBShare(SZZB_SHARE_REG);
        QRegExp szZXBShare(SZZXB_SHARE_REG);
        QRegExp szCYBShare(SZCYB_SHARE_REG);
        QRegExp hkShare(HK_SHARE_REG);
        QRegExp shFund(SH_FUND_REG);
        QRegExp szFund(SZ_FUND_REG);
        QRegExp shIndex(SH_INDEX_REG);
        QRegExp szIndex(SZ_INDEX_REG);

        if(shShare.exactMatch(src)) return SHARE_CHINA_SH;
        if(szZBShare.exactMatch(src)) return SHARE_CHINA_SZ_ZB;
        if(szZXBShare.exactMatch(src)) return SHARE_CHINA_SZ_ZXB;
        if(szCYBShare.exactMatch(src)) return SHARE_CHINA_SZ_CYB;
        if(shIndex.exactMatch(src)) return SHARE_INDEX_SH;
        if(szIndex.exactMatch(src)) return SHARE_INDEX_SZ;
        if(shFund.exactMatch(src)) return SHARE_CHINA_FUND_SH;
        if(szFund.exactMatch(src)) return SHARE_CHINA_FUND_SZ;
        if(hkShare.exactMatch(src)) return SHARE_HK;
        return SHARE_UNDEFINED;
    }

    static QString prefixCode(const QString&  code)
    {
        SHARE_TYPE type = shareType(code);
        if(type == SHARE_CHINA_SH || type == SHARE_CHINA_FUND_SH) return "sh";
        if(type == SHARE_CHINA_SZ_ZB || type == SHARE_CHINA_SZ_ZXB ||type == SHARE_CHINA_SZ_CYB ||type == SHARE_CHINA_FUND_SZ) return "sz";
        if(type == SHARE_HK) return "hk";
        if(type == SHARE_US) return "us";
        return "undefined";
    }

    static QString fullCode(const QString& src)
    {
        QRegExp numexp("\\d{5,}");
        if(numexp.exactMatch(src))
        {
            return prefixCode(src)+src;
        }

        return src;
    }

public:
    //基本信息
    bool            mIsTop10;
    SHARE_TYPE      mShareType;
    double          mProfit;
    double          mTop10Buy;
    double          mTop10Sell;
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
    ShareData();
    ShareData(const ShareBaseData& data):ShareBaseData(data)
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
        return this->mCode == data.mCode && this->mDate == data.mDate && this->mShareType == data.mShareType;
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

    void append(const ShareDataList& list)
    {
        foreach (ShareData data, list) {
            append(data);
        }
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
