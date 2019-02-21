#ifndef HQBLOCKDATA_H
#define HQBLOCKDATA_H

#include "hqbasedata.h"
#include <QStringList>

typedef enum BlockType
{
    BLOCK_NONE= 0,
    BLOCK_GN = 1,
    BLOCK_HY = 2,
    BLOCK_DQ = 4,
    BLOCK_USER = 8,
}BLOCKTYPE;

struct HQBlockData:public HqBaseData
{
public:
    HQBlockData();

    static bool sortByChangeAsc(const HQBlockData& d1, const HQBlockData& d2)
    {
        return d1.mChangePercent < d2.mChangePercent;
    }

    static bool sortByChangeDesc(const HQBlockData& d1, const HQBlockData& d2)
    {
        return d1.mChangePercent > d2.mChangePercent;
    }

    bool operator <(const HQBlockData& other)
    {
        return this->mChangePercent < other.mChangePercent;
    }

    bool operator >(const HQBlockData& other)
    {
        return this->mChangePercent  > other.mChangePercent;
    }

    bool operator ==(const HQBlockData& other)
    {
        return this->mCode == other.mCode && this->mTime == other.mTime;
    }

public:
    QStringList     mShareCodeList;
    int             mBlockType;
    int             mUpNum;
    int             mDownNum;
    int             mEqualNum;
    double          mTotalMoney;
    double          mCur;
    double          mLastClose;
    double          mChg;
    double          mChangePercent;
    double          mVol;//万手
    double          mMoney;//亿
    double          mZjlx;
    BlockDateTime          mTime;
};




typedef         HQBlockData             BlockData;
typedef         QList<HQBlockData>      BlockDataList;
typedef         QMap<QString, BlockData>    BlockDataMap;

Q_DECLARE_METATYPE(HQBlockData)
Q_DECLARE_METATYPE(BlockDataList)

#endif // HQBLOCKDATA_H
