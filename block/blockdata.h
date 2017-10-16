#ifndef BLOCKDATA_H
#define BLOCKDATA_H

#include <QString>
#include <QObject>
#include <QColor>

class BlockData
{
public:
    BlockData();
    ~BlockData();
public:
    QString             mName;
    QString             mCode;
    double              mChangePer;
    QString             mTopShareCode;
    QString             mTopShareName;
    double              mTopShareChangePer;
    QStringList         mShareCodeList;
    bool                mIsFav;

public:
    bool operator <(const BlockData& other)
    {
        return this->mChangePer < other.mChangePer;
    }

    bool operator >(const BlockData& other)
    {
        return this->mChangePer  > other.mChangePer;
    }

    bool operator ==(const BlockData& other)
    {
        return this->mCode == other.mCode;
    }

};

Q_DECLARE_METATYPE(BlockData)

typedef QList<BlockData*>        BlockDataList;


#endif // BLOCKDATA_H
