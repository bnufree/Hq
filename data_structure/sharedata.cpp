#include "sharedata.h"

ShareData::ShareData():HqBaseData()
{
    qRegisterMetaType<ShareDataList>("const ShareDataList&");
    mClose = 0.0;
    mHsl = 0.0;
    mMoneyRatio = 0;
    mZJLX = 0;     //资金流向
    mGXL = 0;   //股息率
    mTotalCap = 0;
    mMutalbleCap = 0;
    mForeignVolChg = 0;
    mForeignCap = 0;
    mForeignCapChg = 0;
    mCur = 0;
    mChg = 0;
    mChgPercent = 0;
    mHigh = 0;
    mLow = 0;
    mOpen = 0;
    mClose = 0;
    mMoney = 0;
    mRZRQ = 0;
    mVol = 0;
    mTime = 0;
}

bool ShareData::sortByPerDesc(const ShareData& d1, const ShareData& d2)
{
    return d1.mChgPercent > d2.mChgPercent;
}

bool ShareData::sortByPerAsc(const ShareData &d1, const ShareData &d2)
{
    return d1.mChgPercent < d2.mChgPercent;
}

bool ShareData::sortByCurDesc(const ShareData& d1, const ShareData& d2)
{
    return d1.mCur > d2.mCur;
}

bool ShareData::sortByCurAsc(const ShareData& d1, const ShareData& d2)
{
    return d1.mCur < d2.mCur;
}

bool ShareData::sortByMonDesc(const ShareData& d1, const ShareData& d2)
{
    return d1.mMoney > d2.mMoney;
}

bool ShareData::sortByMonAsc(const ShareData& d1, const ShareData& d2)
{
    return d1.mMoney < d2.mMoney;
}

bool ShareData::sortByHslDesc(const ShareData& d1, const ShareData& d2)
{
    return d1.mHsl > d2.mHsl;
}

bool ShareData::sortByHslAsc(const ShareData& d1, const ShareData& d2)
{
    return d1.mHsl < d2.mHsl;
}

bool ShareData::sortByMonRatioDesc(const ShareData& d1, const ShareData& d2)
{
    return d1.mMoneyRatio > d2.mMoneyRatio;
}

bool ShareData::sortByMonRatioAsc(const ShareData& d1, const ShareData& d2)
{
    return d1.mMoneyRatio < d2.mMoneyRatio;
}

bool ShareData::sortBy3DayChgAsc(const ShareData &d1, const ShareData &d2)
{
    return d1.mHistory.mLast3DaysChgPers < d2.mHistory.mLast3DaysChgPers;
}

bool ShareData::sortBy3DayChgDesc(const ShareData &d1, const ShareData &d2)
{
    return d1.mHistory.mLast3DaysChgPers > d2.mHistory.mLast3DaysChgPers;
}

bool ShareData::sortBy3DayZjlxAsc(const ShareData &d1, const ShareData &d2)
{
    return d1.mZJLX < d2.mZJLX;
}

bool ShareData::sortBy3DayZjlxDesc(const ShareData &d1, const ShareData &d2)
{
    return d1.mZJLX > d2.mZJLX;
}

bool ShareData::sortByGxlDesc(const ShareData& d1, const ShareData& d2)
{
    return d1.mGXL> d2.mGXL;
}

bool ShareData::sortByGxlAsc(const ShareData& d1, const ShareData& d2)
{
    return d1.mGXL < d2.mGXL;
}

bool ShareData::sortBySzzblDesc(const ShareData& d1, const ShareData& d2)
{
    return d1.mBonusData.mSZZG > d2.mBonusData.mSZZG;
}

bool ShareData::sortBySzzblAsc(const ShareData& d1, const ShareData& d2)
{
    return d1.mBonusData.mSZZG < d2.mBonusData.mSZZG;
}

bool ShareData::sortByGqdjrDesc(const ShareData& d1, const ShareData& d2)
{
    return d1.mBonusData.mGQDJR > d2.mBonusData.mGQDJR;
}

bool ShareData::sortByGqdjrAsc(const ShareData& d1, const ShareData& d2)
{
    return d1.mBonusData.mGQDJR < d2.mBonusData.mGQDJR;
}

