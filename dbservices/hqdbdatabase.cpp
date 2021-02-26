#include "hqdbdatabase.h"
#include <QMutexLocker>
#include <QDebug>
#include <QFile>
#include <QApplication>
#include "utils/comdatadefines.h"

#define     QDebug()                    qDebug()<<__FUNCTION__<<__LINE__
#define     HISTORY_TABLE(code)         QString(TABLE_SHARE_HISTORY_TEMPLATE).arg(code.right(6))

#define     DB_FILE                     QString("%1/db.data").arg(HQ_DATA_DIR)
#define     SQL_WHERE                   " where "

QString DBColValList::insertString() const
{
    if(size() == 0) return QString();
    QStringList colList;
    QVariantList valList;
    for(int i=0; i<size(); i++)
    {
        HQ_QUERY_CONDITION data = at(i);
        colList.append(data.mColName);
        valList.append(QVariant::fromValue(data.mColVal));
    }
    QStringList valStrlist;
    for(int i=0; i<valList.size(); i++)
    {
        HQ_COL_VAL val = valList[i].value<HQ_COL_VAL>();
        if(val.mType == HQ_DATA_TEXT)
        {
            valStrlist.append(QString("'%1'").arg(val.mValue.toString()));
        } else if(val.mType == HQ_DATA_INT)
        {
            valStrlist.append(QString("%1").arg(val.mValue.toInt()));
        } else if(val.mType == HQ_DATA_LONG)
        {
            valStrlist.append(QString("%1").arg(val.mValue.toLongLong()));
        }
        else
        {
            valStrlist.append(QString("%1").arg(val.mValue.toDouble(), 0, 'f', 3));
        }
    }
    return QString("(%1) values (%2)").arg(colList.join(",")).arg(valStrlist.join(","));
}

QString DBColValList::updateString() const
{
    if(size() == 0) return QString();
    QStringList valStrlist;
    for(int i=0; i<size(); i++)
    {
        HQ_QUERY_CONDITION data = at(i);
        if(data.mColVal.mType == HQ_DATA_TEXT)
        {
            valStrlist.append(QString("%1 = '%2'").arg(data.mColName).arg(data.mColVal.mValue.toString()));
        } else if(data.mColVal.mType == HQ_DATA_INT)
        {
            valStrlist.append(QString("%1 = %2").arg(data.mColName).arg(data.mColVal.mValue.toInt()));
        } else if(data.mColVal.mType == HQ_DATA_LONG)
        {
            valStrlist.append(QString("%1 = %2").arg(data.mColName).arg(data.mColVal.mValue.toLongLong()));
        }
        else
        {
            valStrlist.append(QString("%1 = %2").arg(data.mColName).arg(data.mColVal.mValue.toDouble(), 0, 'f', 3));
        }
    }
    return valStrlist.join(",");
}

QString DBColValList::whereString() const
{
    if(size() == 0) return QString();
    QStringList valStrlist;
    for(int i=0; i<size(); i++)
    {
        HQ_QUERY_CONDITION data = at(i);
        if(data.mColVal.mType == HQ_DATA_TEXT)
        {
            if(data.mColCompare == HQ_COMPARE_EQUAL)
            {
                valStrlist.append(QString("%1 = '%2'").arg(data.mColName).arg(data.mColVal.mValue.toString()));
            } else if(data.mColCompare == HQ_COMPARE_STRLIKE)
            {
                valStrlist.append(QString("%1 like '%2'").arg(data.mColName).arg(data.mColVal.mValue.toString()));
            } else if(data.mColCompare == HQ_COMPARE_TEXTIN)
            {
                QStringList list = data.mColVal.mValue.toStringList();
                if(list.size() > 0)
                {
                    QStringList resList;
                    foreach (QString str, list) {
                        resList.append(QString("'%1'").arg(str));
                    }
                    valStrlist.append(QString("%1 in () ").arg(data.mColName).arg(resList.join(",")));
                }
            } else if(data.mColCompare == HQ_COMPARE_GREAT)
            {
                valStrlist.append(QString("%1 > '%2'").arg(data.mColName).arg(data.mColVal.mValue.toString()));
            } else if(data.mColCompare == HQ_COMPARE_LESS)
            {
                valStrlist.append(QString("%1 < '%2'").arg(data.mColName).arg(data.mColVal.mValue.toString()));
            } else if(data.mColCompare == HQ_COMPARE_LESS_EQUAL)
            {
                valStrlist.append(QString("%1 <= '%2'").arg(data.mColName).arg(data.mColVal.mValue.toString()));
            } else if(data.mColCompare == HQ_COMPARE_GREAT_EQUAL)
            {
                valStrlist.append(QString("%1 >= '%2'").arg(data.mColName).arg(data.mColVal.mValue.toString()));
            }
        } else if(data.mColVal.mType == HQ_DATA_INT)
        {
            if(data.mColCompare == HQ_COMPARE_EQUAL)
            {
                valStrlist.append(QString("%1 = %2").arg(data.mColName).arg(data.mColVal.mValue.toInt()));
            } else if(data.mColCompare == HQ_COMPARE_GREAT)
            {
                valStrlist.append(QString("%1 > %2").arg(data.mColName).arg(data.mColVal.mValue.toInt()));
            } else if(data.mColCompare == HQ_COMPARE_LESS)
            {
                valStrlist.append(QString("%1 < %2").arg(data.mColName).arg(data.mColVal.mValue.toInt()));
            } else if(data.mColCompare == HQ_COMPARE_GREAT_EQUAL)
            {
                valStrlist.append(QString("%1 >= %2").arg(data.mColName).arg(data.mColVal.mValue.toInt()));
            } else if(data.mColCompare == HQ_COMPARE_LESS_EQUAL)
            {
                valStrlist.append(QString("%1 <= %2").arg(data.mColName).arg(data.mColVal.mValue.toInt()));
            }
        } else if(data.mColVal.mType == HQ_DATA_LONG)
        {
            if(data.mColCompare == HQ_COMPARE_EQUAL)
            {
                valStrlist.append(QString("%1 = %2").arg(data.mColName).arg(data.mColVal.mValue.toLongLong()));
            } else if(data.mColCompare == HQ_COMPARE_GREAT)
            {
                valStrlist.append(QString("%1 > %2").arg(data.mColName).arg(data.mColVal.mValue.toLongLong()));
            } else if(data.mColCompare == HQ_COMPARE_LESS)
            {
                valStrlist.append(QString("%1 < %2").arg(data.mColName).arg(data.mColVal.mValue.toLongLong()));
            } else if(data.mColCompare == HQ_COMPARE_GREAT_EQUAL)
            {
                valStrlist.append(QString("%1 >= %2").arg(data.mColName).arg(data.mColVal.mValue.toLongLong()));
            } else if(data.mColCompare == HQ_COMPARE_LESS_EQUAL)
            {
                valStrlist.append(QString("%1 <= %2").arg(data.mColName).arg(data.mColVal.mValue.toLongLong()));
            }
        }
        else
        {
            if(data.mColCompare == HQ_COMPARE_EQUAL)
            {
                valStrlist.append(QString("%1 = %2").arg(data.mColName).arg(data.mColVal.mValue.toDouble(), 0, 'f', 3));
            } else if(data.mColCompare == HQ_COMPARE_GREAT)
            {
                valStrlist.append(QString("%1 > %2").arg(data.mColName).arg(data.mColVal.mValue.toDouble(), 0, 'f', 3));
            } else if(data.mColCompare == HQ_COMPARE_LESS)
            {
                valStrlist.append(QString("%1 < %2").arg(data.mColName).arg(data.mColVal.mValue.toDouble(), 0, 'f', 3));
            } else if(data.mColCompare == HQ_COMPARE_GREAT_EQUAL)
            {
                valStrlist.append(QString("%1 >= %2").arg(data.mColName).arg(data.mColVal.mValue.toDouble(), 0, 'f', 3));
            } else if(data.mColCompare == HQ_COMPARE_LESS_EQUAL)
            {
                valStrlist.append(QString("%1 <= %2").arg(data.mColName).arg(data.mColVal.mValue.toDouble(), 0, 'f', 3));
            }
        }
    }
    return  QString(" where %1").arg(valStrlist.join(" and "));
}

