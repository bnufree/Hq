#ifndef STOCKDATA_H
#define STOCKDATA_H

#include "basedata.h"
#include <QStringList>
#include <QDate>

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
    static bool sortByProfitDesc(const StockData& d1, const StockData& d2);
    static bool sortByProfitAsc(const StockData& d1, const StockData& d2);

    bool operator ==(const StockData& data)
    {
        return this->code == data.code;
    }

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
    qint64         vol;
    double         money;
    double      hsl;
    double      last_money;
    int         last_vol;
    double      last_per;
    double      last_3day_pers;
    double      last_5day_pers;
    double      last_10day_pers;
    double      last_month_pers;
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
    double      szzbl; //送转股比例
    double      xjfh;  //现金分红
    double      gxl;   //股息率
    QDate     gqdjr; //股权登记日
    QDate     yaggr; //预案公告日
    double      totalCap;
    double      mutalbleCap;
    bool        index;
    QDate       date; //当前的日期
    double      profit;

};

Q_DECLARE_METATYPE(StockData)
typedef QList<StockData>        StockDataList;
#endif // STOCKDATA_H
