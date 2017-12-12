#ifndef HQDATADEFINES
#define HQDATADEFINES

#include <QString>
#include <QStringList>
#include <QDate>
#include "stockdata.h"

//表名
#define         HQ_SHARE_HISTORY_INFO_TABLE           "stock_"
#define         HQ_BLOCK_TABLE                      "block"
#define         HQ_SHARE_TABLE                      "stock"


//列名
#define         HQ_TABLE_COL_ID                 "id"
#define         HQ_TABLE_COL_CODE               "code"
#define         HQ_TABLE_COL_NAME               "name"
#define         HQ_TABLE_COL_PROFIT             "profit"
#define         HQ_TABLE_COL_TOTALMNT           "total_amount"
#define         HQ_TABLE_COL_MUTAL              "mutable_amount"
#define         HQ_TABLE_COL_CLOSE              "close"
#define         HQ_TABLE_COL_VOL                "vol"
#define         HQ_TABLE_COL_MONEY              "money"
#define         HQ_TABLE_COL_CHANGE_PERCENT     "change"
#define         HQ_TABLE_COL_DATE               "date"
#define         HQ_TABLE_COL_HSGT_TOP10_VOL        "vol_foreign"
#define         HQ_TABLE_COL_FAVORITE           "favorite"
#define         HQ_TABLE_COL_ZJLX               "zjlx"
#define         HQ_TABLE_COL_RZRQ               "rzrq"
#define         HQ_TABLE_COL_HSGT_TOP10_MONEY      "foreign_money"
#define         HQ_TABLE_COL_HSGT_TOP10         "hsgt"
#define         HQ_TABLE_COL_HSGT_HAVE       "foreign_amount"
#define         HQ_TABLE_COL_SHARE_LIST         "share_list"
#define         HQ_TABLE_COL_BLOCK_LIST         "block_list"
#define         HQ_TABLE_COL_BLOCK_TYPE         "type"

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

struct TABLE_COL_DEF
{
    QString mKey;
    QString mDef;
};

typedef QList<TABLE_COL_DEF>        TableColList;

#endif // HQDATADEFINES

