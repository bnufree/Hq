#ifndef HQDBDATABASE_H
#define HQDBDATABASE_H

#include <QObject>
#include <QtSql>
#include <QMutex>
#include "hqdatadefines.h"

enum HISTORY_CHANGEPERCENT{
    DAYS_3 = 1,
    DAYS_5,
    DAYS_10,
    DAYS_MONTH,
    DAYS_YEARS,
};

class HQDBDataBase : public QObject
{
    Q_OBJECT
public:
    explicit HQDBDataBase(QObject *parent = 0);
    ~HQDBDataBase();
    QString getErrorString();
    QDate getLastUpdateDateOfTable(const QString &table);
    bool isDBOK();
    //板块
    bool getBlockDataList(QMap<QString, BlockData*>& pBlockMap, int type = 0);
    bool saveBlockDataList(const QMap<QString, BlockData*>& pBlockMap );
    bool addBlock(const BlockData& data);
    bool modifyBlock(const BlockData& data);
    bool deleteBlock(const QString& code);
    bool isBlockExist(const QString& code);
    //个股
    bool getShareDataList(QMap<QString, StockData*>& pShareMap);
    bool saveShareDataList(const QMap<QString, StockData*>& pShareMap );
    bool addShare(const StockData& data, const QString& table);
    bool deleteShare(const QString& table, const QString& col = QString(), const QVariant& val = QVariant());
    bool addHistoryDataList(const QString& code, const StockDataList& list);
    bool isRecordExist(bool& exist, const QString& table, const QString& col, const QVariant& val);
    bool getMultiDaysChangePercent(double& change, const QString &code, HISTORY_CHANGEPERCENT type );

    bool createDBTables();
    QDate getLastUpdateDateOfShareHistory(const QString &code);

private:
    bool initSqlDB();
    bool createBlockTable();
    bool createShareTable();
    bool createStockHistoryInfoTable(const QString& code);
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
