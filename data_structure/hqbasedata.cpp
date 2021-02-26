#include "hqbasedata.h"

HqBaseData::HqBaseData()
{
    mIsFav = false;
    mType = HqBaseData::UNDEFINED;
    mCode.clear();
    mName.clear();
    mPY.clear();
    mStatus = true;
    mListTime.clear();
    mZGB = 0.0;
    mLTGB = 0.0;
    mReferCodeList.clear();
}

HqBaseData::HqBaseData(const HqBaseData &data)
{
    mIsFav = data.mIsFav;
    mType = data.mType;
    mCode = data.mCode;
    mName = data.mName;
    mPY = data.mPY;
    mStatus = data.mStatus;
    mListTime = data.mListTime;
    mZGB = data.mZGB;
    mLTGB = data.mLTGB;
    mReferCodeList = data.mReferCodeList;
}

HqBaseData::HqBaseData(const QString &code, const QString &name, const QString &py, int type, bool isFav)
{
    mIsFav = isFav;
    mCode = code;
    mName = name;
    mPY = py;
    mType = type;
    mStatus = true;
    mListTime.clear();
    mZGB = 0.0;
    mLTGB = 0.0;
    mReferCodeList.clear();
}

HqBaseData& HqBaseData::operator =(const HqBaseData& data)
{
    if(this == &data) return *this;
    mIsFav = data.mIsFav;
    mType = data.mType;
    mCode = data.mCode;
    mName = data.mName;
    mPY = data.mPY;
    mStatus = data.mStatus;
    return *this;
}
