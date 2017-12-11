#include "blockdata.h"
#include <QDebug>

BlockData::BlockData()
{
    mBlockType = BLOCK_NONE;

}

BlockData::~BlockData()
{
    qDebug()<<"block data descontruction.";
}

//BlockDataList BlockData::BlockDataListFromCodesList(const QStringList &codes)
//{
//    BlockDataList list;
//    foreach (QString code, codes) {
//        BlockData *data = new BlockData;
//        data->mCode = code;
//        list.append(data);
//    }

//    return list;
//}

//QStringList BlockData::BlockCodsListFromBlockData(const BlockDataList &list)
//{
//    QStringList codes;
//    foreach (BlockData *data, list) {
//        codes.append(data->mCode);
//    }

//    return codes;
//}

