#include "hqdbdatabase.h"
#include <QMutexLocker>
#include <QDebug>
#include <QFile>
#include "utils/comdatadefines.h"

#define     QDebug()                    qDebug()<<__FUNCTION__<<__LINE__
#define     HISTORY_TABLE(code)         QString(TABLE_SHARE_HISTORY_TEMPLATE).arg(code.right(6))

#define     DB_FILE                     QString("%1db.data").arg(HQ_DATA_DIR)
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
        } else
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
        } else
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
            } else
            {
                valStrlist.append(QString("%1 like '%2'").arg(data.mColName).arg(data.mColVal.mValue.toString()));
            }
        } else if(data.mColVal.mType == HQ_DATA_INT)
        {
            if(data.mColCompare == HQ_COMPARE_EQUAL)
            {
                valStrlist.append(QString("%1 = %2").arg(data.mColName).arg(data.mColVal.mValue.toInt()));
            } else if(data.mColCompare == HQ_COMPARE_GREAT)
            {
                valStrlist.append(QString("%1 > %2").arg(data.mColName).arg(data.mColVal.mValue.toInt()));
            } else
            {
                valStrlist.append(QString("%1 < %2").arg(data.mColName).arg(data.mColVal.mValue.toInt()));
            }
        } else
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

bool HQDBDataBase::isTableExist(const QString &pTable)
{
    QMutexLocker locker(&mSQLMutex);
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
    QMutexLocker locker(&mSQLMutex);
    return mSQLQuery.exec(sql);
}

bool HQDBDataBase::getBlockDataList(QMap<QString, BlockData*>& pBlockMap, int type)
{
    QMutexLocker locker(&mSQLMutex);
    QString filter = (type != 0 ? tr(" where %1 = %2").arg(HQ_TABLE_COL_BLOCK_TYPE).arg(type) : "");
    if(!mSQLQuery.exec(tr("select * from %1 %2").arg(TABLE_BLOCK).arg(filter))) return false;
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
    QMutexLocker locker(&mSQLMutex);
    //先检查板块名是否存在
    if(!mSQLQuery.exec(tr("select 1 from %1 where %2 = '%3'").arg(TABLE_BLOCK).arg(HQ_TABLE_COL_CODE).arg(code))) return false;
    while (mSQLQuery.next()) {
        return true;
    }
    return false;
}