HQDBDataBase::HQDBDataBase(QObject *parent) : QObject(parent)
{
    mInitDBFlg = initSqlDB();
    if(mInitDBFlg)
    {
        mSQLQuery = QSqlQuery(mDB);
    }
    qDebug()<<"db init:"<<mInitDBFlg<<QFile::exists(mDB.databaseName());
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

    qDebug()<<"db driver:"<<mDB.driverName()<<mDB.driver();
    QDir dir(HQ_DATA_DIR);
    if(!dir.exists())
    {
        dir.mkpath(HQ_DATA_DIR);

    }
    qDebug()<<"data dir exist:"<<dir.exists();
    mDB.setDatabaseName(DB_FILE);
    return mDB.open();
}

QString HQDBDataBase::getErrorString()
{
    QMutexLocker locker(&mSQLMutex);
    return QString("sql: \\n %1 \\n error:%2").arg(mSQLQuery.lastQuery()).arg(mSQLQuery.lastError().text());
}

bool HQDBDataBase::tableExist(const QString &pTable)
{
    QMutexLocker locker(&mSQLMutex);
    if(!mSQLQuery.exec(tr("SELECT COUNT(*) FROM sqlite_master where type='table' and name='%1'").arg(pTable))) return false;
    while (mSQLQuery.next()) {
        return mSQLQuery.value(0).toBool();
    }
    return false;
}

bool HQDBDataBase::deleteTable(const QString &pTable)
{
    QString sql = tr("DROP TABLE %1 ").arg(pTable);
    qDebug()<<"sql:"<<sql;
    QMutexLocker locker(&mSQLMutex);
    return mSQLQuery.exec(sql);
}

bool HQDBDataBase::createTable(const QString &pTable, const TableColList& cols)
{
    bool exist = tableExist(pTable);
    if(exist)
    {
        bool delete_table = false;
        //检查二者的列定义是否一样
        TableColList old_cols = queryTableDef(pTable);
        if(cols.size() == old_cols.size())
        {
            foreach(TABLE_COL_DEF def, cols)
            {
                int index = old_cols.indexOf(def);
                if(index == -1)
                {
                    delete_table = true;
                    break;
                }
            }
        } else
        {
            delete_table = true;
        }
        if(!delete_table) return true;
        delDBUpdateDate(pTable);
        if(!deleteTable(pTable)) return false;
    }
    QStringList colist;
    foreach (TABLE_COL_DEF data, cols) {
        colist.append(tr(" [%1] %2 ").arg(data.mName).arg(data.toSTring()));
    }

    QString sql = tr("CREATE TABLE [%1] ( %2 )").arg(pTable).arg(colist.join(","));
    qDebug()<<"sql:"<<sql;
    QMutexLocker locker(&mSQLMutex);
    return mSQLQuery.exec(sql);
}

bool HQDBDataBase::createShareBlockTable()
{
    TableColList colist;
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_ID, "INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_CODE, "VARCHAR(10) NOT NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_BLOCK_SHARE_LIST, "VARCHAR(10000) NULL"));
    return createTable(TABLE_BLOCK_SHARE, colist);
}

bool HQDBDataBase::updateShareBlockInfo(const BlockDataList &dataList)
{
    int size = dataList.size();
    if(size == 0) return false;
    mDB.transaction();
    for(int i=0; i<size; i++)
    {
        BlockData data = dataList[i];
        DBColValList list;
        list.append(DBColVal(HQ_TABLE_COL_CODE, data.mCode,HQ_DATA_TEXT));
        list.append(DBColVal(HQ_TABLE_COL_BLOCK_SHARE_LIST, data.mShareCodeList.join(","),HQ_DATA_TEXT));
        if(!updateTable(TABLE_BLOCK_SHARE, list, list[0])){
            mDB.rollback();
            return false;
        }
    }
    if(!updateDBUpdateDate(QDate::currentDate(), TABLE_BLOCK_SHARE))
    {
        mDB.rollback();
        return false;
    }
    mDB.commit();
    return true;
}

bool HQDBDataBase::queryShareBlockInfo(ShareDataMap& share, BlockDataMap& block)
{
    QMutexLocker locker(&mSQLMutex);
    if(!mSQLQuery.exec(tr("select * from %1").arg(TABLE_BLOCK_SHARE))) return false;
    while (mSQLQuery.next()) {
        QString block_code = mSQLQuery.value(HQ_TABLE_COL_CODE).toString();
        BlockData& info = block[block_code];
        if(info.mCode.length() == 0) info.mCode = block_code;
        QStringList share_codes = mSQLQuery.value(HQ_TABLE_COL_BLOCK_SHARE_LIST).toString().split(",", QString::SkipEmptyParts);
        if(share_codes.length() > 0)
        {
            info.mShareCodeList = share_codes;
            foreach (QString code, share_codes) {
                ShareData &data = share[code];
                if(data.mCode.length() == 0) data.mCode = code;
                if(!data.mReferCodeList.contains(block_code))
                {
                    data.mReferCodeList.append(block_code);
                }
            }
        }
    }
    return true;
}

bool HQDBDataBase::delShareBlockInfo(const QString &code)
{
    DBColValList list;
    if(code.length() > 0)
    {
        list.append(DBColVal(HQ_TABLE_COL_CODE, code,HQ_DATA_TEXT));
    }
    return deleteRecord(TABLE_BLOCK_SHARE, list);
}

TableColList HQDBDataBase::queryTableDef(const QString& pTable)
{
    TableColList list;
    QString sql = tr("PRAGMA table_info([%1])").arg(pTable);
    QMutexLocker locker(&mSQLMutex);
    if(!mSQLQuery.exec(sql)) return list;
    while (mSQLQuery.next()) {
        TABLE_COL_DEF col;
        col.mName = mSQLQuery.value("name").toString();
        QString def = mSQLQuery.value("type").toString();
        col.mStrSize = 0;
        if(def.contains("INTEGER")) col.mType = DB_INTEGER;
        if(def.contains("BOOL")) col.mType = DB_BOOL;
        if(def.contains("NUMERIC")) col.mType = DB_NUMERIC;
        if(def.contains("VARCHAR"))
        {
            col.mType = DB_VARCHAR;
            int index = def.indexOf("(");
            if(index >= 0)
            {
                int end = def.indexOf(")", index);
                if(end >= 0)
                {
                    col.mStrSize = def.mid(index+ 1, end - index - 1).toInt();
                }
            }
        }
        col.mPrimaryKey = (mSQLQuery.value("pk").toString().toInt() == 1) ? true : false;
        col.mNull = mSQLQuery.value("notnull").toString().toInt() == 1 ? false : true;

        list.append(col);

    }

    return list;

}

bool HQDBDataBase::createBlockTable()
{
    TableColList colist;
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_ID, "INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_CODE, "VARCHAR(10) NOT NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_NAME, "VARCHAR(100) NOT NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_CLOSE, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_FAVORITE, "BOOL NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_CHANGE_PERCENT, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_BLOCK_TYPE, "INTEGER NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_DATE, "VARCHAR(10) NOT NULL"));
    return createTable(TABLE_BLOCK, colist);
}

bool HQDBDataBase::queryBlockDataList(BlockDataMap& pBlockMap, int type)
{
    QMutexLocker locker(&mSQLMutex);
    DBColValList wherelist;
    if(type > 0)
    {
        wherelist.append(DBColVal(HQ_TABLE_COL_BLOCK_TYPE, type, HQ_DATA_INT));
    }
    if(!mSQLQuery.exec(tr("select * from %1 %2").arg(TABLE_BLOCK).arg(wherelist.whereString()))) return false;
    while (mSQLQuery.next()) {
        QString code = mSQLQuery.value(HQ_TABLE_COL_CODE).toString();
        BlockData& info = pBlockMap[code];
        if(info.mCode.length() == 0) info.mCode = code;
        info.mName = mSQLQuery.value(HQ_TABLE_COL_NAME).toString();
        info.mChangePercent = mSQLQuery.value(HQ_TABLE_COL_CHANGE_PERCENT).toDouble();
        info.mIsFav = mSQLQuery.value(HQ_TABLE_COL_FAVORITE).toBool();
        info.mBlockType = mSQLQuery.value(HQ_TABLE_COL_BLOCK_TYPE).toInt();
        info.mTime = BlockDateTime(mSQLQuery.value(HQ_TABLE_COL_DATE).toInt());
    }
    return true;
}

