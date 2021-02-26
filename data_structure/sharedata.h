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
#define         SH_ZB_SHARE_REG        "(sh){0,1}60[0-9]{4}"
#define         SH_KCB_SHARE_REG    "(sh){0,1}68[0-9]{4}"
#define         SH_KZZ_REG          "(sh){0,1}11[0-9]{4}"

#define         SZ_FUND_REG         "(sz){0,1}1[0-9]{5}"
#define         SZ_INDEX_REG        "sz399[0-9]{3}"
#define         SZ_ZB_SHARE_REG        "(sz){0,1}(00[01]{1}[0-9]{3})"
#define         SZ_ZXB_SHARE_REG        "(sz){0,1}(00[23]{1}[0-9]{3})"
#define         SZ_CYB_SHARE_REG        "(sz){0,1}(3[0-9]{5})"
#define         SZ_KZZ_REG          "(sz){0,1}12[0-9]{4}"

#define         HK_SHARE_REG        "(hk){0,1}\\d{5}"



typedef    enum     share_data_type
{
    SHARE_UNDEF = 0x0000,
    SHARE_INDEX_SH = 1,
    SHARE_INDEX_SZ = 1<<1,
    SHARE_INDEX_HK = 1<<2,
    SHARE_INDEX_US = 1<<3,
    SHARE_INDEX = SHARE_INDEX_SH | SHARE_INDEX_SZ | SHARE_INDEX_HK | SHARE_INDEX_US,

    SHARE_SH_ZB = 1<<4,
    SHARE_SH_KCB = 1<<5,
    SHARE_SH = SHARE_SH_ZB | SHARE_SH_KCB,

    SHARE_SZ_ZB = 1<<7,
    SHARE_SZ_ZXB = 1<<8,
    SHARE_SZ_CYB = 1<<9,
    SHARE_SZ = SHARE_SZ_ZB  |SHARE_SZ_ZXB | SHARE_SZ_CYB,

    SHARE_SH_FUND = 1<<6,
    SHARE_SZ_FUND = 1<<10,
    SHARE_FUND = SHARE_SH_FUND | SHARE_SZ_FUND,

    SHARE_US = 1<<11,
    SHARE_HK = 1<<12,
    SHARE_SH_KZZ = 1<<13,
    SHARE_SZ_KZZ = 1<<14,
    SHARE_KZZ = SHARE_SH_KZZ | SHARE_SZ_KZZ,
    SHARE_SH_TOTAL = SHARE_SH | SHARE_SH_FUND | SHARE_SH_KZZ,
    SHARE_SZ_TOTAL = SHARE_SZ | SHARE_SZ_FUND | SHARE_SZ_KZZ,
    SHARE_SHARE_ONLY = SHARE_SH | SHARE_SZ,
}SHARE_DATA_TYPE;

//struct ShareDailyInfo{
//    qint64              time;                       //时间
//    double             money;                      //成交额
//    double             vol;                        //成交量
//    double             lgt_money;                  //陆股通成交额
//    double             lgt_pure;                   //陆股通净买入额
//    double             lgt_mutable_percent;         //陆股通比例
//    double             lgt_vol;                     //陆股通持股数
//    double             close;                      //收盘价
//    double             subscription_price;          //复权价
//    double             last_close;                  //上一日收盘价
//    double             total_share_vol;             //总股本
//    double              mutal_share_vol;            //流通股本
//};

struct ShareCounterInfo{
    double                  mLastMoney = 0.0;           //前成交金额
    double                  mYearDayPrice = 0.0;        //复权的年初价格
    double                  mMonthDayPrice = 0.0;       //复权的月初价格
    double                  mWeekDayPrice = 0.0;        //复权的周初价格
    double                  mYearChgPer = 0.0;          //复权的年涨幅
    double                  mMonthChgPer = 0.0;         //复权的月涨幅
    double                  mWeekChgPer = 0.0;          //复权的周涨幅
    QDate                   mWeekDay;
    QDate                   mMonthDay;
    QDate                   mYearDay;
};

