#include "sharedata.h"

int ShareData::stk_sort_type = STK_DISPLAY_SORT_TYPE_CHGPER;
int ShareData::stk_sort_rule = 1;

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

bool ShareData::ShareSort(const ShareData &d1, const ShareData &d2)
{
    bool sts = true;
    switch (stk_sort_type) {
    case STK_DISPLAY_SORT_TYPE_CHGPER:
        sts = stk_sort_rule == 1? d1.mChgPercent > d2.mChgPercent : d1.mChgPercent < d2.mChgPercent;
        break;
    case STK_DISPLAY_SORT_TYPE_PRICE:
        sts = stk_sort_rule == 1? d1.mCur > d2.mCur : d1.mCur < d2.mCur;
        break;
    case STK_DISPLAY_SORT_TYPE_CJE:
        sts = stk_sort_rule == 1? d1.mMoney > d2.mMoney : d1.mMoney < d2.mMoney;
        break;
    case STK_DISPLAY_SORT_TYPE_HSL:
        sts = stk_sort_rule == 1? d1.mHsl > d2.mHsl : d1.mHsl < d2.mHsl;
        break;
    case STK_DISPLAY_SORT_TYPE_MONEYR:
        sts = stk_sort_rule == 1? d1.mMoneyRatio > d2.mMoneyRatio : d1.mMoneyRatio < d2.mMoneyRatio;
        break;
    case STK_DISPLAY_SORT_TYPE_LAST3:
        sts = stk_sort_rule == 1? d1.mHistory.mLast3DaysChgPers > d2.mHistory.mLast3DaysChgPers : d1.mHistory.mLast3DaysChgPers < d2.mHistory.mLast3DaysChgPers;
        break;
    case STK_DISPLAY_SORT_TYPE_ZJLX:
        sts = stk_sort_rule == 1? d1.mZJLX > d2.mZJLX : d1.mZJLX < d2.mZJLX;
        break;
    case STK_DISPLAY_SORT_TYPE_GXL:
        sts = stk_sort_rule == 1? d1.mGXL > d2.mGXL : d1.mGXL < d2.mGXL;
        break;
    case STK_DISPLAY_SORT_TYPE_SZZBL:
        sts = stk_sort_rule == 1? d1.mBonusData.mSZZG > d2.mBonusData.mSZZG : d1.mBonusData.mSZZG < d2.mBonusData.mSZZG;
        break;
    case STK_DISPLAY_SORT_TYPE_GQDJR:
        sts = stk_sort_rule == 1? d1.mBonusData.mGQDJR > d2.mBonusData.mGQDJR : d1.mBonusData.mGQDJR < d2.mBonusData.mGQDJR;
        break;
    case STK_DISPLAY_SORT_TYPE_TCAP:
        sts = stk_sort_rule == 1? d1.mTotalCap > d2.mTotalCap : d1.mTotalCap < d2.mTotalCap;
        break;
    case STK_DISPLAY_SORT_TYPE_MCAP:
        sts = stk_sort_rule == 1? d1.mMutalbleCap > d2.mMutalbleCap : d1.mMutalbleCap < d2.mMutalbleCap;
        break;
    case STK_DISPLAY_SORT_TYPE_PROFIT:
        sts = stk_sort_rule == 1? d1.mProfit > d2.mProfit : d1.mProfit < d2.mProfit;
        break;
    case STK_DISPLAY_SORT_TYPE_FOREIGN_VOL:
        sts = stk_sort_rule == 1? d1.mHsgtData.mVolTotal > d2.mHsgtData.mVolTotal : d1.mHsgtData.mVolTotal < d2.mHsgtData.mVolTotal;
        break;
    case STK_DISPLAY_SORT_TYPE_FOREIGN_CAP:
        sts = stk_sort_rule == 1? d1.mForeignCap > d2.mForeignCap : d1.mForeignCap < d2.mForeignCap;
        break;
    case STK_DISPLAY_SORT_TYPE_FOREIGN_VOL_CHG:
        sts = stk_sort_rule == 1? d1.mHsgtData.mVolCh1> d2.mHsgtData.mVolCh1 : d1.mHsgtData.mVolCh1 < d2.mHsgtData.mVolCh1;
        break;
    case STK_DISPLAY_SORT_TYPE_FOREIGN_CAP_CHG:
        sts = stk_sort_rule == 1? d1.mForeignCapChg > d2.mForeignCapChg :d1.mForeignCapChg < d2.mForeignCapChg;
        break;
    case STK_DISPLAY_SORT_TYPE_LAST5:
        sts = stk_sort_rule == 1? d1.mHistory.mLast5DaysChgPers > d2.mHistory.mLast5DaysChgPers : d1.mHistory.mLast5DaysChgPers < d2.mHistory.mLast5DaysChgPers;
        break;
    case STK_DISPLAY_SORT_TYPE_LAST10:
        sts = stk_sort_rule == 1? d1.mHistory.mLast10DaysChgPers > d2.mHistory.mLast10DaysChgPers : d1.mHistory.mLast10DaysChgPers < d2.mHistory.mLast10DaysChgPers;
        break;
    case STK_DISPLAY_SORT_TYPE_LAST_MONTH:
        sts = stk_sort_rule == 1?  d1.mHistory.mChgPersFromMonth > d2.mHistory.mChgPersFromMonth : d1.mHistory.mChgPersFromMonth < d2.mHistory.mChgPersFromMonth;
        break;
    case STK_DISPLAY_SORT_TYPE_LAST_WEEK:
        sts = stk_sort_rule == 1?  d1.mHistory.mChgPersFromWeek > d2.mHistory.mChgPersFromWeek : d1.mHistory.mChgPersFromWeek < d2.mHistory.mChgPersFromWeek;
        break;
    case STK_DISPLAY_SORT_TYPE_LAST_YEAR:
        sts = stk_sort_rule == 1? d1.mHistory.mChgPersFromYear > d2.mHistory.mChgPersFromYear : d1.mHistory.mChgPersFromYear < d2.mHistory.mChgPersFromYear;
        break;
    case STK_DISPLAY_SORT_TYPE_JZCSYL:
        sts = stk_sort_rule == 1? d1.mFinanceData.mROE > d2.mFinanceData.mROE : d1.mFinanceData.mROE < d2.mFinanceData.mROE;
        break;
    case STK_DISPLAY_SORT_TYPE_FOREIGN_VOL_CHG5:
        sts = stk_sort_rule == 1? d1.mHsgtData.mVolCh5> d2.mHsgtData.mVolCh5 : d1.mHsgtData.mVolCh5 < d2.mHsgtData.mVolCh5;
        break;
    case STK_DISPLAY_SORT_TYPE_FOREIGN_VOL_CHG10:
        sts = stk_sort_rule == 1? d1.mHsgtData.mVolCh10 > d2.mHsgtData.mVolCh10 : d1.mHsgtData.mVolCh10 < d2.mHsgtData.mVolCh10;
        break;
    default:
        break;
    }
        return sts;
}