bool HQDBDataBase::updateBlockDataList(const BlockDataList& list)
{
    int size = list.size();
    if(size == 0) return false;
    mDB.transaction();
    for(int i=0; i<size; i++)
    {
        BlockData data = list[i];
        DBColValList list;
        list.append(DBColVal(HQ_TABLE_COL_CODE, data.mCode,HQ_DATA_TEXT));
        list.append(DBColVal(HQ_TABLE_COL_NAME, data.mName,HQ_DATA_TEXT));
        list.append(DBColVal(HQ_TABLE_COL_CHANGE_PERCENT, data.mChangePercent,HQ_DATA_DOUBLE));
        list.append(DBColVal(HQ_TABLE_COL_BLOCK_TYPE, data.mBlockType,HQ_DATA_INT));
        list.append(DBColVal(HQ_TABLE_COL_FAVORITE, data.mIsFav,HQ_DATA_INT));
        list.append(DBColVal(HQ_TABLE_COL_CHANGE_PERCENT, data.mChangePercent,HQ_DATA_DOUBLE));
        list.append(DBColVal(HQ_TABLE_COL_DATE, data.mTime,HQ_DATA_INT));
        if(!updateTable(TABLE_BLOCK, list, list[0])){
            mDB.rollback();
            return false;
        }
    }
    if(!updateDBUpdateDate(QDate::currentDate(), TABLE_BLOCK))
    {
        mDB.rollback();
    }
    mDB.commit();
    return true;
}

bool HQDBDataBase::isBlockExist(const QString &code)
{
    DBColValList list;
    list.append(DBColVal(HQ_TABLE_COL_CODE, code,HQ_DATA_TEXT));
    bool exist = false;
    if(!isRecordExist(exist, TABLE_BLOCK, list)) return false;
    return exist;
}


bool HQDBDataBase::deleteBlock(const QString& code)
{
    DBColValList list;
    if(code.length() > 0)
    {
        list.append(DBColVal(HQ_TABLE_COL_CODE, code,HQ_DATA_TEXT));
    }
    return deleteRecord(TABLE_BLOCK, list);
}

bool HQDBDataBase::createDBTables()
{
    qDebug()<<__FUNCTION__<<__LINE__;
    if(!createBlockTable()) return false;
    if(!createShareBasicTable()) return false;
    if(!createDBUpdateDateTable()) return false;
    if(!createShareFavoriteTable()) return false;
    if(!createShareProfitTable()) return false;
    if(!createShareBlockTable()) return false;
    if(!createShareHoldersTable()) return false;
    if(!createShareFinancialInfoTable()) return false;
    if(!createShareBonusIbfoTable()) return false;
    if(!createShareHsgTop10Table()) return false;
    if(!createShareHistoryCounterTable()) return false;
    if(!createShareExchangeRecordTable()) return false;
    qDebug()<<__FUNCTION__<<__LINE__;
    return true;
}
bool HQDBDataBase::createShareFavoriteTable()
{
    TableColList colist;
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_ID, "INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_CODE, "VARCHAR(10) NOT NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_FAVORITE, "BOOL NULL"));
    return createTable(TABLE_FAVORITE, colist);
}

bool HQDBDataBase::updateShareFavInfo(const ShareDataList& dataList)
{
    int size = dataList.size();
    if(size == 0) return false;
    mDB.transaction();
    foreach (ShareData data, dataList) {
        DBColValList list;
        list.append(DBColVal(HQ_TABLE_COL_CODE, data.mCode, HQ_DATA_TEXT));
        list.append(DBColVal(HQ_TABLE_COL_FAVORITE, data.mIsFav, HQ_DATA_INT));
        if(!updateTable(TABLE_SHARE_BASIC_INFO, list, list[0])){
            mDB.rollback();
            return false;
        }
    }
    mDB.commit();
    return true;
}


bool HQDBDataBase::queryShareFavInfo(ShareDataMap& map)
{
    QMutexLocker locker(&mSQLMutex);
    if(!mSQLQuery.exec(tr("select * from %1").arg(TABLE_FAVORITE))) return false;
    while (mSQLQuery.next()) {
        QString code =  mSQLQuery.value(HQ_TABLE_COL_CODE).toString();
        ShareData& info = map[code];
        if(info.mCode.length() == 0) info.mCode = code;
        info.mIsFav = mSQLQuery.value(HQ_TABLE_COL_FAVORITE).toBool();
    }
    return true;
}
bool HQDBDataBase::delShareFavInfo(const QString& code)
{
    DBColValList list;
    if(code.length() > 0)
    {
        list.append(DBColVal(HQ_TABLE_COL_CODE, code, HQ_DATA_TEXT));
    }
     mDB.transaction();
     if(!deleteRecord(TABLE_FAVORITE, list))
     {
         mDB.rollback();
         return false;
     }
     mDB.commit();
     return true;
}


bool HQDBDataBase::createShareProfitTable()
{
    TableColList colist;
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_ID, "INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_CODE, "VARCHAR(10) NOT NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_PROFIT, "NUMERIC NULL"));
    return createTable(TABLE_PROFIT, colist);
}

bool HQDBDataBase::createShareExchangeRecordTable()
{
    TableColList colist;
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_ID, "INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_CODE, "VARCHAR(10) NOT NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_NAME, "VARCHAR(30) NOT NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_DATE, "VARCHAR(10) NOT NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_EXCHANGE_TYPE, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_EXCHANGE_COUNT, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_EXCHANGE_PRICE, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_EXCHANGE_TOTAL, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_EXCHANGE_YONGJIN, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_EXCHANGE_YINHUASUI, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_EXCHANGE_OTHER, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_EXCHANGE_NETINCOME, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_EXCHANGE_SERIAL_NUM, "VARCHAR(30) NULL"));
    return createTable(TABLE_SHARE_EXCHANGE_RECORD, colist);
}

bool HQDBDataBase::updateShareProfitInfo(const ShareDataList& dataList)
{
    int size = dataList.size();
    if(size == 0) return false;
    mDB.transaction();
    foreach (ShareData data, dataList) {
        DBColValList list;
        list.append(DBColVal(HQ_TABLE_COL_CODE, data.mCode, HQ_DATA_TEXT));
        list.append(DBColVal(HQ_TABLE_COL_PROFIT, data.mProfit, HQ_DATA_DOUBLE));
        if(!updateTable(TABLE_PROFIT, list, list[0])){
            mDB.rollback();
            return false;
        }
    }
    mDB.commit();
    return true;
}


bool HQDBDataBase::queryShareProfitInfo(ShareDataMap& map)
{
    QMutexLocker locker(&mSQLMutex);
    if(!mSQLQuery.exec(tr("select * from %1").arg(TABLE_PROFIT))) return false;
    while (mSQLQuery.next()) {
        QString code =  mSQLQuery.value(HQ_TABLE_COL_CODE).toString();
        ShareData& info = map[code];
        if(info.mCode.length() == 0) info.mCode = code;
        info.mProfit = mSQLQuery.value(HQ_TABLE_COL_PROFIT).toDouble();
    }
    return true;
}
bool HQDBDataBase::delShareProfitInfo(const QString& code)
{
    DBColValList list;
    if(code.length() > 0)
    {
        list.append(DBColVal(HQ_TABLE_COL_CODE, code, HQ_DATA_TEXT));
    }
     mDB.transaction();
     if(!deleteRecord(TABLE_PROFIT, list))
     {
         mDB.rollback();
         return false;
     }
     mDB.commit();
     return true;
}

//基本信息更新
bool HQDBDataBase::createShareBasicTable()
{
    TableColList colist;
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_ID, "INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_CODE, "VARCHAR(10) NOT NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_NAME, "VARCHAR(100) NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_PY_ABBR, "VARCHAR(10) NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_TYPE, "INTEGER NOT NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_LISTTIME, "VARCHAR(10) NOT NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_TOTALMNT, "NUMERIC NOT NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_MUTAL, "NUMERIC NOT NULL"));
//    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_FAVORITE, "BOOL NULL"));
    return createTable(TABLE_SHARE_BASIC_INFO, colist);
}


bool HQDBDataBase::updateShareBasicInfo(const ShareDataList& dataList)
{
    int size = dataList.size();
    if(size == 0) return true;
    if(!delShareBasicInfo()) return false;
    mDB.transaction();
    foreach (ShareData data, dataList) {
        DBColValList list;
        list.append(DBColVal(HQ_TABLE_COL_CODE, data.mCode, HQ_DATA_TEXT));
        list.append(DBColVal(HQ_TABLE_COL_NAME, data.mName, HQ_DATA_TEXT));
        list.append(DBColVal(HQ_TABLE_COL_PY_ABBR, data.mPY, HQ_DATA_TEXT));
        list.append(DBColVal(HQ_TABLE_COL_TYPE, data.mType, HQ_DATA_INT));
        list.append(DBColVal(HQ_TABLE_COL_LISTTIME, data.mListTime, HQ_DATA_TEXT));
        list.append(DBColVal(HQ_TABLE_COL_TOTALMNT, data.mZGB, HQ_DATA_DOUBLE));
        list.append(DBColVal(HQ_TABLE_COL_MUTAL, data.mLTGB, HQ_DATA_DOUBLE));
        if(!updateTable(TABLE_SHARE_BASIC_INFO, list, list[0])){
            mDB.rollback();
            return false;
        }
    }
    if(!updateDBUpdateDate(QDate::currentDate(), TABLE_SHARE_BASIC_INFO))
    {
        mDB.rollback();
        return false;
    }
    mDB.commit();
    return true;
}


