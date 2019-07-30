#ifndef HQBASEDATA_H
#define HQBASEDATA_H

#include <QString>
#include "hqutils.h"

struct HqBaseData
{
public:
    //定义基础的数据类型
    typedef enum data_type{
        UNDEFINED = 0x00,
        SHARE = 0x01,
        BLOCK = 0x02,
    }DATA_TYPE;

    HqBaseData();
    ~HqBaseData() {}
    HqBaseData(const HqBaseData& data);
    HqBaseData(const QString& code, const QString& name, const QString& py, int type, bool isFav);
    HqBaseData& operator =(const HqBaseData& other);
    bool operator ==(const HqBaseData& other)
    {
        return mCode == other.mCode && mPY == other.mPY && mIsFav == other.mIsFav;
    }
    void update(const QString& name, const QString& py)
    {
        mName = name;
        mPY = py;
    }
    void update(bool isFav)
    {
        mIsFav = isFav;
    }

public:
    bool        mIsFav;
    int         mType;
    QString     mCode;
    QString     mName;
    QString     mPY; //拼音简称
    bool        mStatus;    //是否停牌

};

typedef     QList<HqBaseData>   HqBaseDataList;
Q_DECLARE_METATYPE(HqBaseData)
Q_DECLARE_METATYPE(HqBaseDataList)

struct ShareHolder{
    QString     mShareCode;
    QString     mHolderCode;
    QString     mName;
    double      mShareCount;
    double      mFundPercent;
    ShareWorkingDate   mDate;

    ShareHolder(){
        mShareCount = 0;
        mFundPercent = 0.0;
    }
};
typedef QList<ShareHolder>  ShareHolderList;
Q_DECLARE_METATYPE(ShareHolder)
Q_DECLARE_METATYPE(ShareHolderList)


//财务数据
typedef struct Finance
{
    QString         mCode;
    double          mEPS;       //每股收益
    double          mBVPS;      //每股净资产
    double          mROE;       //净资产收益率
    double          mTotalShare;
    double          mMutalShare;
    Finance()
    {
        mEPS = 0.0;
        mBVPS = 0.0;
        mROE = 0.0;
        mTotalShare = 0;
        mMutalShare = 0;
    }
}FinancialData;

typedef QList<FinancialData>    FinancialDataList;
Q_DECLARE_METATYPE(FinancialData)
Q_DECLARE_METATYPE(FinancialDataList)


//分红信息
struct  ShareBonus
{
    QString             mCode;
    double              mSZZG; //送转股比例
    double              mXJFH;  //现金分红
    ShareWorkingDate           mGQDJR; //股权登记日
    ShareWorkingDate           mYAGGR; //预案公告日
    ShareWorkingDate           mDate;  //报告日期的记录
    ShareBonus()
    {
        mSZZG = 0.0;
        mXJFH = 0.0;
    }
};

typedef QList<ShareBonus>       ShareBonusList;
typedef QMap<QString, ShareBonusList> ShareBonusMap;

Q_DECLARE_METATYPE(ShareBonus)
Q_DECLARE_METATYPE(ShareBonusList)

typedef struct North_South_Bound_Data
{
    QString             mCode;
    QString             mName;
    double              mBuy;
    double              mSell;
    double              mTotal;
    double              mPure;
    double              mChange;
    qint64              mVolTotal;
    qint64              mVolChange;
    double              mVolMutablePercent;
    double              mVolCh1;
    double              mVolCh5;
    double              mVolCh10;
    bool                mIsTop10;
    ShareWorkingDateTime       mDate;

    North_South_Bound_Data()
    {
        mBuy = 0.0;
        mSell = 0.0;
        mTotal = 0.0;
        mPure = 0.0;
        mChange = 0.0;
        mVolTotal = 0;
        mVolChange = 0;
        mVolMutablePercent = 0.0;
        mVolCh1 = 0.0;
        mVolCh5 = 0.0;
        mVolCh10 = 0.0;
    }

    bool operator <(const North_South_Bound_Data& data) const
    {
        return ((*this).mPure) < (data.mPure);
    }

    bool operator >(const North_South_Bound_Data& data) const
    {
        return ((*this).mPure) > (data.mPure);
    }

}ShareHsgt;

typedef QList<ShareHsgt>       ShareHsgtList;

Q_DECLARE_METATYPE(ShareHsgt)
Q_DECLARE_METATYPE(ShareHsgtList)

struct ShareZjlx
{
    QString             mCode;
    double              mPure;
    ShareWorkingDateTime       mDate;

    ShareZjlx()
    {
        mPure = 0.0;
    }

    bool operator <(const ShareZjlx& data) const
    {
        return ((*this).mPure) < (data.mPure);
    }

    bool operator >(const ShareZjlx& data) const
    {
        return ((*this).mPure) > (data.mPure);
    }

};

typedef QList<ShareZjlx>       ShareZjlxList;

Q_DECLARE_METATYPE(ShareZjlx)
Q_DECLARE_METATYPE(ShareZjlxList)


struct ShareRzRq
{
    QString             mCode;
    double              mRZRQ;
    ShareWorkingDateTime       mDate;

    ShareRzRq()
    {
        mRZRQ = 0.0;
    }

    bool operator <(const ShareRzRq& data) const
    {
        return ((*this).mRZRQ) < (data.mRZRQ);
    }

    bool operator >(const ShareRzRq& data) const
    {
        return ((*this).mRZRQ) > (data.mRZRQ);
    }

};

typedef QList<ShareRzRq>       ShareRzrqList;

Q_DECLARE_METATYPE(ShareRzRq)
Q_DECLARE_METATYPE(ShareRzrqList)





#endif // HQBASEDATA_H
