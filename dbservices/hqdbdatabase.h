#ifndef HQDBDATABASE_H
#define HQDBDATABASE_H

#include <QObject>
#include <QtSql>
#include <QMutex>
#include "hqdatadefines.h"
#include "utils/blockdata.h"
#include "utils/sharedata.h"
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
    HQ_DATA_DOUBLE
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

typedef struct HQ_QUERY_CONDITION{
    QString  mColName;
    HQ_COL_VAL  mColVal;

    HQ_QUERY_CONDITION(const QString colStr, const QVariant& val, HQ_DATA_TYPE t)
    {
        mColName = colStr;
        mColVal.mType = t;
        mColVal.mValue = val;
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
    QString insertString() const;
    QString updateString() const;
    QString whereString() const;
};



class HQDBDataBase : public QObject
{
    Q_OBJECT
public:
    explicit HQDBDataBase(QObject *parent = 0);
    ~HQDBDataBase();
    QString getErrorString();
    ShareDate getLastUpdateDateOfTable(const QString &table);
    bool  updateDateOfTable(const QString& table);
    bool isDBOK();
    //板块
    bool getBlockDataList(QMap<QString, BlockData*>& pBlockMap, int type = 0);
    bool saveBlockDataList(const QMap<QString, BlockData*>& pBlockMap );
    bool addBlock(const BlockData& data);
    bool modifyBlock(const BlockData& data);
    bool deleteBlock(const QString& code);
    bool isBlockExist(const QString& code);
    //个股
    //基本信息更新
    bool getBasicShareDataList(QMap<QString, ShareData*>& pShareMap);
    bool updateBasicShareDataList(QList<ShareData*> dataList);
    bool updateBasicShare(const ShareData& data, bool exist);
    bool clearAndUpdateBasicShareDataList(QList<ShareData*> dataList);
    bool updateHistoryShare(const ShareData& data, bool exist);
    bool deleteShare(const QString& table, const QString& col = QString(), const QVariant& val = QVariant());
    bool updateHistoryDataList(const ShareDataList& list, bool delDB);
    double getMultiDaysChangePercent(const QString &code, HISTORY_CHANGEPERCENT type );
    double getLastMoney(const QString& code);
    bool   getLastForeignVol(qint64& vol, qint64& vol_chg, const QString& code);

    bool createDBTables();
    QString errMsg();
    bool getHistoryDataOfCode(ShareDataList& list, const QString &code);
    bool getSimilarCodeOfText(QStringList& codes, const QString& text);
    //历史日线数据更新
    bool updateShareHistory(const ShareDataList& dataList);
    bool queryShareHistory(ShareDataList& list, const QString& share_code, const ShareDate& start, const ShareDate& end);
    bool delShareHistory(const QString& share_code, const ShareDate& start, const ShareDate& end);

    //股东明细信息信息
    bool updateShareHolder(const ShareHolderList& dataList);
    bool queryShareHolder(ShareHolderList& list, const QString& share_code, const QString& holder_code, const ShareDate& date);
    bool delShareHolder(const QString& share_code, const QString& holder_code, const ShareDate& date);

    //财务信息操作
    bool updateShareFinance(const FinancialDataList& dataList);
    bool queryShareFinance(FinancialDataList& list, const QString& code);
    bool delShareFinance(const QString& code);

    //分红送配信息操作
    bool updateShareBonus(QList<ShareBonus>& bonusList);
    bool queryShareBonus(QList<ShareBonus>& list, const QString& code, const ShareDate& date);
    bool delShareBonus(const QString& code, const ShareDate& date);

    //数据表更新日期操作
    bool updateDBUpdateDate(const ShareDate& date, const QString& table);
    bool queryDBUpdateDate(ShareDate& date, const QString& table);
    bool delDBUpdateDate(const QString& table);

private:
    bool initSqlDB();
    //创建数据库需要的各类表
    //板块明细
    bool createBlockTable();
    //基本信息
    bool createShareBasicTable();
    //历史日线数据表
    bool createShareHistoryInfoTable(const QString& code);
    //股东明细表
    bool createShareHoldersTable();
    //财务信息表
    bool createShareFinancialInfoTable();
    //分红送配信息表
    bool createShareBonusIbfoTable();
    //各个表的最后更新日期
    bool createDBUpdateDateTable();

    bool isTableExist(const QString &pTable);
    bool createTable(const QString& pTable, const TableColList& cols);

    //表的通用操作
    bool updateTable(const QString& tableName, const DBColValList& values, const DBColValList& key );
    bool deleteRecord(const QString& table, const DBColValList& list = DBColValList());    
    bool isRecordExist(bool& exist, const QString& table, const DBColValList& list);

signals:

public slots:
private:
    QSqlDatabase        mDB;
    QSqlQuery           mSQLQuery;
    QMutex              mSQlMutex;
    bool                mInitDBFlg;
};

#endif // HQDBDATABASE_H