bool HQDBDataBase::queryShareBasicInfo(ShareDataMap& map, const QStringList& favlist)
{
    QMutexLocker locker(&mSQLMutex);
    if(!mSQLQuery.exec(tr("select * from %1").arg(TABLE_SHARE_BASIC_INFO))) return false;
    while (mSQLQuery.next()) {
        QString code = mSQLQuery.value(HQ_TABLE_COL_CODE).toString();
        if(code.length() <= 6) code = ShareData::prefixCode(code) + code;
        ShareData& info = map[code];
        info.mCode = code;
        info.mName = mSQLQuery.value(HQ_TABLE_COL_NAME).toString();
        info.mPY = mSQLQuery.value(HQ_TABLE_COL_PY_ABBR).toString();
        info.mType =  mSQLQuery.value(HQ_TABLE_COL_TYPE).toInt();
        info.mListTime = mSQLQuery.value(HQ_TABLE_COL_LISTTIME).toString();
        info.mZGB = mSQLQuery.value(HQ_TABLE_COL_TOTALMNT).toDouble();
        info.mLTGB = mSQLQuery.value(HQ_TABLE_COL_MUTAL).toDouble();
        info.mIsFav = favlist.contains(code);

    }
    return true;
}
bool HQDBDataBase::delShareBasicInfo(const QString& code)
{
    DBColValList list;
    if(code.length() > 0)
    {
        list.append(DBColVal(HQ_TABLE_COL_CODE, code, HQ_DATA_TEXT));
    }
     mDB.transaction();
     if(!deleteRecord(TABLE_SHARE_BASIC_INFO, list))
     {
         mDB.rollback();
         return false;
     }
     mDB.commit();
     return true;
}

bool HQDBDataBase::getSimilarCodeOfText(QStringList &codes, const QString &text)
{
    QMutexLocker locker(&mSQLMutex);
    QRegExp num("[0-9]{1,6}");
    QRegExp alpha("[A-Z]{1,6}");
    bool sts = false;
    if(num.exactMatch(text.toUpper()))
    {
        sts = mSQLQuery.exec(QString("select %1 from %2 where %3 like '%%4%' limit 10")\
                             .arg(HQ_TABLE_COL_CODE)\
                             .arg(TABLE_SHARE_BASIC_INFO)\
                             .arg(HQ_TABLE_COL_CODE)\
                             .arg(text));
    } else {
        sts = mSQLQuery.exec(QString("select %1 from %2 where %3 like '%%4%' limit 10")\
                             .arg(HQ_TABLE_COL_CODE)\
                             .arg(TABLE_SHARE_BASIC_INFO)\
                             .arg(HQ_TABLE_COL_PY_ABBR)\
                             .arg(text));
    }
    if(!sts) return false;
    while (mSQLQuery.next()) {
        codes.append(mSQLQuery.value(0).toString());
    }

    if(codes.length() == 0)
    {
        if(num.exactMatch(text.toUpper()))
        {
            sts = mSQLQuery.exec(QString("select %1 from %2 where %3 = '%4' limit 10")\
                                 .arg(HQ_TABLE_COL_CODE)\
                                 .arg(TABLE_SHARE_BASIC_INFO)\
                                 .arg(HQ_TABLE_COL_CODE)\
                                 .arg(text));
        } else {
            sts = mSQLQuery.exec(QString("select %1 from %2 where %3 = '%4' limit 10")\
                                 .arg(HQ_TABLE_COL_CODE)\
                                 .arg(TABLE_SHARE_BASIC_INFO)\
                                 .arg(HQ_TABLE_COL_PY_ABBR)\
                                 .arg(text));
        }
        if(!sts) return false;
        while (mSQLQuery.next()) {
            codes.append(mSQLQuery.value(0).toString());
        }
    }

    return true;
}


bool HQDBDataBase::createShareHistoryInfoTable(const QString& code)
{
    QString table = QString(TABLE_SHARE_HISTORY_TEMPLATE).arg(code);
    TableColList colist;
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_ID, "INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_CODE, "VARCHAR(6) NOT NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_NAME, "VARCHAR(100) NOT NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_CLOSE, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_CHANGE_PERCENT, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_CLOSE_LAST, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_CLOSE_LAST_ADJUST, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_VOL, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_MONEY, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_ZJLX, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_RZRQ, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_HSGT_HAVE, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_HSGT_HAVE_PERCENT, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_HSGT_HAVE_CHANGE, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_HSGT_TOP10_MONEY, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_HSGT_TOP10_FLAG, "BOOL NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_TOTALMNT, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_MUTAL, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_DATE, "VARCHAR(10) NOT NULL"));
    return createTable(table, colist);
}

bool HQDBDataBase::createShareHistoryCounterTable()
{
    TableColList colist;
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_ID, "INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_CODE, "VARCHAR(6) NOT NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_NAME, "VARCHAR(100) NOT NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_CLOSE, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_CHANGE_PERCENT, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_MONEY, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_WEEK_CHANGE, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_MONTH_CHANGE, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_YEAR_CHANGE, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_FOREIGN_HOLD, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_FOREIGN_HOLD_CHANGE_DAY, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_FOREIGN_HOLD_CHANGE_MONTH, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_FOREIGN_HOLD_CHANGE_WEEK, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_FOREIGN_HOLD_CHANGE_YEAR, "NUMERIC NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_DATE, "VARCHAR(10) NOT NULL"));
    return createTable(TABLE_SHARE_HISTORY_COUNTER, colist);
}

bool HQDBDataBase::queryShareHistroyUpdatedDates(QList<QDate> &list)
{
#if 0
    QString sql = QString("select distinct date from %1 order by date desc").arg(TABLE_SHARE_HISTORY);
    QMutexLocker locker(&mSQLMutex);
    if(!mSQLQuery.exec(sql)) return false;
    while (mSQLQuery.next()) {
        list.append(QDateTime::fromString(mSQLQuery.value(HQ_TABLE_COL_DATE).toString()).date());
    }
#endif
    return true;
}

bool HQDBDataBase::updateShareHistory(const ShareDataList &dataList, int mode)
{
#if 0
    if(dataList.size() == 0) return true;
    if(!createShareHistoryInfoTable()) return false;
    mDB.transaction();
    foreach (ShareData data, dataList) {
        QDate date(data.mTime.date());
        DBColValList list;
        list.append(DBColVal(HQ_TABLE_COL_CODE, data.mCode, HQ_DATA_TEXT));
        list.append(DBColVal(HQ_TABLE_COL_DATE, date.toString(), HQ_DATA_TEXT));
        list.append(DBColVal(HQ_TABLE_COL_NAME, data.mName, HQ_DATA_TEXT));
        if(mode & History_Zjlx)
        {
            list.append(DBColVal(HQ_TABLE_COL_ZJLX, data.mZJLX, HQ_DATA_DOUBLE));
        }
        if(mode & History_Rzrq)
        {
            list.append(DBColVal(HQ_TABLE_COL_RZRQ, data.mRZRQ, HQ_DATA_DOUBLE));
        }
        if(mode & History_HsgtVol)
        {
            list.append(DBColVal(HQ_TABLE_COL_HSGT_HAVE, data.mHsgtData.mVolTotal, HQ_DATA_LONG));
            list.append(DBColVal(HQ_TABLE_COL_HSGT_HAVE_PERCENT, data.mHsgtData.mVolMutablePercent, HQ_DATA_DOUBLE));
            list.append(DBColVal(HQ_TABLE_COL_HSGT_HAVE_CHANGE, data.mHsgtData.mVolChange, HQ_DATA_LONG));
        }
        if(mode & History_HsgtTop10)
        {
            list.append(DBColVal(HQ_TABLE_COL_HSGT_TOP10_MONEY, data.mHsgtData.mPure, HQ_DATA_DOUBLE));
            list.append(DBColVal(HQ_TABLE_COL_HSGT_TOP10_FLAG, true, HQ_DATA_INT));
        }
        if(mode & History_Close)
        {
            list.append(DBColVal(HQ_TABLE_COL_CLOSE, data.mClose, HQ_DATA_DOUBLE));
            list.append(DBColVal(HQ_TABLE_COL_CHANGE_PERCENT, data.mChgPercent, HQ_DATA_DOUBLE));
            list.append(DBColVal(HQ_TABLE_COL_VOL, data.mVol, HQ_DATA_DOUBLE));
            list.append(DBColVal(HQ_TABLE_COL_MONEY, data.mMoney, HQ_DATA_DOUBLE));
            list.append(DBColVal(HQ_TABLE_COL_TOTALMNT, data.mFinanceData.mTotalShare, HQ_DATA_DOUBLE));
            list.append(DBColVal(HQ_TABLE_COL_MUTAL, data.mFinanceData.mMutalShare, HQ_DATA_DOUBLE));
        }
        DBColValList key;
        key.append(list[0]);
        key.append(list[1]);
        if(!updateTable(TABLE_SHARE_HISTORY, list, key, false)){
            mDB.rollback();
            return false;
        }
//        if(!updateDBUpdateDate(date, TABLE_SHARE_HISTORY))
//        {
//            mDB.rollback();
//            return false;
//        }
    }
    mDB.commit();
#endif
    return true;

}

