#include "blockdata.h"

BlockData::BlockData()
{

}

BlockData::~BlockData()
{

}

BlockDataList BlockData::BlockDataListFromCodesList(const QStringList &codes)
{
    BlockDataList list;
    foreach (QString code, codes) {
        BlockData *data = new BlockData;
        data->mCode = code;
        list.append(data);
    }

    return list;
}

QStringList BlockData::BlockCodsListFromBlockData(const BlockDataList &list)
{
    QStringList codes;
    foreach (BlockData *data, list) {
        codes.append(data->mCode);
    }

    return codes;
}

