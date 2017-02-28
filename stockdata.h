#ifndef STOCKDATA_H
#define STOCKDATA_H

#include "basedata.h"
#include <QStringList>

struct zjlxData{
    QString code;
    double zjlx;
};

class StockData : public BaseData
{
public:
    StockData();
    ~StockData();
    static bool sortByPerDesc(const StockData& d1, const StockData& d2);
    static bool sortByPerAsc(const StockData& d1, const StockData& d2);
    static bool sortByCurDesc(const StockData& d1, const StockData& d2);
    static bool sortByCurAsc(const StockData& d1, const StockData& d2);
    static bool sortByMonDesc(const StockData& d1, const StockData& d2);
    static bool sortByMonAsc(const StockData& d1, const StockData& d2);
    static bool sortByHslDesc(const StockData& d1, const StockData& d2);
    static bool sortByHslAsc(const StockData& d1, const StockData& d2);
    static bool sortByMonRatioDesc(const StockData& d1, const StockData& d2);
    static bool sortByMonRatioAsc(const StockData& d1, const StockData& d2);
    static bool sortBy3DayChgDesc(const StockData& d1, const StockData& d2);
    static bool sortBy3DayChgAsc(const StockData& d1, const StockData& d2);
    static bool sortBy3DayZjlxDesc(const StockData& d1, const StockData& d2);
    static bool sortBy3DayZjlxAsc(const StockData& d1, const StockData& d2);
    static bool sortByGxlDesc(const StockData& d1, const StockData& d2);
    static bool sortByGxlAsc(const StockData& d1, const StockData& d2);
    static bool sortBySzzblDesc(const StockData& d1, const StockData& d2);
    static bool sortBySzzblAsc(const StockData& d1, const StockData& d2);
    static bool sortByGqdjrDesc(const StockData& d1, const StockData& d2);
    static bool sortByGqdjrAsc(const StockData& d1, const StockData& d2);
    static bool sortByTcapDesc(const StockData& d1, const StockData& d2);
    static bool sortByTcapAsc(const StockData& d1, const StockData& d2);
    static bool sortByMcapDesc(const StockData& d1, const StockData& d2);
    static bool sortByMcapAsc(const StockData& d1, const StockData& d2);
public:
//    QString     name;
//    QString     code;
//    double      cur;
//    double      chg;
//    double      per;
//    int      money;
//    double      high;
//    double      low;
    double      zht;
    double      dit;
    int         vol;
    double         money;
    double      hsl;
    double      last_money;
    int         last_vol;
    double      last_per;
    double      last_three_pers;
    double      last_five_pers;
    double      open;
    double      last_close;
    qint64      totalshare;
    qint64      mutableshare;
    double      money_ratio;
    double      mkt;
    //QList<void*> blocklist;
    QStringList blocklist;
    QString     time;
    double      zjlx;
    double      szzbl;
    double      xjfh;
    double      gxl;
    QString     gqdjr; //股权登记日
    QString     yaggr;
    double      totalCap;
    double      mutalbleCap;

};

Q_DECLARE_METATYPE(StockData)
typedef QList<StockData>        StockDataList;
#endif // STOCKDATA_H