bool HQDBDataBase::queryShareHistory(ShareDataList &list, const QString &share_code, const QDate &start, const QDate &end)
{
#if 0
    if(share_code.length() == 0) return false;
    DBColValList wherelist;
    if(!start.isNull())
    {
        wherelist.append(DBColVal(HQ_TABLE_COL_DATE, start.toString(), HQ_DATA_TEXT, HQ_COMPARE_GREAT));
    }
    if(share_code.length() > 0)
    {
        wherelist.append(DBColVal(HQ_TABLE_COL_CODE, share_code, HQ_DATA_TEXT));
    }
    if(!end.isNull())
    {
        wherelist.append(DBColVal(HQ_TABLE_COL_DATE, end.toString(), HQ_DATA_TEXT, HQ_COMPARE_LESS));
    }
    QString sql = QString("select * from %1 %2 order by %3 desc").arg(TABLE_SHARE_HISTORY).arg(wherelist.whereString()).arg(HQ_TABLE_COL_DATE);
    QMutexLocker locker(&mSQLMutex);
    if(!mSQLQuery.exec(sql)) return false;
    while (mSQLQuery.next()) {
        ShareData data;
        data.mCode = mSQLQuery.value(HQ_TABLE_COL_CODE).toString();
        data.mName = mSQLQuery.value(HQ_TABLE_COL_NAME).toString();
        data.mClose = mSQLQuery.value(HQ_TABLE_COL_CLOSE).toDouble();
        data.mChgPercent = mSQLQuery.value(HQ_TABLE_COL_CHANGE_PERCENT).toDouble();
        data.mVol = mSQLQuery.value(HQ_TABLE_COL_VOL).toDouble();
        data.mMoney = mSQLQuery.value(HQ_TABLE_COL_MONEY).toDouble();

        data.mRZRQ = mSQLQuery.value(HQ_TABLE_COL_RZRQ).toDouble();
        data.mZJLX = mSQLQuery.value(HQ_TABLE_COL_ZJLX).toDouble();
        data.mHsgtData.mVolTotal = mSQLQuery.value(HQ_TABLE_COL_HSGT_HAVE).toDouble();
        data.mHsgtData.mVolChange = mSQLQuery.value(HQ_TABLE_COL_HSGT_HAVE_CHANGE).toDouble();
        data.mHsgtData.mVolMutablePercent = mSQLQuery.value(HQ_TABLE_COL_HSGT_HAVE_PERCENT).toDouble();
        data.mHsgtData.mPure = mSQLQuery.value(HQ_TABLE_COL_HSGT_TOP10_MONEY).toDouble();
        data.mHsgtData.mIsTop10 = mSQLQuery.value(HQ_TABLE_COL_HSGT_TOP10_FLAG).toDouble();
        data.mFinanceData.mMutalShare = mSQLQuery.value(HQ_TABLE_COL_MUTAL).toDouble();
        data.mFinanceData.mTotalShare = mSQLQuery.value(HQ_TABLE_COL_TOTALMNT).toDouble();
        data.mTime = QDateTime::fromString(mSQLQuery.value(HQ_TABLE_COL_DATE).toString());
        list.append(data);
    }
#endif
    return true;
}

bool HQDBDataBase::delShareHistory(const QString &share_code, const QDate &start, const QDate &end)
{
#if 0
    //if(share_code.length() == 0) return false;
    DBColValList wherelist;
    if(!start.isNull())
    {
        wherelist.append(DBColVal(HQ_TABLE_COL_DATE, start.toString(), HQ_DATA_TEXT, HQ_COMPARE_GREAT));
    }
    if(share_code.length() > 0)
    {
        wherelist.append(DBColVal(HQ_TABLE_COL_CODE, share_code, HQ_DATA_TEXT));
    }
    if(!end.isNull())
    {
        wherelist.append(DBColVal(HQ_TABLE_COL_DATE, end.toString(), HQ_DATA_TEXT, HQ_COMPARE_LESS));
    }
    return deleteRecord(TABLE_SHARE_HISTORY, wherelist);
#endif
    return true;
}

double HQDBDataBase::getMultiDaysChangePercent(const QString &code, HISTORY_CHANGEPERCENT type)
{
    double change = 0.0;
    QMutexLocker locker(&mSQLMutex);
    if(!mSQLQuery.exec(tr("select 1+%1 * 0.01 from %2 %3 order by %4 desc limit %5")
                       .arg(HQ_TABLE_COL_CHANGE_PERCENT)
                       .arg(HISTORY_TABLE(code))
                       .arg(DBColValList(DBColVal(HQ_TABLE_COL_CODE, code, HQ_DATA_TEXT)).whereString())
                       .arg(HQ_TABLE_COL_DATE)
                       .arg(type)))
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
    QMutexLocker locker(&mSQLMutex);
    if(!mSQLQuery.exec(tr("select %1 from %2 %3' order by %4 desc limit 1")
                       .arg(HQ_TABLE_COL_MONEY)
                       .arg(HISTORY_TABLE(code))
                       .arg(DBColValList(DBColVal(HQ_TABLE_COL_CODE, code, HQ_DATA_TEXT)).whereString())
                       .arg(HQ_TABLE_COL_DATE)))
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
    QMutexLocker locker(&mSQLMutex);
    if(!mSQLQuery.exec(tr("select %1 from %2 %3 order by %4 desc limit 2")
                       .arg(HQ_TABLE_COL_HSGT_HAVE)
                       .arg(HISTORY_TABLE(code))
                       .arg(DBColValList(DBColVal(HQ_TABLE_COL_CODE, code, HQ_DATA_TEXT)).whereString())
                       .arg(HQ_TABLE_COL_DATE)))
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

bool HQDBDataBase::queryHistoryInfoFromDate(ShareDataList& list, const QString& code, const QDate& date)
{
    return queryShareHistory(list, code, date);
}

//shareholders
bool HQDBDataBase::createShareHoldersTable()
{
    TableColList colist;
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_ID, "INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_SHARE_CODE, "VARCHAR(6) NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_HOLDER_CODE, "VARCHAR(10) NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_HOLDER_NAME, "VARCHAR(100) NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_HOLDER_VOL, "DOUBLE NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_HOLDER_FUND_PERCENT, "DOUBLE NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_HOLDER_DATE, "INTEGER NULL"));
    return createTable(TABLE_SHARE_HOLEDER, colist);
}

bool HQDBDataBase::updateShareHolder(const ShareHolderList &dataList)
{
    int size = dataList.size();
    if(size == 0) return false;
    mDB.transaction();
    for(int i=0; i<size; i++)
    {
        ShareHolder data = dataList[i];
        DBColValList list;
        list.append(DBColVal(HQ_TABLE_COL_SHARE_CODE, data.mShareCode,HQ_DATA_TEXT));
        list.append(DBColVal(HQ_TABLE_COL_HOLDER_CODE, data.mHolderCode,HQ_DATA_TEXT));
        list.append(DBColVal(HQ_TABLE_COL_HOLDER_NAME, data.mName,HQ_DATA_TEXT));
        list.append(DBColVal(HQ_TABLE_COL_HOLDER_VOL, data.mShareCount, HQ_DATA_DOUBLE));
        list.append(DBColVal(HQ_TABLE_COL_HOLDER_DATE, QDateTime(data.mDate).toTime_t(), HQ_DATA_INT));
        list.append(DBColVal(HQ_TABLE_COL_HOLDER_FUND_PERCENT, data.mFundPercent, HQ_DATA_DOUBLE));

        DBColValList key;
        key.append(list[0]);
        key.append(list[1]);
        key.append(list[4]);
        if(!updateTable(TABLE_SHARE_HOLEDER, list, key)){
            mDB.rollback();
        }
    }

    if(!updateDBUpdateDate(QDate::currentDate(), TABLE_SHARE_HOLEDER))
    {
        mDB.rollback();
    }
    mDB.commit();
}

