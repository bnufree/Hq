#include "hqdbdatabase.h"
#include <QMutexLocker>
#include <QDebug>

#define     QDebug()            qDebug()<<__FUNCTION__<<__LINE__
#define     HISTORY_TABLE(code) HQ_SHARE_HISTORY_INFO_TABLE + code.right(6)


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
    if(!mSQLQuery.exec(tr("SELECT COUNT(*) FROM sqlite_master where type='table' and name='%1'").arg(pTable))) return false;
    while (mSQLQuery.next()) {
        return mSQLQuery.value(0).toBool();
    }
    return false;
}

bool HQDBDataBase::createTable(const QString &pTable, const TableColList& cols)
{
    QStringList colist;
    foreach (TABLE_COL_DEF data, cols) {
        colist.append(tr(" [%1] %2 ").arg(data.mKey).arg(data.mDef));
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

bool HQDBDataBase::getBlockDataList(QMap<QString, BlockData*>& pBlockMap, int type)
{
    QMutexLocker locker(&mSQlMutex);
    QString filter = (type != 0 ? tr(" where %1 = %2").arg(HQ_TABLE_COL_BLOCK_TYPE).arg(type) : "");
    if(!mSQLQuery.exec(tr("select * from %1 %2").arg(HQ_BLOCK_TABLE).arg(filter))) return false;
    while (mSQLQuery.next()) {
        QString code = mSQLQuery.value(HQ_TABLE_COL_CODE).toString();
        BlockData *info = pBlockMap.value(code, 0);
        if(!info) info = new BlockData;
        info->mCode = mSQLQuery.value(HQ_TABLE_COL_CODE).toString();
        info->mName = mSQLQuery.value(HQ_TABLE_COL_NAME).toString();
        info->mChangePer = mSQLQuery.value(HQ_TABLE_COL_CHANGE_PERCENT).toDouble();
        info->mBlockType = mSQLQuery.value(HQ_TABLE_COL_BLOCK_TYPE).toInt();
        info->mIsFav = mSQLQuery.value(HQ_TABLE_COL_FAVORITE).toBool();
        info->mShareCodeList = mSQLQuery.value(HQ_TABLE_COL_SHARE_LIST).toStringList();
        info->mDate = mSQLQuery.value(HQ_TABLE_COL_DATE).toDate();
        pBlockMap[info->mCode] = info;
    }
    return true;
}

bool HQDBDataBase::saveBlockDataList(const QMap<QString, BlockData *>& pBlockMap)
{
    QSqlDatabase::database().transaction();
    foreach (QString key, pBlockMap.keys()) {
        bool sts = false;
        if(isBlockExist(key))
        {
            //更新
            sts = modifyBlock(*(pBlockMap.value(key)));
        } else
        {
            //添加
            sts = addBlock(*(pBlockMap.value(key)));
        }
        if(!sts)
        {
            QSqlDatabase::database().rollback();
            return false;
        }
    }
    QSqlDatabase::database().commit();
    return true;
}

bool HQDBDataBase::isBlockExist(const QString &code)
{
    QMutexLocker locker(&mSQlMutex);
    //先检查板块名是否存在
    if(!mSQLQuery.exec(tr("select 1 from %1 where %2 = '%3'").arg(HQ_BLOCK_TABLE).arg(HQ_TABLE_COL_CODE).arg(code))) return false;
    while (mSQLQuery.next()) {
        return true;
    }
    return false;
}

bool HQDBDataBase::addBlock(const BlockData &info)
{
    QMutexLocker locker(&mSQlMutex);
    mSQLQuery.prepare(QString("insert into %1 values ("
                      "?, ?, ?, ?, ?, ?, ?)").arg(HQ_BLOCK_TABLE));
    mSQLQuery.addBindValue(info.mCode);
    mSQLQuery.addBindValue(info.mName);
    mSQLQuery.addBindValue(info.mClose);
    mSQLQuery.addBindValue(info.mChangePer);
    mSQLQuery.addBindValue(info.mShareCodeList.join(","));
    mSQLQuery.addBindValue(info.mBlockType);
    mSQLQuery.addBindValue(info.mDate);
    return mSQLQuery.exec();
}

bool HQDBDataBase::modifyBlock(const BlockData &info)
{
    QMutexLocker locker(&mSQlMutex);
    mSQLQuery.prepare(QString("update %1 set "
                      " %2 = ?, %3 = ?, %4 = ?, %5 = ? "
                      " where %6 = ? ")
                      .arg(HQ_BLOCK_TABLE)
                      .arg(HQ_TABLE_COL_CLOSE)
                      .arg(HQ_TABLE_COL_CHANGE_PERCENT)
                      .arg(HQ_TABLE_COL_SHARE_LIST)
                      .arg(HQ_TABLE_COL_DATE)
                      .arg(HQ_TABLE_COL_CODE));
    mSQLQuery.addBindValue(info.mClose);
    mSQLQuery.addBindValue(info.mChangePer);
    mSQLQuery.addBindValue(info.mShareCodeList.join(","));
    mSQLQuery.addBindValue(info.mDate);
    mSQLQuery.addBindValue(info.mCode);

    return mSQLQuery.exec();
}

bool HQDBDataBase::deleteBlock(const QString& code)
{
    return mSQLQuery.exec(tr("delete from %1 where %2 = %1").arg(HQ_BLOCK_TABLE).arg(HQ_TABLE_COL_CODE).arg(code));
}

bool HQDBDataBase::createDBTables()
{
    if(!createBlockTable()) return false;
    if(!createShareTable()) return false;
    return true;
}

bool HQDBDataBase::createBlockTable()
{
    if(isTableExist(HQ_BLOCK_TABLE)) return true;
    TableColList colist;
    colist.append({HQ_TABLE_COL_ID, "INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL"});
    colist.append({HQ_TABLE_COL_CODE, "VARCHAR(6) NOT NULL"});
    colist.append({HQ_TABLE_COL_NAME, "VARCHAR(100) NOT NULL"});
    colist.append({HQ_TABLE_COL_CLOSE, "NUMERIC NULL"});
    colist.append({HQ_TABLE_COL_CHANGE_PERCENT, "NUMERIC NULL"});
    colist.append({HQ_TABLE_COL_SHARE_LIST, "VARCHAR(10000) NULL"});
    colist.append({HQ_TABLE_COL_BLOCK_TYPE, "INTEGER NULL"});
    colist.append({HQ_TABLE_COL_DATE, "DATE NULL"});
    return createTable(HQ_BLOCK_TABLE, colist);
}

bool HQDBDataBase::createShareTable()
{
    if(isTableExist(HQ_SHARE_TABLE)) return true;
    TableColList colist;
    colist.append({HQ_TABLE_COL_ID, "INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL"});
    colist.append({HQ_TABLE_COL_CODE, "VARCHAR(6) NOT NULL"});
    colist.append({HQ_TABLE_COL_NAME, "VARCHAR(100) NOT NULL"});
    colist.append({HQ_TABLE_COL_CLOSE, "NUMERIC NULL"});
    colist.append({HQ_TABLE_COL_CHANGE_PERCENT, "NUMERIC NULL"});
    colist.append({HQ_TABLE_COL_VOL, "NUMERIC NULL"});
    colist.append({HQ_TABLE_COL_MONEY, "NUMERIC NULL"});
    colist.append({HQ_TABLE_COL_ZJLX, "NUMERIC NULL"});
    colist.append({HQ_TABLE_COL_RZRQ, "NUMERIC NULL"});
    colist.append({HQ_TABLE_COL_FAVORITE, "BOOL NULL"});
    colist.append({HQ_TABLE_COL_HSGT_TOP10, "BOOL NULL"});
    colist.append({HQ_TABLE_COL_HSGT_TOP10_VOL, "NUMERIC NULL"});
    colist.append({HQ_TABLE_COL_HSGT_TOP10_MONEY, "NUMERIC NULL"});
    colist.append({HQ_TABLE_COL_HSGT_HAVE, "NUMERIC NULL"});
    colist.append({HQ_TABLE_COL_TOTALMNT, "NUMERIC NULL"});
    colist.append({HQ_TABLE_COL_MUTAL, "NUMERIC NULL"});
    colist.append({HQ_TABLE_COL_PROFIT, "NUMERIC NULL"});
    colist.append({HQ_TABLE_COL_BLOCK_LIST, "VARCHAR(10000) NULL"});
    colist.append({HQ_TABLE_COL_DATE, "DATE NULL"});

    return createTable(HQ_SHARE_TABLE, colist);
}
bool HQDBDataBase::addHistoryDataList(const QString &code, const StockDataList &list)
{
    if(!createStockHistoryInfoTable(code)) return false;
    QSqlDatabase::database().transaction();
    foreach (StockData data, list) {
        bool exist = false;
        if(!isRecordExist(exist, HISTORY_TABLE(code), HQ_TABLE_COL_DATE, data.mDate))
        {
            QSqlDatabase::database().rollback();
            return false;
        }
        if(exist) continue;
        if(!addShare(data, HISTORY_TABLE(code)))
        {
            QSqlDatabase::database().rollback();
            return false;
        }
    }
    QSqlDatabase::database().commit();
    return true;
}

bool HQDBDataBase::createStockHistoryInfoTable(const QString& code)
{
    if(isTableExist(HISTORY_TABLE(code))) return true;
    TableColList colist;
    colist.append({HQ_TABLE_COL_ID, "INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL"});
    colist.append({HQ_TABLE_COL_CODE, "VARCHAR(6) NOT NULL"});
    colist.append({HQ_TABLE_COL_NAME, "VARCHAR(100) NOT NULL"});
    colist.append({HQ_TABLE_COL_CLOSE, "NUMERIC NULL"});
    colist.append({HQ_TABLE_COL_CHANGE_PERCENT, "NUMERIC NULL"});
    colist.append({HQ_TABLE_COL_VOL, "NUMERIC NULL"});
    colist.append({HQ_TABLE_COL_MONEY, "NUMERIC NULL"});
    colist.append({HQ_TABLE_COL_ZJLX, "NUMERIC NULL"});
    colist.append({HQ_TABLE_COL_RZRQ, "NUMERIC NULL"});
    colist.append({HQ_TABLE_COL_FAVORITE, "BOOL NULL"});
    colist.append({HQ_TABLE_COL_HSGT_TOP10, "BOOL NULL"});
    colist.append({HQ_TABLE_COL_HSGT_TOP10_VOL, "NUMERIC NULL"});
    colist.append({HQ_TABLE_COL_HSGT_TOP10_MONEY, "NUMERIC NULL"});
    colist.append({HQ_TABLE_COL_HSGT_HAVE, "NUMERIC NULL"});
    colist.append({HQ_TABLE_COL_TOTALMNT, "NUMERIC NULL"});
    colist.append({HQ_TABLE_COL_MUTAL, "NUMERIC NULL"});
    colist.append({HQ_TABLE_COL_PROFIT, "NUMERIC NULL"});
    colist.append({HQ_TABLE_COL_BLOCK_LIST, "VARCHAR(10000) NULL"});
    colist.append({HQ_TABLE_COL_DATE, "DATE NULL"});
    return createTable(HISTORY_TABLE(code), colist);
}

QDate HQDBDataBase::getLastUpdateDateOfShareHistory(const QString &code)
{
    createStockHistoryInfoTable(code);
    return getLastUpdateDateOfTable(HISTORY_TABLE(code));
}

bool HQDBDataBase::getShareDataList(QMap<QString, StockData*>& pShareMap)
{
    QMutexLocker locker(&mSQlMutex);
    if(!mSQLQuery.exec(tr("select * from %1").arg(HQ_SHARE_TABLE))) return false;
    while (mSQLQuery.next()) {
        StockData *info = new StockData;
        info->mCode = mSQLQuery.value(HQ_TABLE_COL_CODE).toString();
        info->mName = mSQLQuery.value(HQ_TABLE_COL_NAME).toString();
        info->mClose = mSQLQuery.value(HQ_TABLE_COL_CLOSE).toDouble();
        info->mChgPercent = mSQLQuery.value(HQ_TABLE_COL_CHANGE_PERCENT).toDouble();
        info->mVol = mSQLQuery.value(HQ_TABLE_COL_VOL).toLongLong();
        info->mMoney = mSQLQuery.value(HQ_TABLE_COL_MONEY).toDouble();
        info->mZJLX = mSQLQuery.value(HQ_TABLE_COL_ZJLX).toDouble();
        info->mRZRQ = mSQLQuery.value(HQ_TABLE_COL_RZRQ).toDouble();
        info->mIsFavCode = mSQLQuery.value(HQ_TABLE_COL_FAVORITE).toBool();
        info->mHSGTTop10 = mSQLQuery.value(HQ_TABLE_COL_HSGT_TOP10).toBool();
        info->mHSGTTop10Money = mSQLQuery.value(HQ_TABLE_COL_HSGT_TOP10_MONEY).toDouble();
        info->mForeignVol = mSQLQuery.value(HQ_TABLE_COL_HSGT_HAVE).toLongLong();
        info->mTotalShare = mSQLQuery.value(HQ_TABLE_COL_TOTALMNT).toLongLong();
        info->mMutableShare = mSQLQuery.value(HQ_TABLE_COL_MUTAL).toLongLong();
        info->mProfit = mSQLQuery.value(HQ_TABLE_COL_PROFIT).toDouble();
        info->mBlockCodeList = mSQLQuery.value(HQ_TABLE_COL_BLOCK_LIST).toStringList();
        info->mDate = mSQLQuery.value(HQ_TABLE_COL_DATE).toDate();
        pShareMap[info->mCode.right(6)] = info;
    }
    return true;
}

bool HQDBDataBase::updateShareData(StockData *data)
{
    if(!data) return false;
    QSqlDatabase::database().transaction();
    //先删除对应的code
    if(!deleteShare(HQ_SHARE_TABLE, HQ_TABLE_COL_CODE, data->mCode));
    {
        QSqlDatabase::database().rollback();
        return false;
    }
    if(!addShare(*data, HQ_SHARE_TABLE))
    {
        QSqlDatabase::database().rollback();
        return false;
    }
    QSqlDatabase::database().commit();
    return true;
}

bool HQDBDataBase::saveShareDataList(const QMap<QString, StockData *>& pShareMap)
{
    QSqlDatabase::database().transaction();
    //先删除基本表
    if(!deleteShare(HQ_SHARE_TABLE))
    {
        QSqlDatabase::database().rollback();
        return false;
    }
    foreach (QString key, pShareMap.keys()) {
        if(!addShare(*(pShareMap.value(key)), HQ_SHARE_TABLE))
        {
            QSqlDatabase::database().rollback();
            return false;
        }
    }
    QSqlDatabase::database().commit();
    return true;
}

bool HQDBDataBase::addShare(const StockData &info, const QString& table)
{
    QMutexLocker locker(&mSQlMutex);
    mSQLQuery.prepare(QString("insert into %1 ("
                              "code, name, close, change, vol,"
                              "money, zjlx, rzrq, favorite, hsgt,"
                              "vol_foreign, foreign_money, foreign_amount,total_amount,mutable_amount,"
                              "profit, block_list, date)"
                              " values ("
                              "?, ?, ?, ?, ?, "
                              "?, ?, ?, ?, ?,"
                              "?, ?, ?, ?, ?,"
                              "?, ?, ?)").arg(table));

    mSQLQuery.addBindValue(info.mCode);
    mSQLQuery.addBindValue(info.mName);
    mSQLQuery.addBindValue(info.mClose);
    mSQLQuery.addBindValue(info.mChgPercent);
    mSQLQuery.addBindValue(info.mVol);
    mSQLQuery.addBindValue(info.mMoney);
    mSQLQuery.addBindValue(info.mZJLX);
    mSQLQuery.addBindValue(info.mRZRQ);
    mSQLQuery.addBindValue(info.mIsFavCode);
    mSQLQuery.addBindValue(info.mHSGTTop10);
    mSQLQuery.addBindValue(info.mHSGTTop10Vol);
    mSQLQuery.addBindValue(info.mHSGTTop10Money);
    mSQLQuery.addBindValue(info.mForeignVol);
    mSQLQuery.addBindValue(info.mTotalShare);
    mSQLQuery.addBindValue(info.mMutableShare);
    mSQLQuery.addBindValue(info.mProfit);
    mSQLQuery.addBindValue(info.mBlockCodeList);
    mSQLQuery.addBindValue(info.mDate);
    return mSQLQuery.exec();
}



bool HQDBDataBase::deleteShare(const QString& table, const QString& col, const QVariant& val)
{
    QMutexLocker locker(&mSQlMutex);
    if(col.length() == 0)
    {
        return mSQLQuery.exec(QString("delete from %1").arg(table));
    }
    mSQLQuery.prepare(QString("delete from %1 where %2 = ?").arg(table).arg(col));
    mSQLQuery.addBindValue(val);
    return mSQLQuery.exec();
}

bool HQDBDataBase::isRecordExist(bool& exist,const QString &table, const QString &col, const QVariant &val)
{
    exist = false;
    QMutexLocker locker(&mSQlMutex);
    mSQLQuery.prepare(QString("select count(1) from %1 where %2 = ?").arg(table).arg(col));
    mSQLQuery.addBindValue(val);
    if(!mSQLQuery.exec()) return false;
    while (mSQLQuery.next()) {
        exist = mSQLQuery.value(0).toBool();
        break;
    }
    return true;
}

double HQDBDataBase::getMultiDaysChangePercent(const QString &code, HISTORY_CHANGEPERCENT type)
{
    double change = 0.0;
    QMutexLocker locker(&mSQlMutex);
    QString table = HISTORY_TABLE(code);
    QString col = HQ_TABLE_COL_CHANGE_PERCENT;
    if(!mSQLQuery.exec(tr("select 1+%1 * 0.01 from %2 order by date desc limit %3").arg(col).arg(table).arg(type)))
    {
        qDebug()<<errMsg();
        return change;
    }
    change = 1.0;
    while (mSQLQuery.next()) {
        change *= mSQLQuery.value(0).toDouble();
    }
    change = (change -1) * 100;
    return change;
}

double HQDBDataBase::getLastMoney(const QString &code)
{
    double change = 0.0;
    QMutexLocker locker(&mSQlMutex);
    QString table = HISTORY_TABLE(code);
    QString col = HQ_TABLE_COL_MONEY;
    if(!mSQLQuery.exec(tr("select %1 from %2 order by date desc limit 1").arg(col).arg(table)))
    {
        qDebug()<<errMsg();
        return change;
    }
    change = 1.0;
    while (mSQLQuery.next()) {
        change = mSQLQuery.value(0).toDouble() /10000.0;
        break;
    }
    return change;
}

bool HQDBDataBase::getLastForeignVol(qint64 &vol, qint64 &vol_chg, const QString &code)
{
    QMutexLocker locker(&mSQlMutex);
    QString table = HISTORY_TABLE(code);
    QString col = HQ_TABLE_COL_HSGT_HAVE;
    if(!mSQLQuery.exec(tr("select %1 from %2 where %3!=0 order by date desc limit 2").arg(col).arg(table).arg(col)))
    {
        qDebug()<<errMsg();
        return false;
    }
    QList<qint64> list;
    while (mSQLQuery.next()) {
        list.append(mSQLQuery.value(0).toLongLong());
    }
    if(list.length() == 0) return false;
    vol = list[0];
    vol_chg = 0;
    if(list.length() > 1)
    {
        vol_chg = list[0] - list[1];
    }

    return true;
}

bool HQDBDataBase::getHistoryDataOfCode(StockDataList& list, const QString &code)
{
    list.clear();
    QMutexLocker locker(&mSQlMutex);
    QString table = HISTORY_TABLE(code);
    if(!mSQLQuery.exec(tr("select * from %1 order by date desc limit 100").arg(table)))
    {
        qDebug()<<errMsg();
        return false;
    }
    while (mSQLQuery.next()) {
        StockData data;
        data.mCode = mSQLQuery.value(HQ_TABLE_COL_CODE).toString();
        data.mName = mSQLQuery.value(HQ_TABLE_COL_NAME).toString();
        data.mClose = mSQLQuery.value(HQ_TABLE_COL_CLOSE).toDouble();
        data.mChgPercent = mSQLQuery.value(HQ_TABLE_COL_CHANGE_PERCENT).toDouble();
        data.mForeignVol = mSQLQuery.value(HQ_TABLE_COL_HSGT_HAVE).toLongLong();
        data.mForeignCap = data.mClose * data.mForeignVol;
        data.mDate = mSQLQuery.value(HQ_TABLE_COL_DATE).toDate();
        list.append(data);
    }
    if(list.length() == 0) return false;
    return true;
}

QString HQDBDataBase::errMsg()
{
    return QString("sql:%1\nerr:%2").arg(mSQLQuery.lastQuery()).arg(mSQLQuery.lastError().text());
}