bool HQDBDataBase::addBlock(const BlockData &info)
{
    QMutexLocker locker(&mSQLMutex);
    mSQLQuery.prepare(QString("insert into %1 values ("
                      "?, ?, ?, ?, ?, ?, ?)").arg(TABLE_BLOCK));
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
    QMutexLocker locker(&mSQLMutex);
    mSQLQuery.prepare(QString("update %1 set "
                      " %2 = ?, %3 = ?, %4 = ?, %5 = ? "
                      " where %6 = ? ")
                      .arg(TABLE_BLOCK)
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
    return mSQLQuery.exec(tr("delete from %1 where %2 = %1").arg(TABLE_BLOCK).arg(HQ_TABLE_COL_CODE).arg(code));
}

bool HQDBDataBase::createDBTables()
{
    qDebug()<<__FUNCTION__<<__LINE__;
    if(!createBlockTable()) return false;
    if(!createShareBasicTable()) return false;
    if(!createDBUpdateDateTable()) return false;
    qDebug()<<__FUNCTION__<<__LINE__;
    return true;
}

bool HQDBDataBase::createBlockTable()
{
    if(isTableExist(TABLE_BLOCK)) return true;
    TableColList colist;
    colist.append({HQ_TABLE_COL_ID, "INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL"});
    colist.append({HQ_TABLE_COL_CODE, "VARCHAR(10) NOT NULL"});
    colist.append({HQ_TABLE_COL_NAME, "VARCHAR(100) NOT NULL"});
    colist.append({HQ_TABLE_COL_CLOSE, "NUMERIC NULL"});
    colist.append({HQ_TABLE_COL_CHANGE_PERCENT, "NUMERIC NULL"});
    colist.append({HQ_TABLE_COL_SHARE_LIST, "VARCHAR(10000) NULL"});
    colist.append({HQ_TABLE_COL_BLOCK_TYPE, "INTEGER NULL"});
    colist.append({HQ_TABLE_COL_DATE, "DATE NULL"});
    return createTable(TABLE_BLOCK, colist);
}

//基本信息更新
bool HQDBDataBase::createShareBasicTable()
{
    if(isTableExist(TABLE_SHARE_BASIC_INFO)) return true;
    TableColList colist;
    colist.append({HQ_TABLE_COL_ID, "INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL"});
    colist.append({HQ_TABLE_COL_CODE, "VARCHAR(10) NOT NULL"});
    colist.append({HQ_TABLE_COL_NAME, "VARCHAR(100) NULL"});
    colist.append({HQ_TABLE_COL_PY_ABBR, "VARCHAR(10) NULL"});
    colist.append({HQ_TABLE_COL_BLOCK_LIST, "VARCHAR(10000) NULL"});
    colist.append({HQ_TABLE_COL_FAVORITE, "BOOL NULL"});
    colist.append({HQ_TABLE_COL_HSGT_TOP10, "BOOL NULL"});
    colist.append({HQ_TABLE_COL_PROFIT, "NUMERIC NULL"});
    return createTable(TABLE_SHARE_BASIC_INFO, colist);
}

bool HQDBDataBase::updateShareBasicInfo(const ShareDataList& dataList, int mode)
{
    int size = dataList.size();
    if(size == 0) return false;
    mDB.transaction();
    foreach (ShareData data, dataList) {
        DBColValList list;
        list.append(DBColVal(HQ_TABLE_COL_CODE, data.mCode, HQ_DATA_TEXT));
        if(mode & Share_Basic_Update_Code)
        {
            list.append(DBColVal(HQ_TABLE_COL_NAME, data.mName, HQ_DATA_TEXT));
            list.append(DBColVal(HQ_TABLE_COL_PY_ABBR, data.mPY, HQ_DATA_TEXT));
        }
        if(mode & Share_Basic_Update_Block)
        {
            list.append(DBColVal(HQ_TABLE_COL_BLOCK_LIST, data.mBlockCodeList.join(","), HQ_DATA_TEXT));
        }
        if(mode & Share_Basic_Update_Fav)
        {
            list.append(DBColVal(HQ_TABLE_COL_FAVORITE, data.mIsFav, HQ_DATA_INT));
        }
        if(mode & Share_Basic_Update_Hsgt)
        {
            list.append(DBColVal(HQ_TABLE_COL_HSGT_TOP10, data.mIsHsTop10, HQ_DATA_INT));
        }
        if(mode & Share_Basic_Update_Profit)
        {
            list.append(DBColVal(HQ_TABLE_COL_PROFIT, data.mProfit, HQ_DATA_INT));
        }
        if(updateTable(TABLE_SHARE_BASIC_INFO, list, list[0])){
            mDB.rollback();
        }
    }
    if(!updateDBUpdateDate(ShareDate::currentDate(), TABLE_SHARE_BASIC_INFO))
    {
        mDB.rollback();
    }
    mDB.commit();
}


bool HQDBDataBase::queryShareBasicInfo(ShareDataList& list)
{
    QMutexLocker locker(&mSQLMutex);
    if(!mSQLQuery.exec(tr("select * from %1").arg(TABLE_SHARE_BASIC_INFO))) return false;
    while (mSQLQuery.next()) {
        ShareData info;
        info.setCode(mSQLQuery.value(HQ_TABLE_COL_CODE).toString());
        info.setName(mSQLQuery.value(HQ_TABLE_COL_NAME).toString());
        info.setPY(mSQLQuery.value(HQ_TABLE_COL_PY_ABBR).toString());
        info.mIsFav = mSQLQuery.value(HQ_TABLE_COL_FAVORITE).toBool();
        info.mHKExInfo.mIsTop10= mSQLQuery.value(HQ_TABLE_COL_HSGT_TOP10).toBool();
        info.mProfit = mSQLQuery.value(HQ_TABLE_COL_PROFIT).toDouble();
        list.append(info);
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
     }
     if(!updateDBUpdateDate(ShareDate::currentDate(), TABLE_SHARE_BASIC_INFO))
     {
         mDB.rollback();
     }
     mDB.commit();
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

    return true;
}


bool HQDBDataBase::createShareHistoryInfoTable(const QString& code)
{
    if(code.length() == 0) return false;
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
    colist.append({HQ_TABLE_COL_HSGT_HAVE, "NUMERIC NULL"});
    colist.append({HQ_TABLE_COL_HSGT_HAVE_PERCENT, "NUMERIC NULL"});
    colist.append({HQ_TABLE_COL_HSGT_TOP10_MONEY, "NUMERIC NULL"});
    colist.append({HQ_TABLE_COL_HSGT_TOP10, "BOOL NULL"});
    colist.append({HQ_TABLE_COL_TOTALMNT, "NUMERIC NULL"});
    colist.append({HQ_TABLE_COL_MUTAL, "NUMERIC NULL"});
    colist.append({HQ_TABLE_COL_DATE, "DATE NULL"});
    return createTable(HISTORY_TABLE(code), colist);
}

bool HQDBDataBase::updateShareHistory(const ShareDataList &dataList, int mode)
{
    if(dataList.size() == 0) return true;
    mDB.transaction();
    foreach (ShareData data, dataList) {
        QString table = HISTORY_TABLE(data.mCode);
        if(!createShareHistoryInfoTable(table)) continue;
        ShareDate date(QDateTime::fromMSecsSinceEpoch(data.mTime).date());
        DBColValList list;
        list.append(DBColVal(HQ_TABLE_COL_CODE, data.mCode, HQ_DATA_TEXT));
        list.append(DBColVal(HQ_TABLE_COL_DATE, date.toTime_t(), HQ_DATA_INT));
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
            list.append(DBColVal(HQ_TABLE_COL_HSGT_HAVE, data.mHKExInfo.mForeignVol, HQ_DATA_DOUBLE));
            list.append(DBColVal(HQ_TABLE_COL_HSGT_HAVE_PERCENT, data.mHKExInfo.mTotalPercent, HQ_DATA_DOUBLE));
        }
        if(mode & History_HsgtTop10)
        {
            list.append(DBColVal(HQ_TABLE_COL_HSGT_TOP10_MONEY, data.mHKExInfo.mPureMoney, HQ_DATA_DOUBLE));
            list.append(DBColVal(HQ_TABLE_COL_HSGT_TOP10, true, HQ_DATA_INT));
        }
        if(mode & History_Close)
        {
            list.append(DBColVal(HQ_TABLE_COL_CLOSE, data.mClose, HQ_DATA_DOUBLE));
            list.append(DBColVal(HQ_TABLE_COL_CHANGE_PERCENT, data.mChgPercent, HQ_DATA_DOUBLE));
            list.append(DBColVal(HQ_TABLE_COL_VOL, data.mVol, HQ_DATA_DOUBLE));
            list.append(DBColVal(HQ_TABLE_COL_MONEY, data.mMoney, HQ_DATA_DOUBLE));
            list.append(DBColVal(HQ_TABLE_COL_TOTALMNT, data.mFinanceInfo.mTotalShare, HQ_DATA_DOUBLE));
            list.append(DBColVal(HQ_TABLE_COL_MUTAL, data.mFinanceInfo.mMutalShare, HQ_DATA_DOUBLE));
        }
        DBColValList key;
        key.append(list[0]);
        key.append(list[1]);
        if(updateTable(table, list, key)){
            mDB.rollback();
        }

    }
    mDB.commit();

}

