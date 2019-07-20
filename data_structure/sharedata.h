#ifndef SHAREDATA_H
#define SHAREDATA_H
#include <QStringList>
#include <QList>
#include <QObject>
#include <QMap>
#include <QDebug>
#include <QRegExp>
#include "hqbasedata.h"
#include "hqutils.h"
#include "utils/comdatadefines.h"
#include <QDebug>

#define         SH_FUND_REG         "(sh){0,1}5[0-9]{5}"
#define         SH_INDEX_REG        "sh0[0-9]{5}"
#define         SZ_FUND_REG         "(sz){0,1}1[0-9]{5}"
#define         SZ_INDEX_REG        "sz399[0-9]{3}"
#define         SH_SHARE_REG        "(sh){0,1}6[0-9]{5}"
#define         SZZB_SHARE_REG        "(sz){0,1}(00[01]{1}[0-9]{3})"
#define         SZZXB_SHARE_REG        "(sz){0,1}(002[0-9]{3})"
#define         SZCYB_SHARE_REG        "(sz){0,1}(30[0-9]{4})"
#define         HK_SHARE_REG        "(hk){0,1}\\d{5}"

typedef    enum     share_data_type
{
    SHARE_UNDEF = 0x0000,
    SHARE_INDEX_SH = 0x0001,
    SHARE_INDEX_SZ = 0x0002,
    SHARE_INDEX_HK = 0x0004,
    SHARE_INDEX_US = 0x0008,
    SHARE_INDEX = SHARE_INDEX_SH | SHARE_INDEX_SZ | SHARE_INDEX_HK | SHARE_INDEX_US,
    SHARE_SH_ZB = 0x0010,
    SHARE_SH_FUND = 0x0020,
    SHARE_SH = SHARE_SH_ZB | SHARE_SH_FUND,
    SHARE_SZ_ZB = 0x0100,
    SHARE_SZ_ZXB = 0x0200,
    SHARE_SZ_CYB = 0x0400,
    SHARE_SZ_FUND = 0x0800,
    SHARE_SZ = SHARE_SZ_ZB | SHARE_SZ_FUND |SHARE_SZ_ZXB | SHARE_SZ_CYB,
    SHARE_FUND = SHARE_SH_FUND | SHARE_SZ_FUND,
    SHARE_US = 0x1000,
    SHARE_HK = 0x2000,
}SHARE_DATA_TYPE;

struct HistoryInfo{
    double                  mLastMoney;
    qint64                  mLastVol;
    double                  mLastChgPer;
    double                  mLast3DaysChgPers;
    double                  mLast5DaysChgPers;
    double                  mLast10DaysChgPers;
    //double                  mLastMonthChgPers;
    double                  mChgPersFromYear;
    double                  mChgPersFromWeek;
    double                  mChgPersFromMonth;
    double                  mChgPersFromYear_BAK;
    double                  mChgPersFromWeek_BAK;
    double                  mChgPersFromMonth_BAK;
    double                  mLastClose;

    double                  mYearDayPrice;
    double                  mMonthDayPrice;
    double                  mWeekDayPrice;
    HistoryInfo()
    {
        mChgPersFromYear_BAK = 1.0;
        mChgPersFromWeek_BAK = 1.0;
        mChgPersFromMonth_BAK = 1.0;
        mYearDayPrice = 0.0;
        mMonthDayPrice = 0.0;
        mWeekDayPrice = 0.0;
    }
};

enum Share_Basic_Update_Mode{
    Share_Basic_Update_Code = 0x01,
    Share_Basic_Update_Hsgt = 0x02,
    Share_Basic_Update_Profit = 0x04,
    Share_Basic_Update_Fav = 0x08,
    Share_Basic_Update_Block = 0x10,
    Share_Basic_Update_All = Share_Basic_Update_Code | Share_Basic_Update_Hsgt | Share_Basic_Update_Profit | Share_Basic_Update_Fav | Share_Basic_Update_Block,

};

