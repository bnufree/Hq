#ifndef HQDBDATABASE_H
#define HQDBDATABASE_H

#include <QObject>
#include <QtSql>
#include <QMutex>
#include "hqdatadefines.h"

class HQDBDataBase : public QObject
{
    Q_OBJECT
public:
    explicit HQDBDataBase(QObject *parent = 0);
    ~HQDBDataBase();
    QString getErrorString();
    bool isTableExist(const QString &pTable);
    bool createTable(const QString& pTable, const QMap<QString, QString>& cols);
    QDate getLastUpdateDateOfTable(const QString &table);
    bool isDBOK();
    bool getBlockDataList(BlockDataList& list, int type = 0);    
    bool createDBTables();
    QDate getLastUpdateDateOfShareHistory(const QString &code);

private:
    bool initSqlDB();
    bool createBlockTable();
    bool createStockBaseInfoTable(const QString& code);

signals:

public slots:
private:
    QSqlDatabase        mDB;
    QSqlQuery           mSQLQuery;
    QMutex              mSQlMutex;
    bool                mInitDBFlg;
};

#endif // HQDBDATABASE_H
