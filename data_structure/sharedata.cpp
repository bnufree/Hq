#include "sharedata.h"

int ShareData::stk_sort_type = STK_DISPLAY_SORT_TYPE_CHGPER;
int ShareData::stk_sort_rule = 1;

ShareData::ShareData():HqBaseData()
{
    qRegisterMetaType<ShareDataList>("const ShareDataList&");
    mProfit = 0.0;
}

bool ShareData::ShareSort(const ShareData &d1, const ShareData &d2)
{
    bool sts = true;
    switch (stk_sort_type) {
    case STK_DISPLAY_SORT_TYPE_CHGPER:
        sts = stk_sort_rule == 1? d1.mRealInfo.mChgPercent > d2.mRealInfo.mChgPercent : d1.mRealInfo.mChgPercent < d2.mRealInfo.mChgPercent;
        break;
    case STK_DISPLAY_SORT_TYPE_PRICE:
        sts = stk_sort_rule == 1? d1.mRealInfo.mClose > d2.mRealInfo.mClose : d1.mRealInfo.mClose < d2.mRealInfo.mClose;
        break;
    case STK_DISPLAY_SORT_TYPE_CJE:
        sts = stk_sort_rule == 1? d1.mRealInfo.mMoney > d2.mRealInfo.mMoney : d1.mRealInfo.mMoney < d2.mRealInfo.mMoney;
        break;
    case STK_DISPLAY_SORT_TYPE_HSL:
        sts = stk_sort_rule == 1? d1.mRealInfo.mHsl > d2.mRealInfo.mHsl : d1.mRealInfo.mHsl < d2.mRealInfo.mHsl;
        break;
    case STK_DISPLAY_SORT_TYPE_MONEYR:
        sts = stk_sort_rule == 1? d1.mRealInfo.mMoneyRatio > d2.mRealInfo.mMoneyRatio : d1.mRealInfo.mMoneyRatio < d2.mRealInfo.mMoneyRatio;
        break;
//    case STK_DISPLAY_SORT_TYPE_LAST3:
//        sts = stk_sort_rule == 1? d1.mHistory.mLast3DaysChgPers > d2.mHistory.mLast3DaysChgPers : d1.mHistory.mLast3DaysChgPers < d2.mHistory.mLast3DaysChgPers;
//        break;
    case STK_DISPLAY_SORT_TYPE_ZJLX:
        sts = stk_sort_rule == 1? d1.mRealInfo.mZJLX > d2.mRealInfo.mZJLX : d1.mRealInfo.mZJLX < d2.mRealInfo.mZJLX;
        break;
    case STK_DISPLAY_SORT_TYPE_GXL:
//        sts = stk_sort_rule == 1? d1.mGXL > d2.mGXL : d1.mGXL < d2.mGXL;
        break;
    case STK_DISPLAY_SORT_TYPE_SZZBL:
//        sts = stk_sort_rule == 1? d1.mBonusData.mSZZG > d2.mBonusData.mSZZG : d1.mBonusData.mSZZG < d2.mBonusData.mSZZG;
        break;
    case STK_DISPLAY_SORT_TYPE_GQDJR:
//        sts = stk_sort_rule == 1? d1.mBonusData.mGQDJR > d2.mBonusData.mGQDJR : d1.mBonusData.mGQDJR < d2.mBonusData.mGQDJR;
        break;
    case STK_DISPLAY_SORT_TYPE_TCAP:
        sts = stk_sort_rule == 1? d1.mRealInfo.mTotalCap > d2.mRealInfo.mTotalCap : d1.mRealInfo.mTotalCap < d2.mRealInfo.mTotalCap;
        break;
    case STK_DISPLAY_SORT_TYPE_MCAP:
        sts = stk_sort_rule == 1? d1.mRealInfo.mMutalbleCap > d2.mRealInfo.mMutalbleCap : d1.mRealInfo.mMutalbleCap < d2.mRealInfo.mMutalbleCap;
        break;
    case STK_DISPLAY_SORT_TYPE_PROFIT:
        sts = stk_sort_rule == 1? d1.mProfit > d2.mProfit : d1.mProfit < d2.mProfit;
        break;
//    case STK_DISPLAY_SORT_TYPE_FOREIGN_VOL:
//        sts = stk_sort_rule == 1? d1.mHsgtData.mVolTotal > d2.mHsgtData.mVolTotal : d1.mHsgtData.mVolTotal < d2.mHsgtData.mVolTotal;
//        break;
    case STK_DISPLAY_SORT_TYPE_FOREIGN_CAP:
        sts = stk_sort_rule == 1? d1.mForeignInfo.mCap > d2.mForeignInfo.mCap : d1.mForeignInfo.mCap < d2.mForeignInfo.mCap;
        break;
//    case STK_DISPLAY_SORT_TYPE_FOREIGN_VOL_CHG:
//        sts = stk_sort_rule == 1? d1.mHsgtData.volChg("1")> d2.mHsgtData.volChg("1") : d1.mHsgtData.volChg("1") < d2.mHsgtData.volChg("1");
//        break;
    case STK_DISPLAY_SORT_TYPE_FOREIGN_JMR_CHG:
        sts = stk_sort_rule == 1? d1.mForeignInfo.mJMR1 > d2.mForeignInfo.mJMR1 :d1.mForeignInfo.mJMR1 < d2.mForeignInfo.mJMR1;
        break;
//    case STK_DISPLAY_SORT_TYPE_LAST5:
//        sts = stk_sort_rule == 1? d1.mHistory.mLast5DaysChgPers > d2.mHistory.mLast5DaysChgPers : d1.mHistory.mLast5DaysChgPers < d2.mHistory.mLast5DaysChgPers;
//        break;
//    case STK_DISPLAY_SORT_TYPE_LAST10:
//        sts = stk_sort_rule == 1? d1.mHistory.mLast10DaysChgPers > d2.mHistory.mLast10DaysChgPers : d1.mHistory.mLast10DaysChgPers < d2.mHistory.mLast10DaysChgPers;
//        break;
    case STK_DISPLAY_SORT_TYPE_LAST_MONTH:
        sts = stk_sort_rule == 1?  d1.mCounterInfo.mMonthChgPer > d2.mCounterInfo.mMonthChgPer : d1.mCounterInfo.mMonthChgPer < d2.mCounterInfo.mMonthChgPer;
        break;
    case STK_DISPLAY_SORT_TYPE_LAST_WEEK:
        sts = stk_sort_rule == 1?  d1.mCounterInfo.mWeekChgPer > d2.mCounterInfo.mWeekChgPer : d1.mCounterInfo.mWeekChgPer < d2.mCounterInfo.mWeekChgPer;
        break;
    case STK_DISPLAY_SORT_TYPE_LAST_YEAR:
        sts = stk_sort_rule == 1? d1.mCounterInfo.mYearChgPer > d2.mCounterInfo.mYearChgPer : d1.mCounterInfo.mYearChgPer < d2.mCounterInfo.mYearChgPer;
        break;
    case STK_DISPLAY_SORT_TYPE_JZCSYL:
//        sts = stk_sort_rule == 1? d1.mFinanceData.mROE > d2.mFinanceData.mROE : d1.mFinanceData.mROE < d2.mFinanceData.mROE;
        break;
    case STK_DISPLAY_SORT_TYPE_FOREIGN_JMR_CHG5:
        sts = stk_sort_rule == 1? d1.mForeignInfo.mJMR5> d2.mForeignInfo.mJMR5 : d1.mForeignInfo.mJMR5< d2.mForeignInfo.mJMR5;
        break;
    case STK_DISPLAY_SORT_TYPE_FOREIGN_JMR_CHG10:
        sts = stk_sort_rule == 1? d1.mForeignInfo.mJMR10 > d2.mForeignInfo.mJMR10 : d1.mForeignInfo.mJMR10 < d2.mForeignInfo.mJMR10;
        break;
    default:
        break;
    }
        return sts;
}