bool HQDBDataBase::queryShareHolder(ShareHolderList &list, const QString& share_code, const QString& holder_code, const QDate &date)
{    
    DBColValList wherelist;
    if(!date.isNull())
    {
        wherelist.append(DBColVal(HQ_TABLE_COL_HOLDER_DATE, QDateTime(date).toTime_t(), HQ_DATA_INT));
    }
    if(share_code.length() > 0)
    {
        wherelist.append(DBColVal(HQ_TABLE_COL_SHARE_CODE, share_code, HQ_DATA_TEXT));
    }
    if(holder_code.length() > 0)
    {
        wherelist.append(DBColVal(HQ_TABLE_COL_SHARE_CODE, holder_code, HQ_DATA_TEXT));
    }
    QString sql = QString("select * from %1 %2").arg(TABLE_SHARE_HOLEDER).arg(wherelist.whereString());
    QMutexLocker locker(&mSQLMutex);
    if(!mSQLQuery.exec(sql)) return false;
    while (mSQLQuery.next()) {
        ShareHolder data;
        data.mShareCode = mSQLQuery.value(HQ_TABLE_COL_SHARE_CODE).toString();
        data.mHolderCode = mSQLQuery.value(HQ_TABLE_COL_HOLDER_CODE).toString();
        data.mName = mSQLQuery.value(HQ_TABLE_COL_HOLDER_NAME).toString();
        data.mShareCount = mSQLQuery.value(HQ_TABLE_COL_HOLDER_VOL).toDouble();
        data.mDate = QDateTime::fromTime_t(mSQLQuery.value(HQ_TABLE_COL_HOLDER_DATE).toInt()).date();
        data.mFundPercent = mSQLQuery.value(HQ_TABLE_COL_HOLDER_FUND_PERCENT).toDouble();
        list.append(data);
    }
    return true;
}

bool HQDBDataBase::delShareHolder(const QString& share_code, const QString& holder_code, const QDate &date)
{
    DBColValList wherelist;
    if(!date.isNull())
    {
        wherelist.append(DBColVal(HQ_TABLE_COL_HOLDER_DATE, QDateTime(date).toTime_t(), HQ_DATA_INT));
    }
    if(share_code.length() > 0)
    {
        wherelist.append(DBColVal(HQ_TABLE_COL_SHARE_CODE, share_code, HQ_DATA_TEXT));
    }
    if(holder_code.length() > 0)
    {
        wherelist.append(DBColVal(HQ_TABLE_COL_SHARE_CODE, holder_code, HQ_DATA_TEXT));
    }

    return deleteRecord(TABLE_SHARE_HOLEDER, wherelist);
}


//财务信息等操作
bool HQDBDataBase::createShareFinancialInfoTable()
{
    TableColList colist;
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_ID, "INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_CODE, "VARCHAR(6) NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_FINANCE_MGSY, "DOUBLE NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_FINANCE_JZC, "DOUBLE NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_FINANCE_JZCSYL, "DOUBLE NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_TOTALMNT, "double NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_MUTAL, "double NULL"));
    return createTable(TABLE_SHARE_FINANCE, colist);
}

bool HQDBDataBase::updateShareFinance(const FinancialDataList& dataList)
{
    int size = dataList.size();
    if(size > 0)
    {
        mDB.transaction();
        for(int i=0; i<size; i++)
        {
            FinancialData data = dataList[i];
            DBColValList list;
            list.append(DBColVal(HQ_TABLE_COL_CODE, data.mCode,HQ_DATA_TEXT));
            list.append(DBColVal(HQ_TABLE_COL_FINANCE_JZC, data.mBVPS,HQ_DATA_DOUBLE));
            list.append(DBColVal(HQ_TABLE_COL_FINANCE_JZCSYL, data.mROE,HQ_DATA_DOUBLE));
            list.append(DBColVal(HQ_TABLE_COL_FINANCE_MGSY, data.mEPS,HQ_DATA_DOUBLE));
            list.append(DBColVal(HQ_TABLE_COL_TOTALMNT, data.mTotalShare,HQ_DATA_DOUBLE));
            list.append(DBColVal(HQ_TABLE_COL_MUTAL, data.mMutalShare,HQ_DATA_DOUBLE));
            if(!updateTable(TABLE_SHARE_FINANCE, list, list[0])){
                mDB.rollback();
                return false;
            }
        }
        if(!updateDBUpdateDate(QDate::currentDate(), TABLE_SHARE_FINANCE))
        {
            mDB.rollback();
            return false;
        }
        mDB.commit();
    }
    return true;
}

bool HQDBDataBase::queryShareFinance(FinancialDataList& list, const QStringList& codes)
{
    DBColValList where;
    if(codes.length() > 0)
    {
        where.append(DBColVal(HQ_TABLE_COL_CODE, codes, HQ_DATA_TEXT, HQ_COMPARE_TEXTIN));
    }

    QString sql = QString("select * from %1 %2").arg(TABLE_SHARE_FINANCE).arg(where.whereString());
    QMutexLocker locker(&mSQLMutex);
    if(!mSQLQuery.exec(sql)) return false;
    while (mSQLQuery.next()) {
        FinancialData data;
        data.mCode = mSQLQuery.value(HQ_TABLE_COL_CODE).toString();
        data.mBVPS = mSQLQuery.value(HQ_TABLE_COL_FINANCE_JZC).toDouble();
        data.mEPS = mSQLQuery.value(HQ_TABLE_COL_FINANCE_MGSY).toDouble();
        data.mROE = mSQLQuery.value(HQ_TABLE_COL_FINANCE_JZCSYL).toDouble();
        data.mTotalShare = mSQLQuery.value(HQ_TABLE_COL_TOTALMNT).toDouble();
        data.mMutalShare = mSQLQuery.value(HQ_TABLE_COL_MUTAL).toDouble();
        list.append(data);
    }
    return true;
}

bool HQDBDataBase::delShareFinance(const QString& code)
{
    DBColValList list;
    if(code.length() > 0)
    {
        list.append(DBColVal(HQ_TABLE_COL_CODE, code, HQ_DATA_TEXT));
    }

    return deleteRecord(TABLE_SHARE_FINANCE, list);
}



//分红送配信息的操作
bool HQDBDataBase::createShareBonusIbfoTable()
{
    TableColList colist;
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_ID, "INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_CODE, "VARCHAR(6) NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_BONUS_SHARE, "DOUBLE NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_BONUS_MONEY, "DOUBLE NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_BONUS_YAGGR, "VARCHAR(10) NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_BONUS_GQDJR, "VARCHAR(10) NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_BONUS_DATE, "VARCHAR(10) NOT NULL"));
    return createTable(TABLE_SHARE_BONUS, colist);
}

bool HQDBDataBase::updateShareBonus(const ShareBonusList& bonusList)
{
    int size = bonusList.size();
    if(size > 0)
    {
        mDB.transaction();
        for(int i=0; i<size; i++)
        {
            ShareBonus bonus = bonusList[i];
            DBColValList list;
            list.append(DBColVal(HQ_TABLE_COL_CODE, bonus.mCode, HQ_DATA_TEXT));
            list.append(DBColVal(HQ_TABLE_COL_BONUS_DATE, bonus.mDate.toString(), HQ_DATA_TEXT));
            list.append(DBColVal(HQ_TABLE_COL_BONUS_SHARE, bonus.mSZZG, HQ_DATA_DOUBLE));
            list.append(DBColVal(HQ_TABLE_COL_BONUS_MONEY, bonus.mXJFH, HQ_DATA_DOUBLE));
            list.append(DBColVal(HQ_TABLE_COL_BONUS_YAGGR, bonus.mYAGGR.toString(), HQ_DATA_TEXT));
            list.append(DBColVal(HQ_TABLE_COL_BONUS_GQDJR, bonus.mGQDJR.toString(), HQ_DATA_TEXT));
            if(!updateTable(TABLE_SHARE_BONUS, list, list.mid(0, 2))){
                mDB.rollback();
                return false;
            }
        }

        if(!updateDBUpdateDate(bonusList.last().mDate, TABLE_SHARE_BONUS))
        {
            mDB.rollback();
            return false;
        }
        mDB.commit();
    }
    return true;
}

