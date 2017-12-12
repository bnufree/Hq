#include <QDebug>
#include <QSharedPointer>
#include <QSqlError>
#include <QDir>
#include "dbservices.h"
#include "qexchangedatamanage.h"
#include <QMap>

#define     HSGT_TABLE          "hsgvol"
HqInfoService* HqInfoService::m_pInstance = 0;
HqInfoService::CGarbo HqInfoService::s_Garbo;
QMutex HqInfoService::mutex;
HqInfoService::HqInfoService(QObject *parent) :
    QObject(parent)
{
    qRegisterMetaType<QList<ChinaShareExchange>>("const QList<ChinaShareExchange>&");
    qRegisterMetaType<StockDataList>("const StockDataList&");    
    initSignalSlot();
    initHistoryDates();
    //3、开启异步通讯
    moveToThread(&m_threadWork);
    m_threadWork.start();
}


HqInfoService::~HqInfoService()
{
    m_threadWork.quit();
    m_threadWork.wait(500);
    m_threadWork.terminate();
}

bool HqInfoService::isDBInitOk()
{
    return mDataBase.isDBOK();
}

void HqInfoService::slotInitDBTables()
{
    if(mDataBase.createDBTables())
    {
        initBlockData();
        initShareData();
    }
}


bool HqInfoService::createHSGTShareAmountTable()
{
    return true;
#if 0
    QString sql = tr("CREATE TABLE [%1] ("
                  "[id] INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL,"
                  "[code] VARCHAR(100)  NULL,"
                  "[vol] NUMERIC  NULL,"
                  "[date] DATE  NULL"
                  ")").arg(HSGT_TABLE);
    return mSqlQuery.exec(sql);
#endif
}

bool HqInfoService::createHistoryTable(const QString &pTableName)
{
    return true;
#if 0
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
#endif
}

void HqInfoService::initHistoryDates()
{
    mLastActiveDate = QExchangeDataManage::instance()->GetLatestActiveDay(QDate::currentDate().addDays(-1));
    mLast3DaysDate = QExchangeDataManage::instance()->GetLatestActiveDay(QDate::currentDate().addDays(-4));
    mLast5DaysDate = QExchangeDataManage::instance()->GetLatestActiveDay(QDate::currentDate().addDays(-6));
    mLast10DaysDate = QExchangeDataManage::instance()->GetLatestActiveDay(QDate::currentDate().addDays(-11));
    mLast1MonthDate = QExchangeDataManage::instance()->GetLatestActiveDay(QDate::currentDate().addMonths(-1));
}

void HqInfoService::initSignalSlot()
{
    connect(this, SIGNAL(signalInitDBTables()), this, SLOT(slotInitDBTables()));
    connect(this, SIGNAL(signalUpdateStockCodesList(QStringList)), this, SLOT(slotUpdateStockCodesList(QStringList)));
}


void HqInfoService::initBlockData(int type)
{
    QMutexLocker locker(&mBlockMutex);
    mDataBase.getBlockDataList(mBlockDataMap, type);
}

//从数据库中获取已经保存的数据
void HqInfoService::initShareData()
{
    QMutexLocker locker(&mShareMutex);
    mDataBase.getShareDataList(mStkRealInfo);
}


HqInfoService *HqInfoService::instance()
{

    if(m_pInstance == 0)//第一次检测
    {
        QMutexLocker locker(&mutex);//加互斥锁。
        if(m_pInstance == 0)
        {
            m_pInstance = new HqInfoService();
        }
    }
    return m_pInstance;
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
//            qDebug()<<"error:"<<mSqlQuery.lastError().text();
        }
    }
    QSqlDatabase::database().commit();
}

bool HqInfoService::addTop10ChinaStockInfo(const ChinaShareExchange &info)
{
    return true;
#if 0
    mSqlQuery.prepare("insert into hstop10 (id, name, close, change_percent, buy, sell, date) values ("
                      "?, ?, ?, ?, ?, ?, ?)");
    mSqlQuery.addBindValue(info.mCode);
    mSqlQuery.addBindValue(info.mName);
    mSqlQuery.addBindValue(info.mCur);
    mSqlQuery.addBindValue(info.mChgPercent);
    mSqlQuery.addBindValue(info.mTop10Buy);
    mSqlQuery.addBindValue(info.mTop10Sell);
    mSqlQuery.addBindValue(info.mDate);

    return mSqlQuery.exec();
#endif
}

QDate HqInfoService::getLastUpdateDateOfHSGT()
{
    return mDataBase.getLastUpdateDateOfTable("hstop10");
}

QDate HqInfoService::getLastUpdateDateOfHSGTVol()
{
    QDate date = mDataBase.getLastUpdateDateOfTable(HSGT_TABLE);
    if(date == QDate(2016,12,4))
    {
        date = QDate::currentDate().addDays(-30);
    }

    return date;
}

