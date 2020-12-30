#ifndef HQDBDATABASE_H
#define HQDBDATABASE_H

#include <QObject>
#include <QtSql>
#include <QMutex>
#include "hqdatadefines.h"
#include "data_structure/hqblockdata.h"
#include "data_structure/sharedata.h"
#include "utils/comdatadefines.h"

enum HISTORY_CHANGEPERCENT{
    DAYS_3 = 3,
    DAYS_5 = 5,
    DAYS_10 = 10,
    DAYS_MONTH = 20,
    DAYS_HALF_YEAR = 120,
    DAYS_YEARS = 240,
};

enum    HQ_DATA_TYPE{
    HQ_DATA_TEXT = 1,
    HQ_DATA_INT,
    HQ_DATA_DOUBLE,
    HQ_DATA_LONG,
};

enum    HQ_DATA_COMPARE{
    HQ_COMPARE_EQUAL = 0,
    HQ_COMPARE_LESS,
    HQ_COMPARE_GREAT,
    HQ_COMPARE_STRLIKE,
    HQ_COMPARE_TEXTIN,
    HQ_COMPARE_LESS_EQUAL,
    HQ_COMPARE_GREAT_EQUAL,
};

struct HQ_COL_VAL{
    QVariant mValue;
    int mType;

    HQ_COL_VAL()
    {
        mType = HQ_DATA_INT;
    }

    HQ_COL_VAL(const QVariant& val, HQ_DATA_TYPE t)
    {
        mType = t;
        mValue = val;
    }
};


Q_DECLARE_METATYPE(HQ_COL_VAL)

typedef class HQ_QUERY_CONDITION{
public:
    QString  mColName;
    HQ_COL_VAL  mColVal;
    HQ_DATA_COMPARE mColCompare;

    HQ_QUERY_CONDITION(const QString colStr, const QVariant& val, HQ_DATA_TYPE t, HQ_DATA_COMPARE compare = HQ_COMPARE_EQUAL)
    {
        mColName = colStr;
        mColVal.mType = t;
        mColVal.mValue = val;
        mColCompare = compare;
    }

    HQ_QUERY_CONDITION(){}
}DBColVal;

class DBPlacementList:public QStringList
{
public:
    DBPlacementList():QStringList(){}
    DBPlacementList(int num){
        for(int i=0; i<num; i++)
        {
            append("?");
        }
    }
};

class DBColValList:public QList<HQ_QUERY_CONDITION>
{
public:
    DBColValList():QList<HQ_QUERY_CONDITION>() {}
    DBColValList(const HQ_QUERY_CONDITION& val) : QList<HQ_QUERY_CONDITION>() {append(val);}
    DBColValList(const QList<HQ_QUERY_CONDITION>& list) : QList<HQ_QUERY_CONDITION>(list) {}
    QString insertString() const;
    QString updateString() const;
    QString whereString() const;
};

enum Share_History_Mode{
    History_none = 0x00,
    History_Zjlx = 0x01,
    History_Close = 0x02,
    History_HsgtVol = 0x04,
    History_HsgtTop10 = 0x08,
    History_Rzrq = 0x10,
    History_All = History_Zjlx | History_Close|History_HsgtVol|History_HsgtTop10|History_Rzrq,
};



class HQDBDataBase : public QObject
{
    Q_OBJECT
public:
    explicit HQDBDataBase(QObject *parent = 0);
    ~HQDBDataBase();
    QString getErrorString();
    QDate getLastUpdateDateOfTable(const QString &table);
    QDate getLastHistoryDateOfShare(/*const QString& code*/);
    bool isDBOK();
    //板块
    bool queryBlockDataList(BlockDataMap& list, int type = 0);
    bool updateBlockDataList(const BlockDataList& list );
    bool deleteBlock(const QString& code = QString());
    bool isBlockExist(const QString& code);
    //个股
    //基本信息更新


    bool createDBTables();
    QString errMsg();

    //基本数据更新
    bool updateShareBasicInfo(const ShareDataList& dataList);
    bool queryShareBasicInfo(ShareDataMap& map, const QStringList& favlist);
    bool delShareBasicInfo(const QString& code = QString());
    bool getSimilarCodeOfText(QStringList& codes, const QString& text);