struct ShareData : public HqBaseData
{
public:
    //基本信息
    SHARE_DATA_TYPE         mShareType;
    HistoryInfo             mHistory;
    double                  mProfit;
    double                  mHsl;
    double                  mMoneyRatio;
    double                  mZJLX;      //资金流向
    double                  mGXL;   //股息率
    double                  mTotalCap;
    double                  mMutalbleCap;
    qint64                  mForeignVolChg;
    double                  mForeignCap;
    double                  mForeignCapChg;
    double                  mCur;
    double                  mChg;
    double                  mChgPercent;
    double                  mHigh;
    double                  mLow;
    double                  mOpen;
    double                  mClose;
    double                  mMoney;
    double                  mRZRQ;
    qint64                  mVol;
    ShareDateTime           mTime;
    QStringList             mBlockCodeList;
    FinancialData           mFinanceData;
    ShareBonus              mBonusData;
    ShareHsgt               mHsgtData;
public:
    ShareData();
    static int  stk_sort_type;
    static int  stk_sort_rule;
    static bool ShareSort(const ShareData& d1, const ShareData& d2);

    bool operator ==(const ShareData& data)
    {
        return this->mCode == data.mCode && this->mTime == data.mTime && this->mShareType == data.mShareType;
    }

    bool appendBlock(const QString& code)
    {
        if(!mBlockCodeList.contains(code)) return false;
        mBlockCodeList.append(code);
        return true;
    }

    bool isContainsBlock(const QString& code)
    {
        return mBlockCodeList.contains(code);
    }
    QStringList getBlockCodesList() const
    {
        return mBlockCodeList;
    }

    static QString shareTypeString(SHARE_DATA_TYPE type)
    {
        if(type == SHARE_SH_ZB) return QObject::tr("上证A股");
        if(type == SHARE_SZ_ZB) return QObject::tr("深证主板");
        if(type == SHARE_SZ_ZXB) return QObject::tr("深证中小");
        if(type == SHARE_SZ_CYB) return QObject::tr("深证创业");
        if(type == SHARE_INDEX_SH) return QObject::tr("上证指数");
        if(type == SHARE_INDEX_SZ) return QObject::tr("深圳指数");
        if(type == SHARE_SH_FUND) return QObject::tr("上证基金");
        if(type == SHARE_SZ_FUND) return QObject::tr("深圳基金");
        if(type == SHARE_HK) return QObject::tr("港股");
        return "-";
    }

    //通过证券的代码来获取证券的类型(不包括指数,指数需要强制指定)
    static SHARE_DATA_TYPE shareType(const QString &src)
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

        if(shShare.exactMatch(src)) return SHARE_SH_ZB;
        if(szZBShare.exactMatch(src)) return SHARE_SZ_ZB;
        if(szZXBShare.exactMatch(src)) return SHARE_SZ_ZXB;
        if(szCYBShare.exactMatch(src)) return SHARE_SZ_CYB;
        if(shIndex.exactMatch(src)) return SHARE_INDEX_SH;
        if(szIndex.exactMatch(src)) return SHARE_INDEX_SZ;
        if(shFund.exactMatch(src)) return SHARE_SH_FUND;
        if(szFund.exactMatch(src)) return SHARE_SZ_FUND;
        if(hkShare.exactMatch(src)) return SHARE_HK;
        return SHARE_UNDEF;
    }

    static QString prefixCode(const QString&  code)
    {
        SHARE_DATA_TYPE type = shareType(code);
        if(type & SHARE_SH) return "sh";
        if(type & SHARE_SZ) return "sz";
        if(type & SHARE_HK) return "hk";
        if(type & SHARE_US) return "us";
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

    HqBaseData base() const
    {
        return HqBaseData(mCode, mName, mPY, mType, mIsFav);
    }

    QString keyOfCodeTime() const
    {
        return QString("%1_%2").arg(mCode).arg(mTime.date().toString("yyyy-MM-dd"));
    }

    bool    isUpdated(const ShareData& tar, int mode)
    {
        bool change = false;
        if((Share_Basic_Update_Code & mode) && mPY != tar.mPY)
        {
            mName = tar.mName;
            mPY = tar.mPY;
            change = true;
        }
        if((Share_Basic_Update_Block & mode) && mBlockCodeList.join(",") != tar.mBlockCodeList.join(","))
        {
            mBlockCodeList = tar.mBlockCodeList;
            change = true;
        }
        if((Share_Basic_Update_Fav & mode) && mIsFav != tar.mIsFav)
        {
            mIsFav = tar.mIsFav;
            change = true;
        }
        if((Share_Basic_Update_Hsgt & mode) && mHsgtData.mIsTop10 != tar.mHsgtData.mIsTop10)
        {
            mHsgtData.mIsTop10 = tar.mHsgtData.mIsTop10;
            change = true;
        }

        if((Share_Basic_Update_Profit & mode) && mProfit != tar.mProfit)
        {
            mProfit = tar.mProfit;
            change = true;
        }

        return change;
    }

};
typedef         QList<ShareData>            ShareDataList;
typedef         QMap<QString, ShareData>    ShareDataMap;
Q_DECLARE_METATYPE(ShareData)
Q_DECLARE_METATYPE(ShareDataList)
Q_DECLARE_METATYPE(ShareDataMap)