QDate HqInfoService::getLastUpdateDateOfShareHistory(const QString &code)
{
    return mDataBase.getLastUpdateDateOfShareHistory(code);
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
#if 0
   if(!mSqlQuery.exec(tr("select * from hstop10 %1 order by date desc, (buy-sell) desc").arg(filterList.length() > 0 ? " where " + filterList.join(" and ") : ""))) return false;
    qDebug()<<mSqlQuery.lastQuery();
    while (mSqlQuery.next()) {
        ChinaShareExchange info;
        info.mCode = mSqlQuery.value("id").toString();
        info.mName = mSqlQuery.value("name").toString();
        info.mCur = mSqlQuery.value("close").toDouble();
        info.mChgPercent = mSqlQuery.value("change_percent").toDouble();
        info.mTop10Buy = mSqlQuery.value("buy").toDouble();
        info.mTop10Sell = mSqlQuery.value("sell").toDouble();
        info.mDate = mSqlQuery.value("date").toDate();
        list.append(info);
    }
#endif

    return true;
}

void HqInfoService::slotRecvShareHistoryInfos(const StockDataList &list)
{
    //更新到数据库
    QSqlDatabase::database().transaction();
    foreach (StockData info, list) {
        if(!slotAddHistoryData(info))
        {
//            qDebug()<<"error:"<<mSqlQuery.lastError().text()<<" "<<mSqlQuery.lastQuery();
        }
    }
    QSqlDatabase::database().commit();
}

bool HqInfoService::slotAddHistoryData(const StockData &info)
{
    return true;
    QString tableName = "stk" + info.mCode.right(6);
//    mSqlQuery.prepare(tr("insert into %1 ("
//                         "name, close, open, high, low, "
//                         "change, change_percent, vol, money, puremoney, "
//                         "marketshare, mutalbleshare, date) values ("
//                         "?, ?, ?, ?, ?, "
//                         "?, ?, ?, ?, ?, "
//                         "?, ?, ?)"
//                         ).arg(tableName));
//    mSqlQuery.addBindValue(info.mName);
//    mSqlQuery.addBindValue(info.mCur);
//    mSqlQuery.addBindValue(info.mOpen);
//    mSqlQuery.addBindValue(info.mHigh);
//    mSqlQuery.addBindValue(info.mLow);

//    mSqlQuery.addBindValue(info.mChg);
//    mSqlQuery.addBindValue(info.mChgPercent);
//    mSqlQuery.addBindValue(info.mVol);
//    mSqlQuery.addBindValue(info.mMoney);
///   mSqlQuery.addBindValue(info.mZJLX);

//    mSqlQuery.addBindValue(info.mTotalShare);
//    mSqlQuery.addBindValue(info.mMutableShare);
//    mSqlQuery.addBindValue(info.mDate);
//    return mSqlQuery.exec();
}

void HqInfoService::slotQueryShareHistoryLastDate(const QString &code)
{
    emit signalSendShareHistoryLastDate(code, getLastUpdateDateOfShareHistory(code));
}


bool HqInfoService::GetHistoryInfoWithDate(const QString &table, const QDate &date, double &close, double &money, qint64 &total_share, qint64 &mutalble_share)
{
#if 0
//    qDebug()<<__FUNCTION__<<__LINE__<<table<<"  "<<date;
    if(mSqlQuery.exec(tr("select close, money, marketshare, mutalbleshare from %1 where date <= '%2' order by date desc limit 1").arg(table).arg(date.toString("yyyy-MM-dd"))))
    {
        while(mSqlQuery.next())
        {
            close = mSqlQuery.value("close").toDouble();
            money = mSqlQuery.value("money").toDouble();
            total_share = mSqlQuery.value("marketshare").toLongLong();
            mutalble_share = mSqlQuery.value("mutalbleshare").toLongLong();
            break;
        }

        //qDebug()<<__FUNCTION__<<__LINE__<<table<<"  "<<date<<" success";
    } else
    {
        //qDebug()<<__FUNCTION__<<__LINE__<<table<<"  "<<date<<" "<<mSqlQuery.lastError().text();
    }
#endif

    return false;
}

void HqInfoService::slotUpdateStkBaseinfoWithHistory(const QString &code)
{
//    qDebug()<<__FUNCTION__<<" "<<code;
    QString table = "stk" + code.right(6);
    double last_money = 0.0, last_close = 0.0;
    qint64 total_share = 0;
    qint64 mutable_share = 0;
    //查询昨日的信息
    GetHistoryInfoWithDate(table, mLastActiveDate, last_close, last_money, total_share, mutable_share);
    if(code == "600111")
        qDebug()<<"total:"<<total_share<<" "<<mutable_share<<" "<<last_close<<" "<<last_money<<" "<<mLastActiveDate;

    StockData *data = mStkRealInfo[code.right(6)];
    data->mCode = code.right(6);
    data->mTotalShare = total_share;
    data->mMutableShare = mutable_share;
    data->mLastMoney = last_money;
    data->mLast3DaysChgPers = GetMultiDaysChangePercent(table, 3);
    data->mLast5DaysChgPers = GetMultiDaysChangePercent(table, 5);
    data->mLast10DaysChgPers = GetMultiDaysChangePercent(table, 10);
    data->mLastMonthChgPers = GetMultiDaysChangePercent(table, 22);
    data->mLastClose = last_close;
    data->mProfit = mStkProfitMap[data->mCode];
    emit signalUpdateStkBaseinfoWithHistoryFinished(code);
}

