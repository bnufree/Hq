#include "stockdata.h"

StockData::StockData()
{
    qRegisterMetaType<StockDataList>("const StockDataList&");
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

StockData::~StockData()
{

}

bool StockData::sortByPerDesc(const StockData& d1, const StockData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mChgPercent > d2.mChgPercent;
}

bool StockData::sortByPerAsc(const StockData &d1, const StockData &d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mChgPercent < d2.mChgPercent;
}

bool StockData::sortByCurDesc(const StockData& d1, const StockData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mCur > d2.mCur;
}

bool StockData::sortByCurAsc(const StockData& d1, const StockData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mCur < d2.mCur;
}

bool StockData::sortByMonDesc(const StockData& d1, const StockData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mMoney > d2.mMoney;
}

bool StockData::sortByMonAsc(const StockData& d1, const StockData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mMoney < d2.mMoney;
}

bool StockData::sortByHslDesc(const StockData& d1, const StockData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mHsl > d2.mHsl;
}

bool StockData::sortByHslAsc(const StockData& d1, const StockData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mHsl < d2.mHsl;
}

bool StockData::sortByMonRatioDesc(const StockData& d1, const StockData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mMoneyRatio > d2.mMoneyRatio;
}

bool StockData::sortByMonRatioAsc(const StockData& d1, const StockData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mMoneyRatio < d2.mMoneyRatio;
}

bool StockData::sortBy3DayChgAsc(const StockData &d1, const StockData &d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mLast3DaysChgPers < d2.mLast3DaysChgPers;
}

bool StockData::sortBy3DayChgDesc(const StockData &d1, const StockData &d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mLast3DaysChgPers > d2.mLast3DaysChgPers;
}

bool StockData::sortBy3DayZjlxAsc(const StockData &d1, const StockData &d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mZJLX < d2.mZJLX;
}

bool StockData::sortBy3DayZjlxDesc(const StockData &d1, const StockData &d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mZJLX > d2.mZJLX;
}

bool StockData::sortByGxlDesc(const StockData& d1, const StockData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mGXL> d2.mGXL;
}

bool StockData::sortByGxlAsc(const StockData& d1, const StockData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mGXL < d2.mGXL;
}

bool StockData::sortBySzzblDesc(const StockData& d1, const StockData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mSZZG > d2.mSZZG;
}

bool StockData::sortBySzzblAsc(const StockData& d1, const StockData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mSZZG < d2.mSZZG;
}

bool StockData::sortByGqdjrDesc(const StockData& d1, const StockData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mGQDJR > d2.mGQDJR;
}

bool StockData::sortByGqdjrAsc(const StockData& d1, const StockData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mGQDJR < d2.mGQDJR;
}

bool StockData::sortByTcapDesc(const StockData& d1, const StockData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mTotalCap > d2.mTotalCap;
}

bool StockData::sortByTcapAsc(const StockData& d1, const StockData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mTotalCap < d2.mTotalCap;
}

bool StockData::sortByMcapDesc(const StockData& d1, const StockData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mMutalbleCap > d2.mMutalbleCap;
}

bool StockData::sortByMcapAsc(const StockData& d1, const StockData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mMutalbleCap < d2.mMutalbleCap;
}

bool StockData::sortByProfitDesc(const StockData& d1, const StockData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mProfit > d2.mProfit;
}

bool StockData::sortByProfitAsc(const StockData& d1, const StockData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mProfit < d2.mProfit;
}

bool StockData::sortByForVolDesc(const StockData& d1, const StockData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mForeignVol > d2.mForeignVol;
}

bool StockData::sortByForVolAsc(const StockData& d1, const StockData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mForeignVol < d2.mForeignVol;
}

bool StockData::sortByForCapDesc(const StockData& d1, const StockData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mForeignCap > d2.mForeignCap;
}

bool StockData::sortByForCapAsc(const StockData& d1, const StockData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mForeignCap < d2.mForeignCap;
}

bool StockData::sortByForVolChgDesc(const StockData& d1, const StockData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mForeignVolChg > d2.mForeignVolChg;
}

bool StockData::sortByForVolChgAsc(const StockData& d1, const StockData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mForeignVolChg < d2.mForeignVolChg;
}

bool StockData::sortByForCapChgDesc(const StockData& d1, const StockData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mForeignCapChg > d2.mForeignCapChg;
}

bool StockData::sortByForCapChgAsc(const StockData& d1, const StockData& d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mForeignCapChg < d2.mForeignCapChg;
}

bool StockData::sortBy5DayChgAsc(const StockData &d1, const StockData &d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mLast5DaysChgPers < d2.mLast5DaysChgPers;
}

bool StockData::sortBy5DayChgDesc(const StockData &d1, const StockData &d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mLast5DaysChgPers > d2.mLast5DaysChgPers;
}

bool StockData::sortBy10DayChgAsc(const StockData &d1, const StockData &d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mLast10DaysChgPers < d2.mLast10DaysChgPers;
}

bool StockData::sortBy10DayChgDesc(const StockData &d1, const StockData &d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mLast10DaysChgPers > d2.mLast10DaysChgPers;
}

bool StockData::sortByMonthChgAsc(const StockData &d1, const StockData &d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mLastMonthChgPers < d2.mLastMonthChgPers;
}

bool StockData::sortByMonthChgDesc(const StockData &d1, const StockData &d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mLastMonthChgPers > d2.mLastMonthChgPers;
}

bool StockData::sortByYearChgAsc(const StockData &d1, const StockData &d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return !d1.mIndexFlag;
    return d1.mChgPersFromYear < d2.mChgPersFromYear;
}

bool StockData::sortByYearChgDesc(const StockData &d1, const StockData &d2)
{
    if(d1.mIndexFlag ^ d2.mIndexFlag) return d1.mIndexFlag;
    return d1.mChgPersFromYear > d2.mChgPersFromYear;
}