bool HQDBDataBase::queryShareHistory(ShareDataList &list, const QString &share_code, const ShareDate &start, const ShareDate &end)
{
    if(share_code.length() == 0) return false;
    DBColValList wherelist;
    if(!start.isNull())
    {
        wherelist.append(DBColVal(HQ_TABLE_COL_DATE, start.toTime_t(), HQ_DATA_INT, HQ_COMPARE_GREAT));
    }
    if(share_code.length() > 0)
    {
        wherelist.append(DBColVal(HQ_TABLE_COL_CODE, share_code, HQ_DATA_TEXT));
    }
    if(!end.isNull())
    {
        wherelist.append(DBColVal(HQ_TABLE_COL_DATE, end.toTime_t(), HQ_DATA_INT, HQ_COMPARE_LESS));
    }
    QString sql = QString("select * from %1 %2").arg(HISTORY_TABLE(share_code)).arg(wherelist.whereString());
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
        data.mHKExInfo.mForeignVol = mSQLQuery.value(HQ_TABLE_COL_HSGT_HAVE).toDouble();
        data.mHKExInfo.mTotalPercent = mSQLQuery.value(HQ_TABLE_COL_HSGT_HAVE_PERCENT).toDouble();
        data.mHKExInfo.mPureMoney = mSQLQuery.value(HQ_TABLE_COL_HSGT_TOP10_MONEY).toDouble();
        data.mHKExInfo.mIsTop10 = mSQLQuery.value(HQ_TABLE_COL_HSGT_TOP10).toDouble();
        data.mFinanceInfo.mMutalShare = mSQLQuery.value(HQ_TABLE_COL_MUTAL).toDouble();
        data.mFinanceInfo.mTotalShare = mSQLQuery.value(HQ_TABLE_COL_TOTALMNT).toDouble();
        data.mTime = ShareDate(mSQLQuery.value(HQ_TABLE_COL_DATE).toInt()).mSecs();
        list.append(data);
    }
    return true;
}

