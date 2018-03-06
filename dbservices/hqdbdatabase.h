#ifndef HQDBDATABASE_H
#define HQDBDATABASE_H

#include <QObject>
#include <QtSql>
#include <QMutex>
#include "hqdatadefines.h"

enum HISTORY_CHANGEPERCENT{
    DAYS_3 = 3,
    DAYS_5 = 5,
    DAYS_10 = 10,
    DAYS_MONTH = 20,
    DAYS_HALF_YEAR = 120,
    DAYS_YEARS = 240,
};

struct HQ_QUERY_CONDITION{
    QString  col;
    QVariant val;

    HQ_QUERY_CONDITION(const QString colStr, const QVariant& val1)
    {
        col = colStr;
        val = val1;
    }
};

class HQDBDataBase : public QObject
{
    Q_OBJECT
public:
    explicit HQDBDataBase(QObject *parent = 0);
    ~HQDBDataBase();
    QString getErrorString();
    QDate getLastUpdateDateOfTable(const QString &table);
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
    bool getBasicShareDataList(QMap<QString, ShareData*>& pShareMap);
    bool updateBasicShareDataList(QList<ShareData*> dataList);
    bool updateBasicShare(const ShareData& data, bool exist);
    bool updateHistoryShare(const ShareData& data, bool exist);
    bool deleteShare(const QString& table, const QString& col = QString(), const QVariant& val = QVariant());
    bool updateHistoryDataList(const ShareDataList& list);
    bool isRecordExist(bool& exist, const QString& table, const QList<HQ_QUERY_CONDITION>& list);
    bool deleteRecord(const QString& table, const QList<HQ_QUERY_CONDITION>& list);
    double getMultiDaysChangePercent(const QString &code, HISTORY_CHANGEPERCENT type );
    double getLastMoney(const QString& code);
    bool   getLastForeignVol(qint64& vol, qint64& vol_chg, const QString& code);

    bool createDBTables();
    QString errMsg();
    bool getHistoryDataOfCode(ShareDataList& list, const QString &code);

private:
    bool initSqlDB();
    bool createBlockTable();
    bool createShareBasicTable();
    bool createGeneralTable();
    bool createShareHistoryInfoTable();
    bool isTableExist(const QString &pTable);
    bool createTable(const QString& pTable, const TableColList& cols);

signals:

public slots:
private:
    QSqlDatabase        mDB;
    QSqlQuery           mSQLQuery;
    QMutex              mSQlMutex;
    bool                mInitDBFlg;
};

#endif // HQDBDATABASE_H
