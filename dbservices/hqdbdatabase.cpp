#include "hqdbdatabase.h"
#include <QMutexLocker>
#include <QDebug>

#define     QDebug()            qDebug()<<__FUNCTION__<<__LINE__
#define     HISTORY_TABLE(code) HQ_SHARE_BASIC_INFO_TABLE + code


HQDBDataBase::HQDBDataBase(QObject *parent) : QObject(parent)
{
    mInitDBFlg = initSqlDB();
    if(mInitDBFlg)
    {
        mSQLQuery = QSqlQuery(mDB);
    }
}

HQDBDataBase::~HQDBDataBase()
{
    if(mInitDBFlg)
    {
        mDB.close();
    }
}

bool HQDBDataBase::isDBOK()
{
    return mInitDBFlg;
}

bool HQDBDataBase::initSqlDB()
{
    //初始化本地数据库的连接
    qDebug()<<"database:"<<QSqlDatabase::database().databaseName();
    if(QSqlDatabase::contains("qt_sql_default_connection"))
    {
        mDB = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        mDB = QSqlDatabase::addDatabase("QSQLITE");
    }
    mDB.setDatabaseName("StockData.s3db");
    return mDB.open();
}

QString HQDBDataBase::getErrorString()
{
    QMutexLocker locker(&mSQlMutex);
    return QString("Error Content: \n %1 \n %2").arg(mSQLQuery.lastQuery()).arg(mSQLQuery.lastError().text());
}

bool HQDBDataBase::isTableExist(const QString &pTable)
{
    QMutexLocker locker(&mSQlMutex);
    if(!mSQLQuery.exec(tr("SELECT COUNT(*) FROM sqlite_master where type='table' and name='%1'").arg(pTable)))
    while (mSQLQuery.next()) {
        return mSQLQuery.value(0).toBool();
    }
    return false;
}

bool HQDBDataBase::createTable(const QString &pTable, const QMap<QString, QString> &cols)
{
    QStringList colist;
    foreach (QString key, cols.keys()) {
        colist.append(tr(" [%1] %2 ").arg(key).arg(cols[key]));
    }

    QString sql = tr("CREATE TABLE [%1] ( %2 )").arg(pTable).arg(colist.join(","));
    QMutexLocker locker(&mSQlMutex);
    return mSQLQuery.exec(sql);
}

QDate HQDBDataBase::getLastUpdateDateOfTable(const QString &table)
{
    QMutexLocker locker(&mSQlMutex);
    QDate date = QDate(2016, 12, 4);
    if(mSQLQuery.exec(tr("select max(date) from %1").arg(table)))
    {
        while (mSQLQuery.next()) {
            if(!mSQLQuery.value(0).isNull())
            {
                date = mSQLQuery.value(0).toDate();
                break;
            }
        }
    }
    return date;
}

bool HQDBDataBase::getBlockDataList(BlockDataList &list, int type)
{
    QMutexLocker locker(&mSQlMutex);
    list.clear();
    QString filter = (type != 0 ? tr(" where %1 = %2").arg(HQ_TABLE_COL_BLOCK_TYPE).arg(type) : "");
    if(!mSQLQuery.exec(tr("select * from %1 %2").arg(HQ_BLOCK_TABLE).arg(filter))) return false;
    while (mSQLQuery.next()) {
        BlockData *info = new BlockData;
        info->mCode = mSQLQuery.value(HQ_TABLE_COL_CODE).toString();
        info->mName = mSQLQuery.value(HQ_TABLE_COL_NAME).toString();
        info->mChangePer = mSQLQuery.value(HQ_TABLE_COL_CHANGE_PERCENT).toDouble();
        info->mBlockType = mSQLQuery.value(HQ_TABLE_COL_BLOCK_TYPE).toInt();
        info->mIsFav = mSQLQuery.value(HQ_TABLE_COL_FAVORITE).toBool();
        info->mShareCodeList = mSQLQuery.value(HQ_TABLE_COL_SHARE_LIST).toStringList();
        info->mDate = QDateTime::fromMSecsSinceEpoch(mSQLQuery.value(HQ_TABLE_COL_DATE).toLongLong()).date();
        list.append(info);
    }
    return true;
}

bool HQDBDataBase::createDBTables()
{
    if(!createBlockTable()) return false;
    return true;
}

bool HQDBDataBase::createBlockTable()
{
    if(isTableExist(HQ_BLOCK_TABLE)) return true;
    QMap<QString, QString> colist;
    colist.insert(HQ_TABLE_COL_ID, "INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL");
    colist.insert(HQ_TABLE_COL_CODE, "VARCHAR(6) NOT NULL");
    colist.insert(HQ_TABLE_COL_NAME, "VARCHAR(100) NOT NULL");
    colist.insert(HQ_TABLE_COL_CLOSE, "NUMERIC NULL");
    colist.insert(HQ_TABLE_COL_CHANGE_PERCENT, "NUMERIC NULL");
    colist.insert(HQ_TABLE_COL_SHARE_LIST, "VARCHAR(10000) NULL");
    colist.insert(HQ_TABLE_COL_DATE, "DATE NULL");
    colist.insert(HQ_TABLE_COL_BLOCK_TYPE, "INTEGER NULL");
    return createTable(HQ_BLOCK_TABLE, colist);
}

bool HQDBDataBase::createStockBaseInfoTable(const QString& code)
{
    if(isTableExist(HISTORY_TABLE(code))) return true;
    QMap<QString, QString> colist;
    colist.insert(HQ_TABLE_COL_ID, "INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL");
    colist.insert(HQ_TABLE_COL_CODE, "VARCHAR(6) NOT NULL");
    colist.insert(HQ_TABLE_COL_NAME, "VARCHAR(100) NOT NULL");
    colist.insert(HQ_TABLE_COL_CLOSE, "NUMERIC NULL");
    colist.insert(HQ_TABLE_COL_CHANGE_PERCENT, "NUMERIC NULL");
    colist.insert(HQ_TABLE_COL_VOL, "NUMERIC NULL");
    colist.insert(HQ_TABLE_COL_MONEY, "NUMERIC NULL");
    colist.insert(HQ_TABLE_COL_ZJLX, "NUMERIC NULL");
    colist.insert(HQ_TABLE_COL_RZRQ, "NUMERIC NULL");
    colist.insert(HQ_TABLE_COL_FAVORITE, "BOOL NULL");
    colist.insert(HQ_TABLE_COL_HSGT_TOP10, "NUMERIC NULL");
    colist.insert(HQ_TABLE_COL_FOREIGN_VOL, "NUMERIC NULL");
    colist.insert(HQ_TABLE_COL_FOREIGN_MONEY, "NUMERIC NULL");
    colist.insert(HQ_TABLE_COL_FOREIGN_HAVE, "NUMERIC NULL");
    colist.insert(HQ_TABLE_COL_TOTALMNT, "NUMERIC NULL");
    colist.insert(HQ_TABLE_COL_MUTAL, "NUMERIC NULL");
    colist.insert(HQ_TABLE_COL_DATE, "DATE NULL");
    colist.insert(HQ_TABLE_COL_PROFIT, "NUMERIC NULL");

    return createTable(HISTORY_TABLE(code), colist);
}

QDate HQDBDataBase::getLastUpdateDateOfShareHistory(const QString &code)
{
    createStockBaseInfoTable(code);
    return getLastUpdateDateOfTable(HISTORY_TABLE(code));
}


