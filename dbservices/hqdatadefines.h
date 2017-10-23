#ifndef HQDATADEFINES
#define HQDATADEFINES

#include <QString>
#include <QStringList>
#include <QDate>
#include "stockdata.h"

//表名
#define         HQ_SHARE_BASIC_INFO_TABLE           "stock_base_info"


//列名
#define         HQ_TABLE_COL_ID                 "id"
#define         HQ_TABLE_COL_CODE               "code"
#define         HQ_TABLE_COL_NAME               "name"
#define         HQ_TABLE_COL_PROFIT             "profit"
#define         HQ_TABLE_COL_TOTALMNT           "total_amount"
#define         HQ_TABLE_COL_MUTAL              "mutable_amount"

struct BlockBaseData{
    int                 mCode;
    int                 mType;              //
    QString             mName;
    QStringList         mShareCodesList;
};

struct BlockRealInfo:public BlockBaseData{
    double              mCurPrice;
    double              mChange;
    double              mChangePercent;
    double              mZjlx;
    QDate               mDate;
};

class ChinaShareExchange:public StockData
{
public:
    QDate               mDate;
    double              mTop10Buy;
    double              mTop10Sell;
    double              mTop10Vol;
    double              mTop10TotalVol;
    double              mTop10TotalCap;
    double              mTop10AvgCost;
};

struct  foreignHolder{
    qint64 last;
    qint64 previous;

    foreignHolder()
    {
        last = 0;
        previous = 0;
    }
};
#endif // HQDATADEFINES