bool HQDBDataBase::delShareHistory(const QString &share_code, const ShareDate &start, const ShareDate &end)
{
    if(share_code.length() == 0) return false;
    DBColValList wherelist;
    if(!start.isNull())
    {
        wherelist.append(DBColVal(HQ_TABLE_COL_DATE, start.toTime_t(), HQ_DATA_INT, HQ_COMPARE_GREAT));
    }
    if(!end.isNull())
    {
        wherelist.append(DBColVal(HQ_TABLE_COL_DATE, end.toTime_t(), HQ_DATA_INT, HQ_COMPARE_LESS));
    }
    return deleteRecord(HISTORY_TABLE(share_code), wherelist);
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

//shareholders
bool HQDBDataBase::createShareHoldersTable()
{
    if(isTableExist(TABLE_SHARE_HOLEDER)) return true;
    TableColList colist;
    colist.append({HQ_TABLE_COL_ID, "INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL"});
    colist.append({HQ_TABLE_COL_SHARE_CODE, "VARCHAR(6) NULL"});
    colist.append({HQ_TABLE_COL_HOLDER_CODE, "VARCHAR(10) NULL"});
    colist.append({HQ_TABLE_COL_HOLDER_NAME, "VARCHAR(100) NULL"});
    colist.append({HQ_TABLE_COL_HOLDER_VOL, "DOUBLE NULL"});
    colist.append({HQ_TABLE_COL_HOLDER_FUND_PERCENT, "DOUBLE NULL"});
    colist.append({HQ_TABLE_COL_HOLDER_DATE, "INTEGER NULL"});
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
        list.append(DBColVal(HQ_TABLE_COL_HOLDER_DATE, data.mDate.toTime_t(), HQ_DATA_INT));
        list.append(DBColVal(HQ_TABLE_COL_HOLDER_FUND_PERCENT, data.mFundPercent, HQ_DATA_DOUBLE));

        DBColValList key;
        key.append(list[0]);
        key.append(list[1]);
        key.append(list[4]);
        if(updateTable(TABLE_SHARE_HOLEDER, list, key)){
            mDB.rollback();
        }
    }
    mDB.commit();
}

bool HQDBDataBase::queryShareHolder(ShareHolderList &list, const QString& share_code, const QString& holder_code, const ShareDate &date)
{    
    DBColValList wherelist;
    if(!date.isNull())
    {
        wherelist.append(DBColVal(HQ_TABLE_COL_HOLDER_DATE, date.toTime_t(), HQ_DATA_INT));
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
        data.mDate = ShareDate::fromTime_t(mSQLQuery.value(HQ_TABLE_COL_HOLDER_DATE).toInt());
        data.mFundPercent = mSQLQuery.value(HQ_TABLE_COL_HOLDER_FUND_PERCENT).toDouble();
        list.append(data);
    }
    return true;
}

