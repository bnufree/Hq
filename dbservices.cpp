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
    qDebug()<<"init db status:"<<sts;
    initSignalSlot();
    //3、开启异步通讯
    moveToThread(&m_threadWork);
    m_threadWork.start();
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
    if(!mSqlQuery.exec(tr("SELECT COUNT(*) FROM sqlite_master where type='table' and name='%1'").arg(pTable))) return  false;
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
                  "[total_vol] NUMERIC  NULL,"
                  "[total_money] REAL  NULL,"
                  "[zjlx] REAL  NULL,"
                  "[ltsz] REAL  NULL,"
                  "[zsz] REAL  NULL"
                  ")").arg(pTableName);
    return mSqlQuery.exec(sql);
}

void HqInfoService::initSignalSlot()
{

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

void HqInfoService::slotRecvTop10ChinaStockInfos(const QString &date)
{
    //先从数据库去查询，如果没有，再到网上更新

}

bool HqInfoService::queryTop10ChinaShareInfos(QList<BlockRealInfo> &list, const QString& date)
{
//    QString filter = (date.length() != 0 ? tr(" where type = %1").arg(date) : "");
//    if(!mSqlQuery.exec(tr("select * from Block %1").arg(filter))) return;

//    QList<BlockRealInfo> selist;
//    while (mSqlQuery.next()) {
//        BlockRealInfo info;
//        int index = 0;
//        info.mCode = mSqlQuery.value(index++).toInt();
//        info.mName = mSqlQuery.value(index++).toString();
//        info.mCurPrice = mSqlQuery.value(index++).toDouble();
//        info.mChange = mSqlQuery.value(index++).toDouble();
//        info.mChangePercent = mSqlQuery.value(index++).toDouble();
//        info.mZjlx = mSqlQuery.value(index++).toDouble();
//        info.mShareCodesList = mSqlQuery.value(index++).toStringList();
//        info.mType = mSqlQuery.value(index++).toInt();
//        info.mDate = QDateTime::fromMSecsSinceEpoch(mSqlQuery.value(index++).toLongLong()).date();
//        if(init) mBlockInfo[info.mCode] = info;
//        selist.append(info);
//    }

//    emit signalSendBlockInfoList(selist);

}