typedef struct hqShareHistoryFileData{
    unsigned int    mDate;
    double          mClose;                 //最新
    double          mLastClose;             //昨日最后价格
    double          mCloseAdjust;           //复权
    double          mMoney;
    qint64          mForeignVol;
    double          mForeignMututablePercent;
    qint64          mTotalShareCount;
}ShareHistoryFileData;

typedef QList<ShareHistoryFileData>       ShareHistoryFileDataList;

Q_DECLARE_METATYPE(ShareHistoryFileData)
Q_DECLARE_METATYPE(ShareHistoryFileDataList)

typedef struct hqShareForeignVolFileData{
    unsigned int    mCode;
    qint64          mForeignVol;
    double          mMutuablePercent;

    hqShareForeignVolFileData()
    {
        mCode = 0;
        mForeignVol = 0;
        mMutuablePercent = 0.0;
    }

    hqShareForeignVolFileData(unsigned int code)
    {
        mCode = code;
        mForeignVol = 0;
        mMutuablePercent = 0.0;
    }

    bool operator ==(const hqShareForeignVolFileData& other)
    {
        return this->mCode == other.mCode;
    }

    friend QDebug operator <<(QDebug debug, const hqShareForeignVolFileData& data)
    {
        QString info = QString("foreign vol info:Code(%1),Vol(%L2),Percent(%3)").arg(data.mCode).arg(data.mForeignVol).arg(data.mMutuablePercent, 0, 'f', 2);
        debug<<info;
        return debug;
    }
}ShareForignVolFileData;

typedef QList<ShareForignVolFileData>       ShareForignVolFileDataList;

Q_DECLARE_METATYPE(ShareForignVolFileData)
Q_DECLARE_METATYPE(ShareForignVolFileDataList)

struct ShareHistoryCounter{
    QString     code;
    QDate       weekDay;
    QDate       monthDay;
    QDate       yearDay;
    double      weekP;
    double      monthP;
    double      yearP;
    double      lastMoney;
    qint64      foreign_vol;
    double      foreign_percent;
    double      foreign_ch1;
    double      foreign_ch5;
    double      foreign_ch10;
};


#if 0
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

    ShareData &valueOfDate(const ShareDate& date, const QString& code)
    {
        QString key = QString("%1_%2").arg(QDateTime(date.date()).toMSecsSinceEpoch()).arg(ShareData::fullCode(code));
        if(!mDataIndexMap.contains(key))
        {
            ShareData data;
            data.mCode = code.right(6);
            data.mTime = ShareDateTime(date.date());
            append(data);
        }
        //qDebug()<<date<<mDataIndexMap.value(date)<<this->size();
        return (*this)[mDataIndexMap.value(key)];
    }

    void append(const ShareData& data)
    {
        if(this->contains(data)) return;
        QList<ShareData>::append(data);
        QString key = QString("%1_%2").arg(data.mTime.toTime_t()).arg(ShareData::fullCode(data.mCode));
        mDataIndexMap[key] = this->size() - 1;
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
    QMap<QString, int>        mDataIndexMap; //key = time+code(sh600036)
};

#endif

//typedef QList<North_South_Bound_Data>        LGT_DATALIST;

#endif // SHAREDATA_H
