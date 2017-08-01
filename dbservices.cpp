#include <QDebug>
#include <QSharedPointer>
#include <QSqlError>
#include <QDir>
#include "dbservices.h"
#include <QtSql/QSqlRecord>

HqInfoService* HqInfoService::m_pInstance = 0;
HqInfoService::CGarbo HqInfoService::s_Garbo;

HqInfoService::HqInfoService(QObject *parent) :
    QObject(parent),
    mDataBaseInitFlag(0)
{
    qRegisterMetaType<QList<ChinaShareExchange>>("const QList<ChinaShareExchange>&");
    bool sts = initDatabase();
    mSqlQuery = QSqlQuery(mDB);
    qDebug()<<"init db status:"<<sts;
    //3、开启异步通讯
    moveToThread(&m_threadWork);
    m_threadWork.start();    
    initSignalSlot();
}

HqInfoService::~HqInfoService()
{
    if(mDataBaseInitFlag)
    {
        mDB.close();
    }
    m_threadWork.quit();
    m_threadWork.wait(500);
    m_threadWork.terminate();
}

bool HqInfoService::isTableExist(const QString &pTable)
{
    qDebug()<<__FUNCTION__<<__LINE__<<" "<<pTable;
    if(!mSqlQuery.exec(tr("SELECT COUNT(*) FROM sqlite_master where type='table' and name='%1'").arg(pTable))) return  false;
    qDebug()<<__FUNCTION__<<__LINE__;
    while (mSqlQuery.next()) {
        return mSqlQuery.value(0).toBool();
    }
    return false;
}

bool HqInfoService::createHistoryTable(const QString &pTableName)
{
    QString sql = tr("CREATE TABLE [%1] ("
                  "[id] INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL,"
                  "[name] VARCHAR(100)  NULL,"
                  "[close] REAL  NULL,"
                  "[open] REAL  NULL,"
                  "[high] REAL  NULL,"
                  "[low] REAL  NULL,"
                  "[change] REAL  NULL,"
                  "[change_percent] REAL  NULL,"
                  "[vol] NUMERIC  NULL,"
                  "[money] REAL  NULL,"
                  "[puremoney] REAL  NULL,"
                  "[marketshare] REAL  NULL,"
                  "[mutalbleshare] REAL  NULL,"
                  "[date] DATE  NULL"
                  ")").arg(pTableName);
    return mSqlQuery.exec(sql);
}

void HqInfoService::initSignalSlot()
{
    connect(this, SIGNAL(signalRecvTop10ChinaStockInfos(QList<ChinaShareExchange>)),
            this, SLOT(slotRecvTop10ChinaStockInfos(QList<ChinaShareExchange>)));
    connect(this, SIGNAL(signalQueryTop10ChinaStockInfos(QDate,QString,int)),
            this, SLOT(slotQueryTop10ChinaStockInfos(QDate,QString,int)));
    connect(this, SIGNAL(signalRecvShareHistoryInfos(StockDataList)),
            this, SLOT(slotRecvShareHistoryInfos(StockDataList)));
    connect(this, SIGNAL(signalQueryShareHistoryLastDate(QString)),
            this, SLOT(slotQueryShareHistoryLastDate(QString)));
}

void HqInfoService::recvRealBlockInfo(const QList<BlockRealInfo> &list)
{
    foreach (BlockRealInfo info, list) {
        if(!blockExist(info.mCode)) addBlock(info);
        mBlockInfo[info.mCode] = info;
    }
}

void HqInfoService::updateBlockInfoList(const QList<BlockRealInfo> &list)
{
    foreach (BlockRealInfo info, list) {
        modBlock(info);
    }
}

bool HqInfoService::blockExist(int code)
{
    return mBlockInfo.contains(code);
}

void HqInfoService::addBlock(const BlockRealInfo &info)
{
    mSqlQuery.prepare("insert into Block values ("
                      "?, ?, ?, ?, ?, ?, ?, ?, ?)");
    mSqlQuery.addBindValue(info.mCode);
    mSqlQuery.addBindValue(info.mName);
    mSqlQuery.addBindValue(info.mCurPrice);
    mSqlQuery.addBindValue(info.mChange);
    mSqlQuery.addBindValue(info.mChangePercent);
    mSqlQuery.addBindValue(info.mZjlx);
    mSqlQuery.addBindValue(info.mShareCodesList.join(","));
    mSqlQuery.addBindValue(info.mType);
    mSqlQuery.addBindValue(QDateTime::currentMSecsSinceEpoch());

    mSqlQuery.exec();
}