void HqInfoService::slotUpdateHistoryChange(const QString &code)
{

}

StockData* HqInfoService::getBasicStkData(const QString &code)
{
    QMutexLocker locker(&mShareMutex);
    if(!mStkRealInfo.contains(code))
    {
        StockData *data = new StockData;
        data->mCode = code;
        mStkRealInfo[code] = data;
    }

    return mStkRealInfo[code];

}

double HqInfoService::GetMultiDaysChangePercent(const QString &table, int days)
{
    return 0.0;
}

#if 0
    if(!mSqlQuery.exec(tr("select 1+change_percent/100 from %1 order by date desc limit %2").arg(table).arg(days)))
    {
        return 0.0;
    }

    double res = 1.0;
    while (mSqlQuery.next()) {
        res *= mSqlQuery.value(0).toDouble();
    }



    return (res - 1) * 100;
}
#endif

void HqInfoService::GetForeignVolChange(const QString &code, qint64 &cur, qint64 &pre)
{
#if 0
    if(!mSqlQuery.exec(tr("select vol from %1 where code = '%2' order by date desc limit 2").arg(HSGT_TABLE).arg(code)))
    {
        return;
    }

    cur = 0;
    pre = 0;

    while (mSqlQuery.next()) {
        if(cur == 0)
        {
            cur = mSqlQuery.value(0).toLongLong();
        } else
        {
            pre = mSqlQuery.value(0).toLongLong();
        }
    }
#endif
}

void HqInfoService::slotUpdateStkProfitList(const StockDataList &list)
{
    foreach (StockData data, list) {
        mStkProfitMap[data.mCode.right(6)] = data.mProfit;
    }
}

void HqInfoService::slotAddShareAmoutByForeigner(const StockDataList &list)
{
#if 0
    //先检查表表是否存在，不存在，就添加
    if(!isTableExist(HSGT_TABLE)) createHSGTShareAmountTable();
    QSqlDatabase::database().transaction();
    foreach (StockData info, list) {
        mSqlQuery.prepare(tr("insert into %1 (code, vol, date) values ("
                          "?, ?, ?)").arg(HSGT_TABLE));
        mSqlQuery.addBindValue(info.mCode);
        mSqlQuery.addBindValue(info.mForeignVol);
        mSqlQuery.addBindValue(info.mDate);
        mSqlQuery.exec();
    }
    QSqlDatabase::database().commit();
#endif
}

double HqInfoService::getProfit(const QString &code)
{
    return mStkProfitMap[code.right(6)];
}

QStringList HqInfoService::getExchangeCodeList()
{
    return mStkProfitMap.keys();
}

void HqInfoService::slotUpdateShareAmountByForeigner()
{
#if 0
    QDate date = getLastUpdateDateOfHSGTVol();
    qDebug()<<"last date:"<<date;
    QDate preDate = date.addDays(-1);
    while (preDate.dayOfWeek() == 6 || preDate.dayOfWeek() == 7) {
        preDate = preDate.addDays(-1);
    }
    //取得昨天的数据
    //开始插入
    if(!mSqlQuery.exec(tr("select * from %1 where date >= '%2'").arg(HSGT_TABLE).arg(preDate.toString("yyyy-MM-dd"))))
    {
        qDebug()<<"error:"<<mSqlQuery.lastError().text();
    }
    while(mSqlQuery.next())
    {
        QString code = mSqlQuery.value("code").toString();
        qint64 num = mSqlQuery.value("vol").toLongLong();
        QDate curDate = mSqlQuery.value("date").toDate();
        if(curDate == date)
        {
            mStkForeignerHoldMap[code].last = num;
        } else {
            mStkForeignerHoldMap[code].previous = num;
        }
    }
//    qDebug()<<"sql:"<<mSqlQuery.lastQuery();
#endif
}

foreignHolder HqInfoService::amountForeigner(const QString &code)
{
    return mStkForeignerHoldMap[code];
}

BlockData*  HqInfoService::getBlockDataOfCode(const QString &code)
{
    QMutexLocker locker(&mBlockMutex);
    return mBlockDataMap.value(code, 0);

}

void   HqInfoService::setBlockData(BlockData *data)
{
    if(data == 0) return;
    QMutexLocker locker(&mBlockMutex);
    mBlockDataMap[data->mCode] = data;
}

void   HqInfoService::setShareBlock(const QString &code, const QString &block)
{
    QMutexLocker locker(&mShareMutex);
    QStringList &wklist = mShareBlockMap[code];
    if(!wklist.contains(block))
    {
        wklist.append(block);
    }
}

//从更新当前代码列表后，更新数据列表
void  HqInfoService::slotUpdateStockCodesList(const QStringList &list)
{
    QMutexLocker locker(&mShareMutex);
    foreach (QString code, list) {
        if(!mStkRealInfo.contains(code.right(6)))
        {
            StockData *data = new StockData;
            data->mCode = code.right(6);
            mStkRealInfo[code] = data;
        }
    }

    emit signalDbInitFinished();

}