bool HQDBDataBase::delShareHolder(const QString& share_code, const QString& holder_code, const ShareDate &date)
{
    DBColValList wherelist;
    if(!date.isNull())
    {
        wherelist.append(DBColVal(HQ_TABLE_COL_HOLDER_DATE, date.toTime_t(), HQ_DATA_INT));
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
    if(isTableExist(TABLE_SHARE_FINANCE)) return true;
    TableColList colist;
    colist.append({HQ_TABLE_COL_ID, "INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL"});
    colist.append({HQ_TABLE_COL_CODE, "VARCHAR(6) NULL"});
    colist.append({HQ_TABLE_COL_FINANCE_MGSY, "DOUBLE NULL"});
    colist.append({HQ_TABLE_COL_FINANCE_JZC, "DOUBLE NULL"});
    colist.append({HQ_TABLE_COL_FINANCE_JZCSYL, "DOUBLE NULL"});
    colist.append({HQ_TABLE_COL_TOTALMNT, "double NULL"});
    colist.append({HQ_TABLE_COL_MUTAL, "double NULL"});
    return createTable(TABLE_SHARE_FINANCE, colist);
}

bool HQDBDataBase::updateShareFinance(const FinancialDataList& dataList)
{
    int size = dataList.size();
    if(size == 0) return false;
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
        if(updateTable(TABLE_SHARE_BONUS, list, list[0])){
            mDB.rollback();
        }
    }
    mDB.commit();
}