bool HQDBDataBase::queryShareBonus(QList<ShareBonus>& list, const QString& code, const QDate& date)
{
    QString sql = QString("select * from %1").arg(TABLE_SHARE_BONUS);
    if(code.length() > 0 || !(date.isNull())){
        sql.append(SQL_WHERE);
        if(code.length() > 0) {
            sql.append(QString("%1 = '%2'").arg(HQ_TABLE_COL_CODE).arg(code));
        }
        if(!date.isNull()) {
            sql.append(QString("%1 = '%2'").arg(HQ_TABLE_COL_BONUS_DATE).arg(date.toString()));
        }
    } else
    {
        sql = "select * from share_bonus  group by code order by code desc";
    }
    QMutexLocker locker(&mSQLMutex);
    if(!mSQLQuery.exec(sql)) return false;
    while (mSQLQuery.next()) {
        ShareBonus bonus;
        bonus.mCode = mSQLQuery.value(HQ_TABLE_COL_CODE).toString();
        bonus.mDate = QDate::fromString(mSQLQuery.value(HQ_TABLE_COL_BONUS_DATE).toString());
        bonus.mGQDJR = QDate::fromString(mSQLQuery.value(HQ_TABLE_COL_BONUS_GQDJR).toString());
        bonus.mSZZG = mSQLQuery.value(HQ_TABLE_COL_BONUS_SHARE).toDouble();
        bonus.mXJFH = mSQLQuery.value(HQ_TABLE_COL_BONUS_MONEY).toDouble();
        bonus.mYAGGR = QDate::fromString(mSQLQuery.value(HQ_TABLE_COL_BONUS_YAGGR).toString());
        list.append(bonus);
    }
    return true;
}

bool HQDBDataBase::delShareBonus(const QString& code, const QDate& date)
{
    DBColValList list;
    if(code.length() > 0)
    {
        list.append(DBColVal(HQ_TABLE_COL_CODE, code, HQ_DATA_TEXT));
    }
    if(!date.isNull())
    {
        list.append(DBColVal(HQ_TABLE_COL_BONUS_DATE, QDateTime(date).toTime_t(),HQ_DATA_INT));
    }

    return deleteRecord(TABLE_SHARE_BONUS, list);
}


//陆股通TOP10
bool HQDBDataBase::createShareHsgTop10Table()
{
    TableColList colist;
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_ID, "INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_CODE, "VARCHAR(6) NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_NAME, "VARCHAR(100) NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_HSGT_TOP10_BUY, "DOUBLE NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_HSGT_TOP10_SELL, "DOUBLE NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_HSGT_TOP10_MONEY, "DOUBLE NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_DATE, "VARCHAR(10) NOT NULL"));
    return createTable(TABLE_HSGT_TOP10, colist);
}

bool HQDBDataBase::updateShareHsgtTop10List(const ShareHsgtList& dataList)
{
    int size = dataList.size();
    if(size > 0)
    {
        QDate update_date;
        mDB.transaction();
        foreach(ShareHsgt data, dataList)
        {
            DBColValList list;
            list.append(DBColVal(HQ_TABLE_COL_CODE, data.mCode, HQ_DATA_TEXT));
            list.append(DBColVal(HQ_TABLE_COL_NAME, data.mName, HQ_DATA_TEXT));
            list.append(DBColVal(HQ_TABLE_COL_DATE, data.mDate.toString(), HQ_DATA_TEXT));
            list.append(DBColVal(HQ_TABLE_COL_HSGT_TOP10_BUY, data.mBuy, HQ_DATA_DOUBLE));
            list.append(DBColVal(HQ_TABLE_COL_HSGT_TOP10_SELL, data.mSell, HQ_DATA_DOUBLE));
            list.append(DBColVal(HQ_TABLE_COL_HSGT_TOP10_MONEY, data.mPure, HQ_DATA_DOUBLE));
            DBColValList key;
            key.append(list[0]);
            key.append(list[2]);
            if(!updateTable(TABLE_HSGT_TOP10, list, key)){
                mDB.rollback();
                return false;
            }
        }
        if(update_date.isNull() && dataList.size() > 0)
        {
            update_date = dataList.last().mDate;
            if(!updateDBUpdateDate(update_date, TABLE_HSGT_TOP10))
            {
                mDB.rollback();
                return false;
            }
        }
        mDB.commit();
    }
    return true;
}

bool HQDBDataBase::queryShareHsgtTop10List(ShareHsgtList& list, const QString& code, const QDate& date)
{
    DBColValList whereList;
    if(code.length() > 0 )
    {
        whereList.append(DBColVal(HQ_TABLE_COL_CODE, code, HQ_DATA_TEXT));
    }
    if(!date.isNull())
    {
        whereList.append(DBColVal(HQ_TABLE_COL_DATE, date.toString(), HQ_DATA_TEXT));
    }
    QString sql = QString("select * from %1 %2 order by %3 desc, %4 desc limit 100").arg(TABLE_HSGT_TOP10).arg(whereList.whereString()).arg(HQ_TABLE_COL_DATE).arg(HQ_TABLE_COL_HSGT_TOP10_MONEY);
    QMutexLocker locker(&mSQLMutex);
    if(!mSQLQuery.exec(sql)) return false;
    while (mSQLQuery.next()) {
        ShareHsgt data;
        data.mCode = mSQLQuery.value(HQ_TABLE_COL_CODE).toString();
        data.mName = mSQLQuery.value(HQ_TABLE_COL_NAME).toString();
        data.mBuy = mSQLQuery.value(HQ_TABLE_COL_HSGT_TOP10_BUY).toDouble();
        data.mSell = mSQLQuery.value(HQ_TABLE_COL_HSGT_TOP10_SELL).toDouble();
        data.mPure = mSQLQuery.value(HQ_TABLE_COL_HSGT_TOP10_MONEY).toDouble();
        data.mDate = QDateTime::fromString(mSQLQuery.value(HQ_TABLE_COL_DATE).toString()).date();
        list.append(data);
    }
    return true;
}

bool HQDBDataBase::delShareHsgtTop10(const QString& code, const QDate& date)
{
    DBColValList list;
    if(code.length() > 0)
    {
        list.append(DBColVal(HQ_TABLE_COL_CODE, code, HQ_DATA_TEXT));
    }
    if(!date.isNull())
    {
        list.append(DBColVal(HQ_TABLE_COL_DATE, date.toString(),HQ_DATA_TEXT));
    }

    return deleteRecord(TABLE_HSGT_TOP10, list);
}

//数据表更新日期的相关操作
bool HQDBDataBase::createDBUpdateDateTable()
{
    TableColList colist;
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_ID, "INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_TABLE_NM, "VARCHAR(100) NULL"));
    colist.append(TABLE_COL_DEF(HQ_TABLE_COL_DATE, "VARCHAR(10) NOT NULL"));
    return createTable(TABLE_DB_UPDATE, colist);
}

bool HQDBDataBase::queryDBUpdateDate(QDate &date, const QString &table)
{
    QMutexLocker locker(&mSQLMutex);
    QString sql = QString("select %1 from %2 where %3 = '%4' ").arg(HQ_TABLE_COL_DATE).arg(TABLE_DB_UPDATE).arg(HQ_TABLE_COL_TABLE_NM).arg(table);
    if(!mSQLQuery.exec(sql)) return false;
    while (mSQLQuery.next()) {
        date = QDate::fromString(mSQLQuery.value(0).toString());
        break;
    }
    return true;
}

bool HQDBDataBase::updateDBUpdateDate(const QDate& date, const QString& table)
{
    DBColValList list;
    list.append(DBColVal(HQ_TABLE_COL_TABLE_NM, table, HQ_DATA_TEXT));
    list.append(DBColVal(HQ_TABLE_COL_DATE, date.toString(), HQ_DATA_TEXT));
    return updateTable(TABLE_DB_UPDATE, list, list[0]);
}

bool HQDBDataBase::delDBUpdateDate(const QString &table)
{
    DBColValList list;
    if(table.length() > 0)
    {
        list.append(DBColVal(HQ_TABLE_COL_TABLE_NM, table, HQ_DATA_TEXT));
    }

    return deleteRecord(TABLE_DB_UPDATE, list);
}