void HqInfoService::modBlock(const BlockRealInfo &info)
{
    mSqlQuery.prepare("update Block set "
                      "price = ?, change = ?, change_percent = ?"
                      "zjlx = ?, codelist = ?, update = ? "
                      "where id = ? ");
    mSqlQuery.addBindValue(info.mCurPrice);
    mSqlQuery.addBindValue(info.mChange);
    mSqlQuery.addBindValue(info.mChangePercent);
    mSqlQuery.addBindValue(info.mZjlx);
    mSqlQuery.addBindValue(info.mShareCodesList.join(","));
    mSqlQuery.addBindValue(QDateTime::currentMSecsSinceEpoch());
    mSqlQuery.addBindValue(info.mCode);

    mSqlQuery.exec();
}

void HqInfoService::delBlock(int code)
{
    mSqlQuery.exec(tr("delete from Block where id = %1").arg(code));
}

void HqInfoService::queryBlock(int type, bool init)
{
    QString filter = (type != 0 ? tr(" where type = %1").arg(type) : "");
    if(!mSqlQuery.exec(tr("select * from Block %1").arg(filter))) return;

    QList<BlockRealInfo> selist;
    while (mSqlQuery.next()) {
        BlockRealInfo info;
        int index = 0;
        info.mCode = mSqlQuery.value(index++).toInt();
        info.mName = mSqlQuery.value(index++).toString();
        info.mCurPrice = mSqlQuery.value(index++).toDouble();
        info.mChange = mSqlQuery.value(index++).toDouble();
        info.mChangePercent = mSqlQuery.value(index++).toDouble();
        info.mZjlx = mSqlQuery.value(index++).toDouble();
        info.mShareCodesList = mSqlQuery.value(index++).toStringList();
        info.mType = mSqlQuery.value(index++).toInt();
        info.mDate = QDateTime::fromMSecsSinceEpoch(mSqlQuery.value(index++).toLongLong()).date();
        if(init) mBlockInfo[info.mCode] = info;
        selist.append(info);
    }

    emit signalSendBlockInfoList(selist);
}

void HqInfoService::initBlockInfo()
{
    queryBlock(0, true);
}


HqInfoService *HqInfoService::instance()
{
    if(m_pInstance == 0)
    {
        m_pInstance = new HqInfoService();
    }
    return m_pInstance;
}


bool HqInfoService::initDatabase()
{
    if(mDataBaseInitFlag) return true;
    //初始化本地数据库的连接
    mDB = QSqlDatabase::addDatabase("QSQLITE");//链接数据库
    mDB.setDatabaseName("StockData.s3db");
    return mDB.open();
}

void HqInfoService::saveDB()
{
    updateBlockInfoList(mBlockInfo.values());
}

bool HqInfoService::isActive()
{
    QDateTime cur = QDateTime::currentDateTime();
    if(cur.time().hour() >= 15 || cur.time().hour() <9 ) return false;
    return true;
}

void HqInfoService::slotRecvTop10ChinaStockInfos(const QList<ChinaShareExchange>& list)
{
    //更新到数据库
    QSqlDatabase::database().transaction();
    foreach (ChinaShareExchange info, list) {
        if(!addTop10ChinaStockInfo(info))
        {
            qDebug()<<"error:"<<mSqlQuery.lastError().text();
        }
    }
    QSqlDatabase::database().commit();
}

bool HqInfoService::addTop10ChinaStockInfo(const ChinaShareExchange &info)
{
    mSqlQuery.prepare("insert into hstop10 (id, name, close, change_percent, buy, sell, date) values ("
                      "?, ?, ?, ?, ?, ?, ?)");
    mSqlQuery.addBindValue(info.code);
    mSqlQuery.addBindValue(info.name);
    mSqlQuery.addBindValue(info.cur);
    mSqlQuery.addBindValue(info.per);
    mSqlQuery.addBindValue(info.mTop10Buy);
    mSqlQuery.addBindValue(info.mTop10Sell);
    mSqlQuery.addBindValue(info.mDate);

    return mSqlQuery.exec();
}