enum Share_Basic_Update_Mode{
    Share_Basic_Update_Code = 0x01,
    Share_Basic_Update_Hsgt = 0x02,
    Share_Basic_Update_Profit = 0x04,
    Share_Basic_Update_Fav = 0x08,
    Share_Basic_Update_Block = 0x10,
    Share_Basic_Update_All = Share_Basic_Update_Code | Share_Basic_Update_Hsgt | Share_Basic_Update_Profit | Share_Basic_Update_Fav | Share_Basic_Update_Block,

};

struct ShareRealData{
    double                  mHsl = 0.0;           //换手率
    double                  mMoneyRatio = 0.0;    //资金比率
    double                  mZJLX = 0.0;          //资金流向
    double                  mTotalCap = 0.0;      //总市值
    double                  mMutalbleCap = 0.0;   //流通市值
    double                  mLastClose = 0.0;     //前收盘价
    double                  mChg = 0.0;           //涨跌
    double                  mChgPercent = 0.0;    //涨跌幅
    double                  mHigh = 0.0;          //最高
    double                  mLow = 0.0;           //最低
    double                  mOpen = 0.0;          //开盘
    double                  mClose = 0.0;         //收盘价
    double                  mMoney = 0.0;         //成交金额
    qint64                  mVol = 0.0;           //成交量
    ShareTradeDateTime      mTime = 0.0;          //更新时间
};

struct ShareForeignHolder{
    double                  mVol = 0.0;             //外资持股量
    double                  mPercent = 0.0;         //外资持股占流通百分比
    double                  mCap = 0.0;             //外资持股市值
    double                  mJMR1 = 0.0;            //1日净买入额
    double                  mJMR5 = 0.0;            //5日净买入额
    double                  mJMR10 = 0.0;           //10日净买入额
    double                  mJMRM = 0.0;            //月净买入额
    double                  mJMRY = 0.0;            //年净买入额
    double                  mMoney = 0.0;           //当日外资成交金额
    bool                    mIsTop10 = false;       //当日是否外资成交榜

};

struct ShareData : public HqBaseData
{
public:
    //实时信息
    ShareRealData           mRealInfo;
    //统计信息
    ShareCounterInfo        mCounterInfo;
    //外资持股信息
    ShareForeignHolder      mForeignInfo;
    //交易盈亏统计
    double                  mProfit;

public:
    ShareData();
    static int  stk_sort_type;
    static int  stk_sort_rule;
    static bool ShareSort(const ShareData& d1, const ShareData& d2);

    bool operator ==(const ShareData& data)
    {
        return this->mCode == data.mCode && this->mListTime == data.mListTime && this->mType == data.mType;
    }

    bool appendBlock(const QString& code)
    {
        if(!mReferCodeList.contains(code)) return false;
        mReferCodeList.append(code);
        return true;
    }

    bool isContainsBlock(const QString& code)
    {
        return mReferCodeList.contains(code);
    }
    QStringList getBlockCodesList() const
    {
        return mReferCodeList;
    }


    static QString shareTypeString(SHARE_DATA_TYPE type)
    {
        if(type == SHARE_SH_ZB) return QObject::tr("上证A股");
        if(type == SHARE_SH_KCB) return QObject::tr("科创板");
        if(type == SHARE_SZ_ZB) return QObject::tr("深证主板");
        if(type == SHARE_SZ_ZXB) return QObject::tr("深证中小");
        if(type == SHARE_SZ_CYB) return QObject::tr("深证创业");
        if(type == SHARE_INDEX_SH) return QObject::tr("上证指数");
        if(type == SHARE_INDEX_SZ) return QObject::tr("深圳指数");
        if(type == SHARE_SH_FUND) return QObject::tr("上证基金");
        if(type == SHARE_SZ_FUND) return QObject::tr("深圳基金");
        if(type == SHARE_SZ_KZZ) return QObject::tr("深圳可转债");
        if(type == SHARE_SH_KZZ) return QObject::tr("上海可转债");
        if(type == SHARE_HK) return QObject::tr("港股");
        return "-";
    }

