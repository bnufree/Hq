#include "hqblockdata.h"

HQBlockData::HQBlockData():HqBaseData()
{
    mShareCodeList.clear();
    mUpNum = 0;
    mDownNum = 0;
    mEqualNum = 0;
    mTotalMoney = 0;
    mCur = 0;
    mLastClose = 0;
    mChg = 0;
    mChangePercent = 0;
    mVol = 0;//万手
    mMoney = 0;//亿
    mZjlx = 0;
    mTime = 0;
}