bool HQDBDataBase::updateTable(const QString& tableName, const DBColValList& values, const DBColValList& key, bool check )
{
    //检查记录已经添加
    bool exist = false;
    if( check && (!isRecordExist(exist, tableName, key))) return false;
    QMutexLocker locker(&mSQLMutex);
    if(exist){
        //更新
        return mSQLQuery.exec(QString("update %1 set %2 %3").arg(tableName).arg(values.updateString()).arg(key.whereString()));
    } else {
        //添加
        return mSQLQuery.exec(QString("insert into %1 %2").arg(tableName).arg(values.insertString()));
    }
    return true;
}

bool HQDBDataBase::deleteRecord(const QString &table, const DBColValList &list)
{
    QMutexLocker locker(&mSQLMutex);
    QString sql = QString("delete from %1 %2").arg(table).arg(list.whereString());
    qDebug()<<__FUNCTION__<<sql;
    if(!mSQLQuery.exec(sql)) return false;
    return true;
}

bool HQDBDataBase::isRecordExist(bool& exist, const QString& table, const DBColValList& list)
{
    exist = false;
    if(list.size() == 0) return true;
    QMutexLocker locker(&mSQLMutex);
    if(!mSQLQuery.exec(QString("select count(1) from %1 %2 ").arg(table).arg(list.whereString()))) return false;
    while (mSQLQuery.next()) {
        exist = mSQLQuery.value(0).toBool();
        break;
    }
    return true;
}

QDate HQDBDataBase::getLastUpdateDateOfTable(const QString &table)
{
    QDate date;
    queryDBUpdateDate(date, table);
    return date;
}

QDate HQDBDataBase::getLastHistoryDateOfShare(/*const QString &code*/)
{
#if 0
    return getLastUpdateDateOfTable(TABLE_SHARE_HISTORY);
#endif
    return QDate();
}



QString HQDBDataBase::errMsg()
{
    return QString("sql:%1\\nerr:%2").arg(mSQLQuery.lastQuery()).arg(mSQLQuery.lastError().text());
}


bool HQDBDataBase::updateExhangeRecordList(const QList<ShareExchangeData>& srclist)
{
    int size = srclist.size();
    if(size > 0)
    {
        int total_size = srclist.size();
        int index = 0;
        mDB.transaction();
        foreach(ShareExchangeData data, srclist)
        {
            qDebug()<<"now update list:"<<index << total_size;
            DBColValList list;
            list.append(DBColVal(HQ_TABLE_COL_CODE, data.mCode, HQ_DATA_TEXT));
            list.append(DBColVal(HQ_TABLE_COL_NAME, data.mName, HQ_DATA_TEXT));
            list.append(DBColVal(HQ_TABLE_COL_DATE, data.mDateTime, HQ_DATA_TEXT));
            list.append(DBColVal(HQ_TABLE_COL_EXCHANGE_TYPE, data.mType, HQ_DATA_DOUBLE));
            list.append(DBColVal(HQ_TABLE_COL_EXCHANGE_COUNT, data.mNum, HQ_DATA_DOUBLE));
            list.append(DBColVal(HQ_TABLE_COL_EXCHANGE_PRICE, data.mPrice, HQ_DATA_DOUBLE));
            list.append(DBColVal(HQ_TABLE_COL_EXCHANGE_TOTAL, data.mMoney, HQ_DATA_DOUBLE));
            list.append(DBColVal(HQ_TABLE_COL_EXCHANGE_YONGJIN, data.mYongjin, HQ_DATA_DOUBLE));
            list.append(DBColVal(HQ_TABLE_COL_EXCHANGE_YINHUASUI, data.mYinhuasui, HQ_DATA_DOUBLE));
            list.append(DBColVal(HQ_TABLE_COL_EXCHANGE_OTHER, data.mOther, HQ_DATA_DOUBLE));
            list.append(DBColVal(HQ_TABLE_COL_EXCHANGE_NETINCOME, data.mNetIncome, HQ_DATA_DOUBLE));
            list.append(DBColVal(HQ_TABLE_COL_EXCHANGE_SERIAL_NUM, data.mSerialText, HQ_DATA_TEXT));

            DBColValList key;
            key.append(list.last());
            if(!updateTable(TABLE_SHARE_EXCHANGE_RECORD, list, key)){
                mDB.rollback();
                return false;
            }
            index ++;
            qDebug()<<"now update list:"<<index << total_size;
        }
        mDB.commit();
    }
    return true;
}

bool HQDBDataBase::queryExchangeRecord(QList<ShareExchangeData>& list, int& total_page, int page, const QString& code, const QString& start_date, const QString& end_date)
{
    DBColValList whereList;
    if(code.length() > 0 )
    {
        whereList.append(DBColVal(HQ_TABLE_COL_CODE, code, HQ_DATA_TEXT));
    }
    if(!start_date.isNull())
    {
        whereList.append(DBColVal(HQ_TABLE_COL_DATE, start_date, HQ_DATA_TEXT, HQ_COMPARE_GREAT_EQUAL));
    }

    if(!end_date.isNull())
    {
        whereList.append(DBColVal(HQ_TABLE_COL_DATE, end_date, HQ_DATA_TEXT, HQ_COMPARE_LESS_EQUAL));
    }
    QString sql = "";
    QMutexLocker locker(&mSQLMutex);

    sql = QString("select count(1) from %1 %2").arg(TABLE_SHARE_EXCHANGE_RECORD).arg(whereList.whereString());
    if(!mSQLQuery.exec(sql)) return false;
    int total_rows = 0;
    while (mSQLQuery.next()) {
        total_rows = mSQLQuery.value(0).toInt();
        break;
    }
    int page_size = 50;
    total_page = (total_rows + page_size -1) / page_size;

    //判断是否需要分页显示
    QString pagestr;
    if(page > 0)
    {
        int startId = (page - 1)  * page_size;
        pagestr = QString(" limit %1,%2").arg(startId).arg(page_size);
    }
    sql = QString("select * from %1 %2 order by %3 desc, %4 desc %5").arg(TABLE_SHARE_EXCHANGE_RECORD).arg(whereList.whereString()).arg(HQ_TABLE_COL_DATE).arg(HQ_TABLE_COL_CODE).arg(pagestr);

    if(!mSQLQuery.exec(sql)) return false;
    while (mSQLQuery.next()) {
        ShareExchangeData data;
        data.mID = mSQLQuery.value(HQ_TABLE_COL_ID).toInt();
        data.mCode = mSQLQuery.value(HQ_TABLE_COL_CODE).toString();
        data.mName = mSQLQuery.value(HQ_TABLE_COL_NAME).toString();
        data.mDateTime = mSQLQuery.value(HQ_TABLE_COL_DATE).toString();
        data.mType = mSQLQuery.value(HQ_TABLE_COL_EXCHANGE_TYPE).toInt();
        data.mNum = mSQLQuery.value(HQ_TABLE_COL_EXCHANGE_COUNT).toInt();
        data.mPrice = mSQLQuery.value(HQ_TABLE_COL_EXCHANGE_PRICE).toDouble();
        data.mMoney = mSQLQuery.value(HQ_TABLE_COL_EXCHANGE_TOTAL).toDouble();
        data.mYongjin = mSQLQuery.value(HQ_TABLE_COL_EXCHANGE_YONGJIN).toDouble();
        data.mYinhuasui = mSQLQuery.value(HQ_TABLE_COL_EXCHANGE_YINHUASUI).toDouble();
        data.mOther = mSQLQuery.value(HQ_TABLE_COL_EXCHANGE_OTHER).toDouble();
        data.mNetIncome = mSQLQuery.value(HQ_TABLE_COL_EXCHANGE_NETINCOME).toDouble();
        data.mSerialText = mSQLQuery.value(HQ_TABLE_COL_EXCHANGE_SERIAL_NUM).toString();

        list.append(data);
    }
    return true;
}

bool HQDBDataBase::deleteExchangeRecord(const QString& code, const QString& start_date, const QString& end_date)
{
    DBColValList whereList;
    if(code.length() > 0 )
    {
        whereList.append(DBColVal(HQ_TABLE_COL_CODE, code, HQ_DATA_TEXT));
    }
    if(!start_date.isNull())
    {
        whereList.append(DBColVal(HQ_TABLE_COL_DATE, start_date, HQ_DATA_TEXT, HQ_COMPARE_GREAT_EQUAL));
    }

    if(!end_date.isNull())
    {
        whereList.append(DBColVal(HQ_TABLE_COL_DATE, end_date, HQ_DATA_TEXT, HQ_COMPARE_LESS_EQUAL));
    }
    return deleteRecord(TABLE_SHARE_EXCHANGE_RECORD, whereList);
}



