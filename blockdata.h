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
    QString         code;
    QString         name;
    double          changePer;
    double          mktkap; //市值
    double          hsl;
    int             stockUpNum; //上涨家数
    int             stockDownNum; //下跌家数
    int             updown; // 0持平 1 升 -1 降
    QStringList   stklist;

public:
    bool operator <(const BlockData& other)
    {
        return this->changePer < other.changePer;
    }

    bool operator >(const BlockData& other)
    {
        return this->changePer  > other.changePer;
    }

    bool operator ==(const BlockData& other)
    {
        return (this->code == other.code) && (this->changePer == other.changePer);
    }

};

Q_DECLARE_METATYPE(BlockData)
#endif // BLOCKDATA_H
