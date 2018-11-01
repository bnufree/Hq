#ifndef HQBLOCKDATA_H
#define HQBLOCKDATA_H

#include "hqbasedata.h"
#include <QStringList>

class HQBlockData:public HqBaseData
{
public:
    HQBlockData();
    ~HQBlockData();
private:
    QStringList     mShareCodeList;
    int             mUpNum;
    int             mDownNum;
    int             mEqualNum;
    double          mTotalMoney;
    double          mPureInMoney;
    double          mCur;
    double          mLastClose;
    double          mChg;
    double          mChangePercent;
    double          mVol;//万手
    double          mMoney;//亿
    double          mZjlx;
    qint64          mTime;
};

#endif // HQBLOCKDATA_H