    //通过证券的代码来获取证券的类型(不包括指数,指数需要强制指定)
    static SHARE_DATA_TYPE shareType(const QString &src)
    {
        QRegExp shZBShare(SH_ZB_SHARE_REG);
        QRegExp shKCBShare(SH_KCB_SHARE_REG);
        QRegExp shKZZShare(SH_KZZ_REG);

        QRegExp szZBShare(SZ_ZB_SHARE_REG);
        QRegExp szZXBShare(SZ_ZXB_SHARE_REG);
        QRegExp szCYBShare(SZ_CYB_SHARE_REG);
        QRegExp szKZZShare(SZ_KZZ_REG);

        QRegExp hkShare(HK_SHARE_REG);

        QRegExp shFund(SH_FUND_REG);
        QRegExp szFund(SZ_FUND_REG);
        QRegExp shIndex(SH_INDEX_REG);
        QRegExp szIndex(SZ_INDEX_REG);

        if(shZBShare.exactMatch(src)) return SHARE_SH_ZB;
        if(shKCBShare.exactMatch(src)) return SHARE_SH_KCB;
        if(shKZZShare.exactMatch(src)) return SHARE_SH_KZZ;

        if(szZBShare.exactMatch(src)) return SHARE_SZ_ZB;
        if(szZXBShare.exactMatch(src)) return SHARE_SZ_ZXB;
        if(szCYBShare.exactMatch(src)) return SHARE_SZ_CYB;
        if(szKZZShare.exactMatch(src)) return SHARE_SZ_KZZ;

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
        if((type & SHARE_SH) || (type & SHARE_SH_KZZ) || (type & SHARE_SH_FUND)) return "sh";
        if((type & SHARE_SZ) || (type & SHARE_SZ_KZZ) || (type & SHARE_SZ_FUND)) return "sz";
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
        return QString("%1_%2").arg(mCode).arg(mRealInfo.mTime.date().toString("yyyy-MM-dd"));
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
        if((Share_Basic_Update_Block & mode) && mReferCodeList.join(",") != tar.mReferCodeList.join(","))
        {
            mReferCodeList = tar.mReferCodeList;
            change = true;
        }
        if((Share_Basic_Update_Fav & mode) && mIsFav != tar.mIsFav)
        {
            mIsFav = tar.mIsFav;
            change = true;
        }
        if((Share_Basic_Update_Hsgt & mode) && mForeignInfo.mIsTop10 != tar.mForeignInfo.mIsTop10)
        {
            mForeignInfo.mIsTop10 = tar.mForeignInfo.mIsTop10;
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
typedef         QMap<QString, ShareData>    ShareDataMap;   //key市场代码(sh/sz) + code  sh600036
Q_DECLARE_METATYPE(ShareData)
Q_DECLARE_METATYPE(ShareDataList)
Q_DECLARE_METATYPE(ShareDataMap)

typedef struct hqShareDailyData{
    unsigned int    mDate;
    double          mOpen;
    double          mHigh;
    double          mLow;
    double          mClose;                 //最新
    double          mLastClose;             //昨日最后价格
    double          mCloseAdjust;           //复权  计算涨跌幅使用
    double          mMoney;                 //总成交金额
    double          mVol;                   //总成交量
    double          mZGB;
    double          mLTGB;
    double          mForeignVol;
    double          mForeignMututablePercent;

    hqShareDailyData()
    {
        mDate = 0;
        mClose = 0.0;
        mLastClose = 0.0;
        mCloseAdjust = 0.0;
        mMoney = 0.0;
        mForeignVol = 0;
        mForeignMututablePercent = 0.0;
        mZGB = 0.0;
        mLTGB = 0.0;
        mOpen = 0.0;
        mVol = 0.0;
        mHigh = 0.0;
        mLow = 0.0;
    }
}ShareDailyData;

typedef QList<ShareDailyData>       ShareDailyDataList;

Q_DECLARE_METATYPE(ShareDailyData)
Q_DECLARE_METATYPE(ShareDailyDataList)

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
    ShareCounterInfo  info;
};

struct  GRAPHIC_DATA{
    QDate           mDate;
    double          mClose;
    double          mRzye;
    double          mZjlx;
    double          mForVol;
    double          mMoney;
};
class GRAPHIC_DATA_LIST : public QList<GRAPHIC_DATA>
{
public:
    inline GRAPHIC_DATA_LIST()
    {
        mMaxClose = 0.0;
        mMaxRzye = 0.0;
        mMaxZjlx = 0.0;
        mMaxForeignVol = 0.0;
        mMaxMoney = 0.0;
        clear();
    }

    inline GRAPHIC_DATA_LIST(const ShareDataList& list)
    {
        foreach (ShareData data, list) {
            GRAPHIC_DATA graph;
            graph.mDate = data.mRealInfo.mTime.date();
            graph.mClose = data.mRealInfo.mClose;
            graph.mForVol = data.mForeignInfo.mVol;
            graph.mMoney = data.mRealInfo.mMoney;
//            graph.mRzye = data.mRZRQ;
            graph.mZjlx = data.mRealInfo.mZJLX;
            append(graph);
        }
        for(int i=1; i<size(); i++)
        {
            if(this->at(i).mForVol == 0 && this->at(i-1).mForVol != 0)
            {
                (*this)[i].mForVol = (*this)[i-1].mForVol;
            }
        }
    }

    inline GRAPHIC_DATA_LIST(const ShareDailyDataList& list)
    {
        foreach (ShareDailyData data, list) {
            GRAPHIC_DATA graph;
            graph.mDate = QDateTime::fromTime_t(data.mDate).date();
            graph.mClose = data.mClose;
            graph.mForVol = data.mForeignVol;
            graph.mMoney = data.mMoney;
//            graph.mRzye = data.mRZRQ;
//            graph.mZjlx = data.mZJLX;
            append(graph);
        }
        for(int i=1; i<size(); i++)
        {
            if(this->at(i).mForVol == 0 && this->at(i-1).mForVol != 0)
            {
                (*this)[i].mForVol = (*this)[i-1].mForVol;
            }
        }
    }

    void append(const GRAPHIC_DATA& data)
    {
        QList<GRAPHIC_DATA>::append(data);
        if(data.mClose > mMaxClose)
        {
            mMaxClose = data.mClose;
        }
        if(data.mForVol > mMaxForeignVol)
        {
            mMaxForeignVol = data.mForVol;
        }
        if(data.mRzye> mMaxRzye)
        {
            mMaxRzye = data.mRzye;
        }
        if(data.mZjlx > mMaxZjlx)
        {
            mMaxZjlx = data.mZjlx;
        }
        if(data.mMoney > mMaxMoney)
        {
            mMaxMoney = data.mMoney;
        }
    }


public:
    double      mMaxClose;
    double      mMaxRzye;
    double      mMaxZjlx;
    double      mMaxForeignVol;
    double      mMaxMoney;
};

enum   ShareExchangeType
{
    ShareExchange_None = 0,
    ShareExchange_Buy = 1,
    ShareExchange_Sell,
    ShareExchange_Share_Bonus,
    ShareExchange_Dividend_Bonus,
};

struct ShareExchangeData{
    int         mID;
    QString     mCode;
    QString     mName;
    QString     mDateTime;
    int         mType;
    int         mNum;
    int         mTotalNum;
    double      mPrice;
    double      mMoney;
    double      mYongjin;
    double      mYinhuasui;
    double      mOther;
    double      mNetIncome;
    QString     mSerialText;
};

struct ShareForeignVolChgCounter
{
    double      mShareHold_Change;
    double      mShareSZ_Change;
    double      mShareRate_Change;
};

struct ShareForeignVolCounter{
    int             mCode;
    qint64          mDate;
    double          mPrice;                     //最新价
    double          mChangePercent;             //涨跌幅
    double          mShareHold;                 //持股数
    double          mShareSZ;                   //持股市值
    double          mLTZB;                      //流通占比
    double          mZZB;                       //总占比
    double          mLTSZ;                      //流通市值
    double          mZSZ;                       //总市值
    ShareForeignVolChgCounter  mChg1;
    ShareForeignVolChgCounter  mChg3;
    ShareForeignVolChgCounter  mChg5;
    ShareForeignVolChgCounter  mChg10;
    ShareForeignVolChgCounter  mChgM;
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

    ShareData &valueOfDate(const ShareWorkingDate& date, const QString& code)
    {
        QString key = QString("%1_%2").arg(QDateTime(date.date()).toMSecsSinceEpoch()).arg(ShareData::fullCode(code));
        if(!mDataIndexMap.contains(key))
        {
            ShareData data;
            data.mCode = code.right(6);
            data.mTime = ShareWorkingDateTime(date.date());
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