bool ShareData::sortByTcapDesc(const ShareData& d1, const ShareData& d2)
{
    return d1.mTotalCap > d2.mTotalCap;
}

bool ShareData::sortByTcapAsc(const ShareData& d1, const ShareData& d2)
{
    return d1.mTotalCap < d2.mTotalCap;
}

bool ShareData::sortByMcapDesc(const ShareData& d1, const ShareData& d2)
{
    return d1.mMutalbleCap > d2.mMutalbleCap;
}

bool ShareData::sortByMcapAsc(const ShareData& d1, const ShareData& d2)
{
    return d1.mMutalbleCap < d2.mMutalbleCap;
}

bool ShareData::sortByProfitDesc(const ShareData& d1, const ShareData& d2)
{
    return d1.mProfit > d2.mProfit;
}

bool ShareData::sortByProfitAsc(const ShareData& d1, const ShareData& d2)
{
    return d1.mProfit < d2.mProfit;
}

bool ShareData::sortByForVolDesc(const ShareData& d1, const ShareData& d2)
{
    return d1.mHsgtData.mVol > d2.mHsgtData.mVol;
}

bool ShareData::sortByForVolAsc(const ShareData& d1, const ShareData& d2)
{
    return d1.mHsgtData.mVol < d2.mHsgtData.mVol;
}

bool ShareData::sortByForCapDesc(const ShareData& d1, const ShareData& d2)
{
    return d1.mForeignCap > d2.mForeignCap;
}

bool ShareData::sortByForCapAsc(const ShareData& d1, const ShareData& d2)
{
    return d1.mForeignCap < d2.mForeignCap;
}

bool ShareData::sortByForVolChgDesc(const ShareData& d1, const ShareData& d2)
{
    return d1.mForeignVolChg > d2.mForeignVolChg;
}

bool ShareData::sortByForVolChgAsc(const ShareData& d1, const ShareData& d2)
{
    return d1.mForeignVolChg < d2.mForeignVolChg;
}

bool ShareData::sortByForCapChgDesc(const ShareData& d1, const ShareData& d2)
{
    return d1.mForeignCapChg > d2.mForeignCapChg;
}

bool ShareData::sortByForCapChgAsc(const ShareData& d1, const ShareData& d2)
{
    return d1.mForeignCapChg < d2.mForeignCapChg;
}

bool ShareData::sortBy5DayChgAsc(const ShareData &d1, const ShareData &d2)
{
    return d1.mHistory.mLast5DaysChgPers < d2.mHistory.mLast5DaysChgPers;
}

bool ShareData::sortBy5DayChgDesc(const ShareData &d1, const ShareData &d2)
{
    return d1.mHistory.mLast5DaysChgPers > d2.mHistory.mLast5DaysChgPers;
}

bool ShareData::sortBy10DayChgAsc(const ShareData &d1, const ShareData &d2)
{
    return d1.mHistory.mLast10DaysChgPers < d2.mHistory.mLast10DaysChgPers;
}

bool ShareData::sortBy10DayChgDesc(const ShareData &d1, const ShareData &d2)
{
    return d1.mHistory.mLast10DaysChgPers > d2.mHistory.mLast10DaysChgPers;
}

bool ShareData::sortByMonthChgAsc(const ShareData &d1, const ShareData &d2)
{
    return d1.mHistory.mLastMonthChgPers < d2.mHistory.mLastMonthChgPers;
}

bool ShareData::sortByMonthChgDesc(const ShareData &d1, const ShareData &d2)
{
    return d1.mHistory.mLastMonthChgPers > d2.mHistory.mLastMonthChgPers;
}

bool ShareData::sortByYearChgAsc(const ShareData &d1, const ShareData &d2)
{
    return d1.mHistory.mChgPersFromYear < d2.mHistory.mChgPersFromYear;
}

bool ShareData::sortByYearChgDesc(const ShareData &d1, const ShareData &d2)
{
    return d1.mHistory.mChgPersFromYear > d2.mHistory.mChgPersFromYear;
}

bool ShareData::sortByDateAsc(const ShareData &d1, const ShareData &d2)
{
    return d1.mTime < d2.mTime;
}

bool ShareData::sortByDateDesc(const ShareData &d1, const ShareData &d2)
{
    return d1.mTime > d2.mTime;
}

