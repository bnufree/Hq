#ifndef HQDATADEFINES
#define HQDATADEFINES

#include <QString>
#include <QStringList>
#include <QDate>
//#include "sharedata.h"

//表名
#define         HQ_SHARE_HISTORY_INFO_TABLE           "Share_history"
#define         HQ_BLOCK_TABLE                      "block"
#define         HQ_SHARE_BASIC_INFO_TABLE                      "Share_info"
#define         HQ_HSGTOP10_TABLE                   "hsgTop10"
#define         HQ_GENERAL_TABLE                    "general_info"
#define         HQ_FINANCE_TABLE                    "finance"
#define         HQ_CLOSE_DATE_TABLE                 "close_date"

//列名
#define         HQ_TABLE_COL_ID                 "id"
#define         HQ_TABLE_COL_CODE               "code"
#define         HQ_TABLE_COL_NAME               "name"
#define         HQ_TABLE_COL_PY_ABBR            "abbr"
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
#define         HQ_TABLE_COL_TABLE_NM           "name"
#define         HQ_TABLE_COL_MGSY               "MGSY"
#define         HQ_TABLE_COL_JZCSYL             "JZCSYL"
#define         HQ_TABLE_COL_YAGGR              "YAGGR"
#define         HQ_TABLE_COL_GQDJR              "GQDJR"
#define         HQ_TABLE_COL_FHSP               "FHSP"

//typedef         ShareData               ShareData;
//typedef         ShareDataList           ShareDataList;
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

struct FINANCE_DATA{
    int mCode;          //将代码整数话
    int mMGSY;          //每股收益*100;
    int mMGJZC;         //每股净资产*100;
    int mJZCSYL;        //净资产收益率*100;
    int mXJFH;          //每股现金分红*10000
    int mSZBL;          //10股送转比例*10
    qint64 mTotalShare;
    qint64 mMutalShare;
    qint64 mYAGGR;
    qint64 mGQDJR;
};

typedef QList<FINANCE_DATA>         FinDataList;

#endif // HQDATADEFINES

