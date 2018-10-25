#ifndef COMDATADEFINES_H
#define COMDATADEFINES_H

#include  <QString>
#include  <QVariant>
#include  <QApplication>

typedef enum enStockDisplayRule
{
    STK_DISPLAY_SORT_RULE_NONE = -1,
    STK_DISPLAY_SORT_RULE_DESCEND,
    STK_DISPLAY_SORT_RULE_ASCEND
}STK_DISPLAY_RULE;

typedef enum enStockDisplayType
{
    STK_DISPLAY_SORT_TYPE_CODE = -3,
    STK_DISPLAY_SORT_TYPE_NAME = -2,
    STK_DISPLAY_SORT_TYPE_NONE = -1,
    STK_DISPLAY_SORT_TYPE_PRICE = 0,
    STK_DISPLAY_SORT_TYPE_CHGPER,
    STK_DISPLAY_SORT_TYPE_CJE,
    STK_DISPLAY_SORT_TYPE_MONEYR,
    STK_DISPLAY_SORT_TYPE_LAST3,
    STK_DISPLAY_SORT_TYPE_LAST5,
    STK_DISPLAY_SORT_TYPE_LAST10,
    STK_DISPLAY_SORT_TYPE_LAST_MONTH,
    STK_DISPLAY_SORT_TYPE_LAST_YEAR,
    STK_DISPLAY_SORT_TYPE_ZJLX,
    STK_DISPLAY_SORT_TYPE_GXL,
    STK_DISPLAY_SORT_TYPE_SZZBL,
    STK_DISPLAY_SORT_TYPE_TCAP,
    STK_DISPLAY_SORT_TYPE_MCAP,
    STK_DISPLAY_SORT_TYPE_GQDJR,
    STK_DISPLAY_SORT_TYPE_YAGGR,
    STK_DISPLAY_SORT_TYPE_PROFIT,
    STK_DISPLAY_SORT_TYPE_FOREIGN_VOL,
    STK_DISPLAY_SORT_TYPE_FOREIGN_VOL_CHG,
    STK_DISPLAY_SORT_TYPE_FOREIGN_VOL_CHG3,
    STK_DISPLAY_SORT_TYPE_FOREIGN_VOL_CHG5,
    STK_DISPLAY_SORT_TYPE_FOREIGN_CAP,
    STK_DISPLAY_SORT_TYPE_FOREIGN_CAP_CHG,
    STK_DISPLAY_SORT_TYPE_HSL,
}STK_DISPLAY_TYPE;

typedef enum MktType
{
    MKT_ALL,        //沪深全部
    MKT_SH,         //上海
    MKT_SZ,         //深圳
    MKT_ZXB,        //中小板
    MKT_CYB,        //创业板
    MKT_JJ,         //沪深基金
    MKT_HK_HSZS,    //恒生指数
    MKT_HK_HSGQ,    //恒生国企
    MKT_HK_GGT,        //港股通（）
    MKT_ZXG,        //自选股
    MKT_LGT_TOP10,
    MKT_OTHER,      //保留
}MKT_TYPE;

typedef enum Page_Ctrl
{
    FIRST_PAGE = 0,
    PRE_PAGE,
    NEXT_PAGE,
    END_PAGE,
}PAGECTRL;

typedef enum Share_Detail_Info
{
    INFO_MINUTE_GRAPH = 0,
    INFO_K_GRAPH,
    INFO_HSHK,
    INFO_BLOCK_LIST,
    INFO_STOCK_LIST,
}STK_DETAIL;

struct  struMenu{
    QString     mDisplayText;
    QVariant    mCmd;
    QVariant    mKey;

    struMenu(const QString& text, QVariant val)
    {
        mDisplayText = text;
        mCmd = val;
    }
    struMenu()
    {
        mDisplayText = "";
    }
};

Q_DECLARE_METATYPE(struMenu)

typedef struct struTableColData
{
    STK_DISPLAY_RULE        mRule;
    STK_DISPLAY_TYPE        mType;
    QString                 mColStr;
    bool                    mIsDisplay;
    int                     mColNum;

    struTableColData()
    {
        mRule = STK_DISPLAY_SORT_RULE_DESCEND;
        mType = STK_DISPLAY_SORT_TYPE_NONE;
        mColStr = "";
        mIsDisplay = true;
        mColNum = -1;
    }
    struTableColData(const QString& col, STK_DISPLAY_TYPE type, bool isDispaly = true)
    {
        mColStr = col;
        mRule = STK_DISPLAY_SORT_RULE_DESCEND;
        mType = type;
        mIsDisplay = isDispaly;
        mColNum = -1;
    }
}TableColData;

typedef QList<TableColData>         TableColDataList;

#ifdef Q_OS_WIN
#define         HQ_APP_DIR                  QApplication::applicationDirPath()
#else
#define         HQ_APP_DIR                  QString("/sdcard/hq")
#endif

#define         HQ_CONFIG_DIR               QString("%1/etc/").arg(HQ_APP_DIR)
#define         HQ_DATA_DIR                 QString("%1/data/").arg(HQ_APP_DIR)
#define         HQ_CODE_DIR                 QString("%1%2/").arg(HQ_DATA_DIR).arg("code")
#define         HQ_HSTOP10_DIR              QString("%1%2/").arg(HQ_DATA_DIR).arg("hstop10")
#define         HQ_LGTHISTORY_DIR              QString("%1%2/").arg(HQ_DATA_DIR).arg("china_a_foreign")
#define         HQ_SHARE_HISTORY_DIR        QString("%1%2/").arg(HQ_DATA_DIR).arg("history")

#endif // COMDATADEFINES_H

