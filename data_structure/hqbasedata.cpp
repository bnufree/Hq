#include "hqbasedata.h"

HqBaseData::HqBaseData()
{
    mIsFav = false;
    mType = HqBaseData::UNDEFINED;
}

HqBaseData::HqBaseData(const HqBaseData &data)
{
    mIsFav = data.mIsFav;
    mType = data.mType;
    mCode = data.mCode;
    mName = data.mName;
    mPY = data.mPY;
}

HqBaseData::HqBaseData(const QString &code, const QString &name, const QString &py, int type, bool isFav)
{
    mIsFav = isFav;
    mCode = code;
    mName = name;
    mPY = py;
    mType = type;
}

HqBaseData& HqBaseData::operator =(const HqBaseData& data)
{
    if(this == &data) return *this;
    mIsFav = data.mIsFav;
    mType = data.mType;
    mCode = data.mCode;
    mName = data.mName;
    mPY = data.mPY;
    return *this;
}