bool HQDBDataBase::queryShareFinance(FinancialDataList& list, const QString& code)
{
    DBColValList where;
    if(code.length() > 0)
    {
        where.append(DBColVal(HQ_TABLE_COL_CODE, code, HQ_DATA_TEXT));
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
    if(isTableExist(TABLE_SHARE_BONUS)) return true;
    TableColList colist;
    colist.append({HQ_TABLE_COL_ID, "INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL"});
    colist.append({HQ_TABLE_COL_CODE, "VARCHAR(6) NULL"});
    colist.append({HQ_TABLE_COL_BONUS_SHARE, "DOUBLE NULL"});
    colist.append({HQ_TABLE_COL_BONUS_MONEY, "DOUBLE NULL"});
    colist.append({HQ_TABLE_COL_BONUS_YAGGR, "INTEGER NULL"});
    colist.append({HQ_TABLE_COL_BONUS_GQDJR, "INTEGER NULL"});
    colist.append({HQ_TABLE_COL_BONUS_DATE, "INTEGER NULL"});
    return createTable(TABLE_SHARE_BONUS, colist);
}

bool HQDBDataBase::updateShareBonus(QList<ShareBonus>& bonusList)
{
    int size = bonusList.size();
    if(size == 0) return false;
    mDB.transaction();
    for(int i=0; i<size; i++)
    {
        ShareBonus bonus = bonusList[i];
        DBColValList list;
        list.append(DBColVal(HQ_TABLE_COL_CODE, bonus.mCode, HQ_DATA_TEXT));
        list.append(DBColVal(HQ_TABLE_COL_BONUS_SHARE, bonus.mSZZG, HQ_DATA_DOUBLE));
        list.append(DBColVal(HQ_TABLE_COL_BONUS_MONEY, bonus.mXJFH, HQ_DATA_DOUBLE));
        list.append(DBColVal(HQ_TABLE_COL_BONUS_YAGGR, bonus.mYAGGR.toTime_t(), HQ_DATA_INT));
        list.append(DBColVal(HQ_TABLE_COL_BONUS_GQDJR, bonus.mGQDJR.toTime_t(), HQ_DATA_INT));
        list.append(DBColVal(HQ_TABLE_COL_BONUS_DATE, bonus.mDate.toTime_t(), HQ_DATA_INT));
        if(updateTable(TABLE_SHARE_BONUS, list, list[0])){
            mDB.rollback();
        }
    }
    mDB.commit();
}

bool HQDBDataBase::queryShareBonus(QList<ShareBonus>& list, const QString& code, const ShareDate& date)
{
    QString sql = QString("select * from %1").arg(TABLE_SHARE_BONUS);
    if(code.length() > 0 || !(date.isNull())){
        sql.append(SQL_WHERE);
        if(code.length() > 0) {
            sql.append(QString("%1 = '%2'").arg(HQ_TABLE_COL_CODE).arg(code));
        }
        if(!date.isNull()) {
            sql.append(QString("%1 = %2").arg(HQ_TABLE_COL_BONUS_DATE).arg(date.toTime_t()));
        }
    }
    QMutexLocker locker(&mSQLMutex);
    if(!mSQLQuery.exec(sql)) return false;
    while (mSQLQuery.next()) {
        ShareBonus bonus;
        bonus.mCode = mSQLQuery.value(HQ_TABLE_COL_CODE).toString();
        bonus.mDate = ShareDate::fromTime_t(mSQLQuery.value(HQ_TABLE_COL_BONUS_DATE).toInt());
        bonus.mGQDJR = ShareDate::fromTime_t(mSQLQuery.value(HQ_TABLE_COL_BONUS_GQDJR).toInt());
        bonus.mSZZG = mSQLQuery.value(HQ_TABLE_COL_BONUS_SHARE).toDouble();
        bonus.mXJFH = mSQLQuery.value(HQ_TABLE_COL_BONUS_MONEY).toDouble();
        bonus.mYAGGR = ShareDate::fromTime_t(mSQLQuery.value(HQ_TABLE_COL_BONUS_YAGGR).toInt());
        list.append(bonus);
    }
    return true;
}

bool HQDBDataBase::delShareBonus(const QString& code, const ShareDate& date)
{
    DBColValList list;
    if(code.length() > 0)
    {
        list.append(DBColVal(HQ_TABLE_COL_CODE, code, HQ_DATA_TEXT));
    }
    if(!date.isNull())
    {
        list.append(DBColVal(HQ_TABLE_COL_BONUS_DATE, date.toTime_t(),HQ_DATA_INT));
    }

    return deleteRecord(TABLE_SHARE_BONUS, list);
}


//陆股通TOP10
bool HQDBDataBase::createShareHsgTop10Table()
{
    if(isTableExist(TABLE_HSGT_TOP10)) return true;
    TableColList colist;
    colist.append({HQ_TABLE_COL_ID, "INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL"});
    colist.append({HQ_TABLE_COL_CODE, "VARCHAR(6) NULL"});
    colist.append({HQ_TABLE_COL_HSGT_TOP10_BUY, "DOUBLE NULL"});
    colist.append({HQ_TABLE_COL_HSGT_TOP10_SELL, "DOUBLE NULL"});
    colist.append({HQ_TABLE_COL_HSGT_TOP10_MONEY, "DOUBLE NULL"});
    colist.append({HQ_TABLE_COL_DATE, "INTEGER NULL"});
    return createTable(TABLE_HSGT_TOP10, colist);
}

bool HQDBDataBase::updateShareHsgtTop10List(const ShareHsgtList& dataList)
{
    int size = dataList.size();
    if(size == 0) return false;
    mDB.transaction();
    foreach(ShareHsgt data, dataList)
    {
        DBColValList list;
        list.append(DBColVal(HQ_TABLE_COL_CODE, data.mCode, HQ_DATA_TEXT));
        list.append(DBColVal(HQ_TABLE_COL_DATE, data.mDate.toTime_t(), HQ_DATA_INT));
        list.append(DBColVal(HQ_TABLE_COL_HSGT_TOP10_BUY, data.mBuyMoney, HQ_DATA_DOUBLE));
        list.append(DBColVal(HQ_TABLE_COL_HSGT_TOP10_SELL, data.mSellMoney, HQ_DATA_DOUBLE));
        list.append(DBColVal(HQ_TABLE_COL_HSGT_TOP10_MONEY, data.mPureMoney, HQ_DATA_DOUBLE));
        DBColValList key;
        key.append(list[0]);
        key.append(list[1]);
        if(updateTable(TABLE_HSGT_TOP10, list, key)){
            mDB.rollback();
        }
    }
    mDB.commit();
}

bool HQDBDataBase::queryShareHsgtTop10List(ShareHsgtList& list, const QString& code, const ShareDate& date)
{
    DBColValList whereList;
    if(code.length() > 0 )
    {
        whereList.append(DBColVal(HQ_TABLE_COL_CODE, code, HQ_DATA_TEXT));
    }
    if(!date.isNull())
    {
        whereList.append(DBColVal(HQ_TABLE_COL_DATE, date.toTime_t(), HQ_DATA_INT));
    }
    QString sql = QString("select * from %1 %2").arg(TABLE_HSGT_TOP10).arg(whereList.whereString());
    QMutexLocker locker(&mSQLMutex);
    if(!mSQLQuery.exec(sql)) return false;
    while (mSQLQuery.next()) {
        ShareHsgt data;
        bonus.mCode = mSQLQuery.value(HQ_TABLE_COL_CODE).toString();
        bonus.mDate = ShareDate::fromTime_t(mSQLQuery.value(HQ_TABLE_COL_BONUS_DATE).toInt());
        bonus.mGQDJR = ShareDate::fromTime_t(mSQLQuery.value(HQ_TABLE_COL_BONUS_GQDJR).toInt());
        bonus.mSZZG = mSQLQuery.value(HQ_TABLE_COL_BONUS_SHARE).toDouble();
        bonus.mXJFH = mSQLQuery.value(HQ_TABLE_COL_BONUS_MONEY).toDouble();
        bonus.mYAGGR = ShareDate::fromTime_t(mSQLQuery.value(HQ_TABLE_COL_BONUS_YAGGR).toInt());
        list.append(bonus);
    }
    return true;
}

bool HQDBDataBase::delShareHsgtTop10(const QString& code, const ShareDate& date)
{
    DBColValList list;
    if(code.length() > 0)
    {
        list.append(DBColVal(HQ_TABLE_COL_CODE, code, HQ_DATA_TEXT));
    }
    if(!date.isNull())
    {
        list.append(DBColVal(HQ_TABLE_COL_BONUS_DATE, date.toTime_t(),HQ_DATA_INT));
    }

    return deleteRecord(TABLE_SHARE_BONUS, list);
}

//数据表更新日期的相关操作
bool HQDBDataBase::createDBUpdateDateTable()
{
    if(isTableExist(TABLE_DB_UPDATE)) return true;
    TableColList colist;
    colist.append({HQ_TABLE_COL_ID, "INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL"});
    colist.append({HQ_TABLE_COL_TABLE_NM, "VARCHAR(100) NULL"});
    colist.append({HQ_TABLE_COL_DATE, "INTEGER  NOT NULL"});
    return createTable(TABLE_DB_UPDATE, colist);
}

bool HQDBDataBase::queryDBUpdateDate(ShareDate &date, const QString &table)
{
    QMutexLocker locker(&mSQLMutex);
    QString sql = QString("select %1 from %2 where %3 = '%4' ").arg(HQ_TABLE_COL_DATE).arg(TABLE_DB_UPDATE).arg(HQ_TABLE_COL_TABLE_NM).arg(table);
    if(!mSQLQuery.exec(sql)) return false;
    while (mSQLQuery.next()) {
        date = ShareDate::fromTime_t(mSQLQuery.value(0).toInt());
        break;
    }
    return true;
}

bool HQDBDataBase::updateDBUpdateDate(const ShareDate& date, const QString& table)
{
    DBColValList list;
    list.append(DBColVal(HQ_TABLE_COL_TABLE_NM, table, HQ_DATA_TEXT));
    list.append(DBColVal(HQ_TABLE_COL_DATE, date.toTime_t(), HQ_DATA_INT));
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

bool HQDBDataBase::updateTable(const QString& tableName, const DBColValList& values, const DBColValList& key )
{
    //检查记录已经添加
    bool exist = false;
    if(!isRecordExist(exist, tableName, key)) return false;
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
    if(!mSQLQuery.exec(QString("delete from %1 %2").arg(table).arg(list.whereString()))) return false;
    return true;
}

bool HQDBDataBase::isRecordExist(bool& exist, const QString& table, const DBColValList& list)
{
    exist = false;
    if(list.size() == 0) return true;
    QMutexLocker locker(&mSQLMutex);
    if(!mSQLQuery.exec(QString("select count(1) from %1 %2 ").arg(table).arg(list.whereString()))) return false;
    while (mSQLQuery.next()) {
        exist = true;
        break;
    }
    return true;
}



QString HQDBDataBase::errMsg()
{
    return QString("sql:%1\\nerr:%2").arg(mSQLQuery.lastQuery()).arg(mSQLQuery.lastError().text());
}