QDate HqInfoService::getLastUpdateDateOfTable(const QString &table)
{
    QDate date = QDate(2016, 12, 4);
    if(mSqlQuery.exec(tr("select max(date) from %1").arg(table)))
    {
        while (mSqlQuery.next()) {
            if(!mSqlQuery.value(0).isNull())
            {
                date = mSqlQuery.value(0).toDate();
                break;
            }
        }
    }
    return date;
}

QDate HqInfoService::getLastUpdateDateOfHSGT()
{
    return getLastUpdateDateOfTable("hstop10");
}

QDate HqInfoService::getLastUpdateDateOfShareHistory(const QString &code)
{
    QString table = "stk"+ code.right(6);
    if(!isTableExist(table)) createHistoryTable(table);

    return getLastUpdateDateOfTable(table);
}

void HqInfoService::slotQueryTop10ChinaStockInfos(const QDate &date, const QString &share, int market)
{
    QList<ChinaShareExchange> list;
    queryTop10ChinaShareInfos(list,date, share, market);
    emit signalSendTop10ChinaStockInfos(list);
}

bool HqInfoService::queryTop10ChinaShareInfos(QList<ChinaShareExchange>& list, const QDate& date, const QString& share, int market)
{
    QStringList filterList;
    if((!date.isNull()) && date.isValid())
    {
        filterList.append(tr(" date = '%1' ").arg(date.toString("yyyy-MM-dd")));
    }
    if(!share.isEmpty())
    {
        QRegExp reg("[0-9]{1,6}");
        if(reg.exactMatch(share))
        {
            filterList.append(tr(" id like '%%1%' ").arg(share));
        } else
        {
            filterList.append(tr(" name like '%%1%' ").arg(share));
        }
    }
    if(!mSqlQuery.exec(tr("select * from hstop10 %1 order by date desc").arg(filterList.length() > 0 ? " where " + filterList.join(" and ") : ""))) return false;
    qDebug()<<mSqlQuery.lastQuery();
    while (mSqlQuery.next()) {
        ChinaShareExchange info;
        info.code = mSqlQuery.value("id").toString();
        info.name = mSqlQuery.value("name").toString();
        info.cur = mSqlQuery.value("close").toDouble();
        info.per = mSqlQuery.value("change_percent").toDouble();
        info.mTop10Buy = mSqlQuery.value("buy").toDouble();
        info.mTop10Sell = mSqlQuery.value("sell").toDouble();
        info.mDate = mSqlQuery.value("date").toDate();
        list.append(info);
    }

    return true;
}

void HqInfoService::slotRecvShareHistoryInfos(const StockDataList &list)
{
    //更新到数据库
    QSqlDatabase::database().transaction();
    foreach (StockData info, list) {
        if(!slotAddHistoryData(info))
        {
            qDebug()<<"error:"<<mSqlQuery.lastError().text()<<" "<<mSqlQuery.lastQuery();
        }
    }
    QSqlDatabase::database().commit();
}

bool HqInfoService::slotAddHistoryData(const StockData &info)
{
    QString tableName = "stk" + info.code.right(6);
//    if(!isTableExist(tableName))
//    {
//        if(!createHistoryTable(tableName)) return false;
//    }
    mSqlQuery.prepare(tr("insert into %1 ("
                         "name, close, open, high, low, "
                         "change, change_percent, vol, money, puremoney, "
                         "marketshare, mutalbleshare, date) values ("
                         "?, ?, ?, ?, ?, "
                         "?, ?, ?, ?, ?, "
                         "?, ?, ?)"
                         ).arg(tableName));
    mSqlQuery.addBindValue(info.name);
    mSqlQuery.addBindValue(info.cur);
    mSqlQuery.addBindValue(info.open);
    mSqlQuery.addBindValue(info.high);
    mSqlQuery.addBindValue(info.low);

    mSqlQuery.addBindValue(info.chg);
    mSqlQuery.addBindValue(info.per);
    mSqlQuery.addBindValue(info.vol);
    mSqlQuery.addBindValue(info.money);
    mSqlQuery.addBindValue(info.zjlx);

    mSqlQuery.addBindValue(info.totalshare);
    mSqlQuery.addBindValue(info.mutableshare);
    mSqlQuery.addBindValue(info.date);
    return mSqlQuery.exec();
}

void HqInfoService::slotQueryShareHistoryLastDate(const QString &code)
{
    emit signalSendShareHistoryLastDate(code, getLastUpdateDateOfShareHistory(code));
}
