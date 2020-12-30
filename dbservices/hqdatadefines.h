#ifndef HQDATADEFINES
#define HQDATADEFINES

#include <QString>
#include <QStringList>
#include <QDate>
//#include "sharedata.h"

//表名
#define         TABLE_SHARE_HISTORY_TEMPLATE        "share_history_%1"
#define         TABLE_BLOCK                         "block"
#define         TABLE_SHARE_BASIC_INFO              "share_info"
#define         TABLE_HSGT_TOP10                    "share_hsgt_top10"
#define         TABLE_SHARE_BONUS                   "share_bonus"
#define         TABLE_SHARE_FINANCE                 "share_finance"
#define         TABLE_DB_UPDATE                     "share_db_update"
#define         TABLE_SHARE_HOLEDER                 "share_holder"
#define         TABLE_FAVORITE                      "share_favor"
#define         TABLE_PROFIT                        "share_profit"
#define         TABLE_BLOCK_SHARE                   "block_share"
#define         TABLE_SHARE_HISTORY_COUNTER                 "share_history_counter"
#define         TABLE_SHARE_EXCHANGE_RECORD         "exchange_record"

//列名
#define         HQ_TABLE_COL_ID                 "id"
#define         HQ_TABLE_COL_CODE               "code"
#define         HQ_TABLE_COL_NAME               "name"
#define         HQ_TABLE_COL_PY_ABBR            "abbr"
#define         HQ_TABLE_COL_TYPE               "type"
#define         HQ_TABLE_COL_LISTTIME           "list_time"
#define         HQ_TABLE_COL_PROFIT             "profit"
#define         HQ_TABLE_COL_TOTALMNT           "total_amount"
#define         HQ_TABLE_COL_MUTAL              "mutable_amount"
#define         HQ_TABLE_COL_CLOSE              "close"
#define         HQ_TABLE_COL_CLOSE_LAST             "last_close"
#define         HQ_TABLE_COL_CLOSE_LAST_ADJUST              "last_close_adjust"
#define         HQ_TABLE_COL_VOL                "vol"
#define         HQ_TABLE_COL_MONEY              "money"
#define         HQ_TABLE_COL_CHANGE_PERCENT     "change"
#define         HQ_TABLE_COL_DATE               "date"
#define         HQ_TABLE_COL_FAVORITE           "favorite"
#define         HQ_TABLE_COL_ZJLX               "zjlx"
#define         HQ_TABLE_COL_RZRQ               "rzrq"
#define         HQ_TABLE_COL_BLOCK_SHARE_LIST         "share_list"
#define         HQ_TABLE_COL_BLOCK_LIST0         "block_list"
#define         HQ_TABLE_COL_BLOCK_TYPE         "type"
#define         HQ_TABLE_COL_TABLE_NM           "name"


#define         HQ_TABLE_COL_HSGT_TOP10_VOL        "vol_foreign"
#define         HQ_TABLE_COL_HSGT_TOP10_MONEY      "foreign_money"
#define         HQ_TABLE_COL_HSGT_TOP10_BUY         "buy"
#define         HQ_TABLE_COL_HSGT_TOP10_SELL        "sell"
#define         HQ_TABLE_COL_HSGT_TOP10_FLAG             "hsgt"
#define         HQ_TABLE_COL_HSGT_HAVE       "foreign_amount"
#define         HQ_TABLE_COL_HSGT_HAVE_PERCENT  "foreign_percent"
#define         HQ_TABLE_COL_HSGT_HAVE_CHANGE  "foreign_change"

#define         HQ_TABLE_COL_HOLDER_CODE                "holder_code"
#define         HQ_TABLE_COL_HOLDER_NAME                "holder_name"
#define         HQ_TABLE_COL_HOLDER_VOL                 "holder_vol"
#define         HQ_TABLE_COL_HOLDER_FUND_PERCENT        "fund_percent"
#define         HQ_TABLE_COL_SHARE_CODE                 "share_code"
#define         HQ_TABLE_COL_HOLDER_DATE                 "date"


#define         HQ_TABLE_COL_FINANCE_MGSY               "MGSY"
#define         HQ_TABLE_COL_FINANCE_JZCSYL             "JZCSYL"
#define         HQ_TABLE_COL_FINANCE_JZC                "JZC"

#define         HQ_TABLE_COL_BONUS_YAGGR        "YAGGR"
#define         HQ_TABLE_COL_BONUS_GQDJR        "GQDJR"
#define         HQ_TABLE_COL_BONUS_SHARE        "share"
#define         HQ_TABLE_COL_BONUS_MONEY        "money"
#define         HQ_TABLE_COL_BONUS_DATE         "date"

