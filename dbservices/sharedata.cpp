#include "sharedata.h"

ShareData::ShareData():ShareBaseData()
{
    qRegisterMetaType<ShareDataList>("const ShareDataList&");
    mIndexFlag = false;
    mProfit = 0;
    mForeignCap = 0.0;
    mForeignVol = 0;
    mMax = 0.0;
    mMin = 0.0;
    mMoney = 0.0;
    mHsl = 0.0;
    mLastMoney = 0.0;
    mLastVol = 0;
    mLastChgPer = 0.0;
    mLast3DaysChgPers = 0.0;
    mLast5DaysChgPers = 0.0;
    mLast10DaysChgPers = 0.0;
    mLastMonthChgPers = 0.0;
    mChgPersFromMonth = 0.0;
    mChgPersFromWeek = 0.0;
    mChgPersFromYear = 0.0;
    mLastClose = 0.0;
    mMoneyRatio = 0.0;
    mBlockList.clear();
    mZJLX = 0.0;
    mSZZG = 0.0;
    mXJFH = 0.0;
    mGXL = 0.0;
    mGQDJR = QDate(2017,1,1);
    mYAGGR = QDate(2017,1,1);
    mTotalCap = 0;
    mMutalbleCap = 0;
    mForeignCapChg = 0.0;
    mForeignVolChg = 0;
}

ShareData::~ShareData()
{

}

bool ShareData::sortByPerDesc(const ShareData& d1, const ShareData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mChgPercent > d2.mChgPercent;
}

bool ShareData::sortByPerAsc(const ShareData &d1, const ShareData &d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mChgPercent < d2.mChgPercent;
}

bool ShareData::sortByCurDesc(const ShareData& d1, const ShareData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mCur > d2.mCur;
}

bool ShareData::sortByCurAsc(const ShareData& d1, const ShareData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mCur < d2.mCur;
}

bool ShareData::sortByMonDesc(const ShareData& d1, const ShareData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mMoney > d2.mMoney;
}

bool ShareData::sortByMonAsc(const ShareData& d1, const ShareData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mMoney < d2.mMoney;
}

bool ShareData::sortByHslDesc(const ShareData& d1, const ShareData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mHsl > d2.mHsl;
}

bool ShareData::sortByHslAsc(const ShareData& d1, const ShareData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mHsl < d2.mHsl;
}

bool ShareData::sortByMonRatioDesc(const ShareData& d1, const ShareData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mMoneyRatio > d2.mMoneyRatio;
}

bool ShareData::sortByMonRatioAsc(const ShareData& d1, const ShareData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mMoneyRatio < d2.mMoneyRatio;
}

bool ShareData::sortBy3DayChgAsc(const ShareData &d1, const ShareData &d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mLast3DaysChgPers < d2.mLast3DaysChgPers;
}

bool ShareData::sortBy3DayChgDesc(const ShareData &d1, const ShareData &d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mLast3DaysChgPers > d2.mLast3DaysChgPers;
}

bool ShareData::sortBy3DayZjlxAsc(const ShareData &d1, const ShareData &d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mZJLX < d2.mZJLX;
}

bool ShareData::sortBy3DayZjlxDesc(const ShareData &d1, const ShareData &d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mZJLX > d2.mZJLX;
}

bool ShareData::sortByGxlDesc(const ShareData& d1, const ShareData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mGXL> d2.mGXL;
}

bool ShareData::sortByGxlAsc(const ShareData& d1, const ShareData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mGXL < d2.mGXL;
}

bool ShareData::sortBySzzblDesc(const ShareData& d1, const ShareData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mSZZG > d2.mSZZG;
}

bool ShareData::sortBySzzblAsc(const ShareData& d1, const ShareData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mSZZG < d2.mSZZG;
}

bool ShareData::sortByGqdjrDesc(const ShareData& d1, const ShareData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mGQDJR > d2.mGQDJR;
}

bool ShareData::sortByGqdjrAsc(const ShareData& d1, const ShareData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mGQDJR < d2.mGQDJR;
}

bool ShareData::sortByTcapDesc(const ShareData& d1, const ShareData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mTotalCap > d2.mTotalCap;
}

bool ShareData::sortByTcapAsc(const ShareData& d1, const ShareData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mTotalCap < d2.mTotalCap;
}

bool ShareData::sortByMcapDesc(const ShareData& d1, const ShareData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mMutalbleCap > d2.mMutalbleCap;
}

bool ShareData::sortByMcapAsc(const ShareData& d1, const ShareData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mMutalbleCap < d2.mMutalbleCap;
}

bool ShareData::sortByProfitDesc(const ShareData& d1, const ShareData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mProfit > d2.mProfit;
}

bool ShareData::sortByProfitAsc(const ShareData& d1, const ShareData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mProfit < d2.mProfit;
}

bool ShareData::sortByForVolDesc(const ShareData& d1, const ShareData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mForeignVol > d2.mForeignVol;
}

bool ShareData::sortByForVolAsc(const ShareData& d1, const ShareData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mForeignVol < d2.mForeignVol;
}

bool ShareData::sortByForCapDesc(const ShareData& d1, const ShareData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mForeignCap > d2.mForeignCap;
}

bool ShareData::sortByForCapAsc(const ShareData& d1, const ShareData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mForeignCap < d2.mForeignCap;
}

bool ShareData::sortByForVolChgDesc(const ShareData& d1, const ShareData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mForeignVolChg > d2.mForeignVolChg;
}

bool ShareData::sortByForVolChgAsc(const ShareData& d1, const ShareData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mForeignVolChg < d2.mForeignVolChg;
}

bool ShareData::sortByForCapChgDesc(const ShareData& d1, const ShareData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mForeignCapChg > d2.mForeignCapChg;
}

bool ShareData::sortByForCapChgAsc(const ShareData& d1, const ShareData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mForeignCapChg < d2.mForeignCapChg;
}

bool ShareData::sortBy5DayChgAsc(const ShareData &d1, const ShareData &d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mLast5DaysChgPers < d2.mLast5DaysChgPers;
}

bool ShareData::sortBy5DayChgDesc(const ShareData &d1, const ShareData &d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mLast5DaysChgPers > d2.mLast5DaysChgPers;
}

bool ShareData::sortBy10DayChgAsc(const ShareData &d1, const ShareData &d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mLast10DaysChgPers < d2.mLast10DaysChgPers;
}

bool ShareData::sortBy10DayChgDesc(const ShareData &d1, const ShareData &d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mLast10DaysChgPers > d2.mLast10DaysChgPers;
}

bool ShareData::sortByMonthChgAsc(const ShareData &d1, const ShareData &d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mLastMonthChgPers < d2.mLastMonthChgPers;
}

bool ShareData::sortByMonthChgDesc(const ShareData &d1, const ShareData &d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mLastMonthChgPers > d2.mLastMonthChgPers;
}

bool ShareData::sortByYearChgAsc(const ShareData &d1, const ShareData &d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mChgPersFromYear < d2.mChgPersFromYear;
}

bool ShareData::sortByYearChgDesc(const ShareData &d1, const ShareData &d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mChgPersFromYear > d2.mChgPersFromYear;
}