    //自选股更新
    bool updateShareFavInfo(const ShareDataList& dataList);
    bool queryShareFavInfo(ShareDataMap& map);
    bool delShareFavInfo(const QString& code);
    //利润表更新
    bool updateShareProfitInfo(const ShareDataList& dataList);
    bool queryShareProfitInfo(ShareDataMap& map);
    bool delShareProfitInfo(const QString& code);
    //板块明细更新
    bool updateShareBlockInfo(const BlockDataList& dataList);
    bool queryShareBlockInfo(ShareDataMap& share, BlockDataMap& block);
    bool delShareBlockInfo(const QString& code);

    //历史日线数据更新
    bool queryShareHistroyUpdatedDates(QList<QDate>& list);
    bool updateShareHistory(const ShareDataList& dataList, int mode);
    bool queryShareHistory(ShareDataList& list, const QString& share_code, const QDate& start = QDate(), const QDate& end = QDate());
    bool delShareHistory(const QString& share_code, const QDate& start, const QDate& end);
    double getMultiDaysChangePercent(const QString &code, HISTORY_CHANGEPERCENT type );
    double getLastMoney(const QString& code);
    bool   getLastForeignVol(qint64& vol, qint64& vol_chg, const QString& code);
    //获取指定日期的成交数据
    bool queryHistoryInfoFromDate(ShareDataList& list, const QString& code, const QDate& date);

    //股东明细信息信息
    bool updateShareHolder(const ShareHolderList& dataList);
    bool queryShareHolder(ShareHolderList& list, const QString& share_code, const QString& holder_code, const QDate& date);
    bool delShareHolder(const QString& share_code, const QString& holder_code, const QDate& date);

    //财务信息操作
    bool updateShareFinance(const FinancialDataList& dataList);
    bool queryShareFinance(FinancialDataList& list, const QStringList& codes);
    bool delShareFinance(const QString& code);

    //分红送配信息操作
    bool updateShareBonus(const ShareBonusList& bonusList);
    bool queryShareBonus(QList<ShareBonus>& list, const QString& code, const QDate& date);
    bool delShareBonus(const QString& code, const QDate& date);

    //数据表更新日期操作
    bool updateDBUpdateDate(const QDate& date, const QString& table);
    bool queryDBUpdateDate(QDate& date, const QString& table);
    bool delDBUpdateDate(const QString& table);

    //沪深港信息TOP10
    bool updateShareHsgtTop10List(const ShareHsgtList& dataList);
    bool queryShareHsgtTop10List(ShareHsgtList& list, const QString& code, const QDate& date);
    bool delShareHsgtTop10(const QString& code, const QDate& date);

    //明细
    bool updateExhangeRecordList(const QList<ShareExchangeData>& list);
    bool queryExchangeRecord(QList<ShareExchangeData>& list, int& total_page, int page, const QString& code, const QString& start_date, const QString& end_date);
    bool deleteExchangeRecord(const QString& code, const QString& start_date, const QString& end_date);




private:
    bool initSqlDB();
    //创建数据库需要的各类表
    TableColList queryTableDef(const QString& table);
    //板块明细
    bool createBlockTable();
    //基本信息
    bool createShareBasicTable();
    bool createShareFavoriteTable();
    bool createShareProfitTable();
    bool createShareBlockTable();
    bool createShareExchangeRecordTable();
    //历史日线数据表
    bool createShareHistoryInfoTable(const QString& code);
    //股东明细表
    bool createShareHoldersTable();
    //财务信息表
    bool createShareFinancialInfoTable();
    //分红送配信息表
    bool createShareBonusIbfoTable();
    //沪深港TOP10
    bool createShareHsgTop10Table();
    //
    bool createShareHistoryCounterTable();
    //各个表的最后更新日期
    bool createDBUpdateDateTable();

    bool tableExist(const QString &pTable);
    bool createTable(const QString& pTable, const TableColList& cols);
    bool deleteTable(const QString& pTable);

    //表的通用操作
    bool updateTable(const QString& tableName, const DBColValList& values, const DBColValList& key, bool check = true );
    bool deleteRecord(const QString& table, const DBColValList& list = DBColValList());    
    bool isRecordExist(bool& exist, const QString& table, const DBColValList& list);

signals:

public slots:
private:
    QSqlDatabase        mDB;
    QSqlQuery           mSQLQuery;
    QMutex              mSQLMutex;
    bool                mInitDBFlg;
};

#endif // HQDBDATABASE_H