#define         HQ_TABLE_COL_WEEK_CHANGE                        "chgweek"
#define         HQ_TABLE_COL_MONTH_CHANGE                       "chgmonth"
#define         HQ_TABLE_COL_YEAR_CHANGE                        "chgyear"
#define         HQ_TABLE_COL_FOREIGN_HOLD                       "foreign_vol"
#define         HQ_TABLE_COL_FOREIGN_HOLD_CHANGE_DAY            "foreign_chg_day"
#define         HQ_TABLE_COL_FOREIGN_HOLD_CHANGE_WEEK           "foreign_chg_week"
#define         HQ_TABLE_COL_FOREIGN_HOLD_CHANGE_MONTH          "foreign_chg_month"
#define         HQ_TABLE_COL_FOREIGN_HOLD_CHANGE_YEAR           "foreign_chg_year"
#define         HQ_TABLE_COL_EXCHANGE_TYPE                      "exchange_type"
#define         HQ_TABLE_COL_EXCHANGE_COUNT                       "exchange_count"
#define         HQ_TABLE_COL_EXCHANGE_PRICE                     "exchange_price"
#define         HQ_TABLE_COL_EXCHANGE_TOTAL                     "exchange_total"
#define         HQ_TABLE_COL_EXCHANGE_YONGJIN                   "exchange_yongjin"
#define         HQ_TABLE_COL_EXCHANGE_YINHUASUI                   "exchange_yinhuasui"
#define         HQ_TABLE_COL_EXCHANGE_OTHER                   "exchange_others"
#define         HQ_TABLE_COL_EXCHANGE_NETINCOME                     "echange_netincome"
#define         HQ_TABLE_COL_EXCHANGE_SERIAL_NUM                   "exchange_serial_num"



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

enum DB_DATA_TYPE{
    DB_INTEGER = 0,
    DB_VARCHAR,
    DB_BOOL,
    DB_NUMERIC,
};

struct TABLE_COL_DEF
{
public:

    QString         mName;
    int             mType;
    bool            mPrimaryKey;
    bool            mAutoIncrement;
    bool            mNull;
    int             mStrSize;

    TABLE_COL_DEF(const QString& name, int type,  bool _null = true, int strsize = 0, bool primary = false, bool autoinc = false)
    {
        mName = name;
        mType = type;
        mPrimaryKey = primary;
        mAutoIncrement = autoinc;
        mNull = _null;
        mStrSize = strsize;
    }

    TABLE_COL_DEF(const QString& name, const QString& def)
    {
        mStrSize = 0;
        mName = name;
        if(def.contains("INTEGER")) mType = DB_INTEGER;
        if(def.contains("BOOL")) mType = DB_BOOL;
        if(def.contains("NUMERIC")) mType = DB_NUMERIC;
        if(def.contains("VARCHAR"))
        {
            mType = DB_VARCHAR;
            int index = def.indexOf("(");
            if(index >= 0)
            {
                int end = def.indexOf(")", index);
                if(end >= 0)
                {
                    mStrSize = def.mid(index+ 1, end - index - 1).toInt();
                }
            }
        }
        mPrimaryKey = false;
        if(def.contains("PRIMARY") && def.contains("KEY")) mPrimaryKey = true;
        mAutoIncrement = false;
        if(def.contains("AUTOINCREMENT")) mAutoIncrement = true;
        mNull = true;
        if(def.contains("NOT NULL")) mNull = false;
    }

    TABLE_COL_DEF() {
        mName = "";
        mType = DB_INTEGER;
        mPrimaryKey = false;
        mAutoIncrement = false;
        mNull = true;
        mStrSize = 0;
    }

    QString toSTring() const
    {
        QStringList list;
        if(mType == DB_INTEGER)
        {
            list.append("INTEGER");
        } else if(mType == DB_BOOL)
        {
            list.append("BOOL");
        } else if(mType == DB_NUMERIC)
        {
            list.append("NUMERIC");
        } else if(mType == DB_VARCHAR)
        {
            list.append(QString("VARCHAR(%1)").arg(mStrSize));
        }

        if(mPrimaryKey)
        {
            list.append("PRIMARY KEY");
        }
        if(mAutoIncrement)
        {
            list.append("AUTOINCREMENT");
        }
        if(mNull)
        {
            list.append("NULL");
        } else
        {
            list.append("NOT NULL");
        }

        return list.join(" ");
    }

    bool operator==(const TABLE_COL_DEF& other)
    {
        return this->mName == other.mName && this->mType == other.mType
                && this->mPrimaryKey == other.mPrimaryKey
//                && this->mAutoIncrement == other.mAutoIncrement
                && this->mNull == other.mNull
                && this->mStrSize == other.mStrSize;
    }
};

typedef QList<TABLE_COL_DEF>        TableColList;

//struct FINANCE_DATA{
//    int mCode;          //将代码整数话
//    int mMGSY;          //每股收益*100;
//    int mMGJZC;         //每股净资产*100;
//    int mJZCSYL;        //净资产收益率*100;
//    int mXJFH;          //每股现金分红*10000
//    int mSZBL;          //10股送转比例*10
//    qint64 mTotalShare;
//    qint64 mMutalShare;
//    qint64 mYAGGR;
//    qint64 mGQDJR;
//};

//typedef QList<FINANCE_DATA>         FinDataList;

#endif // HQDATADEFINES

