#include <QDebug>
#include <QSharedPointer>
#include <QSqlError>
#include <QDir>
#include "dbservices.h"
#include "qexchangedatamanage.h"
#include <QMap>
#include "utils/sharedata.h"
#include "utils/profiles.h"


#define     HSGT_TABLE          "hsgvol"
#define     FAV_CODE_SEC        "ZXG"
#define     FAV_CODE            "codes"
HqInfoService* HqInfoService::m_pInstance = 0;
HqInfoService::CGarbo HqInfoService::s_Garbo;
QMutex HqInfoService::mutex;
HqInfoService::HqInfoService(QObject *parent) :
    QObject(parent)
{
    qRegisterMetaType<ShareDataList>("const ShareDataList&");
    qRegisterMetaType<FinDataList>("const FinDataList&");
    qRegisterMetaType<ShareBaseDataList>("const ShareBaseDataList&");
    mFavCodeList = Profiles::instance()->value(FAV_CODE_SEC, FAV_CODE, QStringList()).toStringList();
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

//初始化数据库
void HqInfoService::slotInitDBTables()
{
    qDebug()<<__FUNCTION__<<__LINE__;
    if(mDataBase.createDBTables())
    {
        //initBlockData();
        //initShareData();
        emit signalDbInitFinished();
    } else
    {
        qDebug()<<"init DB Tables failed:"<<mDataBase.errMsg();
    }
    qDebug()<<__FUNCTION__<<__LINE__;
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
    connect(this, SIGNAL(signalUpdateShareCodesList(QStringList)), this, SLOT(slotUpdateShareCodesList(QStringList)));
    connect(this, SIGNAL(signalRecvShareHistoryInfos(QString,ShareDataList, bool)), this, SLOT(slotRecvShareHistoryInfos(QString,ShareDataList, bool)));
    connect(this, SIGNAL(signalUpdateShareinfoWithHistory(QString,double,double,double,double,double,double, qint64, qint64,ShareDataList)),\
            this, SLOT(slotUpdateShareinfoWithHistory(QString,double,double,double,double,double,double, qint64, qint64,ShareDataList)));
    connect(this, SIGNAL(signalUpdateShareBasicInfo(ShareBaseDataList)), this, SLOT(slotUpdateShareBasicInfo(ShareBaseDataList)));
    connect(this, SIGNAL(signalQueryShareForeignVol(QString)), this, SLOT(slotQueryShareForeignVol(QString)));
    connect(this, SIGNAL(signalRecvAllShareHistoryInfos(ShareDataList,bool)), this, SLOT(slotRecvAllShareHistoryInfos(ShareDataList,bool)));
    connect(this, SIGNAL(signalSetFavCode(QString)), this, SLOT(slotSetFavCode(QString)));
    connect(this, SIGNAL(signalSearchCodesOfText(QString)), this, SLOT(slotSearchCodesOfText(QString)));
}


void HqInfoService::initBlockData(int type)
{
    QMutexLocker locker(&mBlockMutex);
    mDataBase.getBlockDataList(mBlockDataMap, type);
}

void HqInfoService::slotSearchCodesOfText(const QString &text)
{
    QStringList list;
    if(!mDataBase.getSimilarCodeOfText(list, text))
    {
        qDebug()<<"error:"<<mDataBase.errMsg();
    }
    qDebug()<<"codes:"<<list;
    emit signalSendSearchCodesOfText(list);
}

//从数据库中获取已经保存的数据
void HqInfoService::initShareData()
{
    QMutexLocker locker(&mShareMutex);
    mDataBase.getBasicShareDataList(mStkRealInfo);
}

void HqInfoService::saveShares()
{
    //mDataBase.saveShareDataList(mStkRealInfo);
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

QDate HqInfoService::getLastUpdateDateOfBasicInfo()
{
    return getLastUpdateDateOfTable(HQ_SHARE_BASIC_INFO_TABLE);
}

QDate HqInfoService::getLastUpdateDateOfHistoryInfo()
{
    return getLastUpdateDateOfTable(HQ_SHARE_HISTORY_INFO_TABLE);
}

QDate HqInfoService::getLastUpdateDateOfTable(const QString& table)
{
    return mDataBase.getLastUpdateDateOfTable(table);
}



void HqInfoService::slotRecvShareHistoryInfos(const QString& code, const ShareDataList &list, bool deletedb)
{
    //更新到数据库
    if(!mDataBase.updateHistoryDataList(list,deletedb))
    {
        qDebug()<<mDataBase.errMsg();
    }
}

void HqInfoService::slotRecvAllShareHistoryInfos(const ShareDataList &list, bool deletedb)
{
    //更新到数据库
    if(!mDataBase.updateHistoryDataList(list, deletedb))
    {
        qDebug()<<mDataBase.errMsg();
        return;
    }

    emit signalUpdateHistoryInfoFinished();
}

bool HqInfoService::slotAddHistoryData(const ShareData &info)
{
    return true;
    //QString tableName = "stk" + info.mCode.right(6);
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

//void HqInfoService::slotQueryShareHistoryLastDate(const QString &code)
//{
//    emit signalSendShareHistoryLastDate(code, getLastUpdateDateOfShareHistory(code));
//}


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

void HqInfoService::slotUpdateShareinfoWithHistory(const QString& code,\
                                                   double lastMoney,\
                                                   double last3Change,\
                                                   double last5Change,\
                                                   double last10Change,\
                                                   double lastMonthChange,\
                                                   double lastYearChange,\
                                                   qint64 vol,\
                                                   qint64 vol_chnage,\
                                                   const ShareDataList& list)
{
    mShareHistoryDataList[ShareBaseData::fullCode(code.right(6))] = list;
    ShareData *data = mStkRealInfo[ShareBaseData::fullCode(code.right(6))];
    if(data)
    {
        data->mLastMoney = lastMoney * 0.0001;
        data->mLast3DaysChgPers = last3Change;
        data->mLast5DaysChgPers = last5Change;
        data->mLast10DaysChgPers = last10Change;
        data->mLastMonthChgPers = lastMonthChange;
        data->mChgPersFromYear = lastYearChange;
        data->mForeignVol = vol;
        data->mForeignVolChg = vol_chnage;
    }
    emit signalUpdateShareHistoryFinished(code);
}

void HqInfoService::slotUpdateHistoryChange(const QString &code)
{

}

ShareData* HqInfoService::getBasicStkData(const QString &code)
{
    QString key = ShareBaseData::fullCode(code);
    QMutexLocker locker(&mShareMutex);
    if(!mStkRealInfo.contains(key))
    {
        ShareData *data = new ShareData;
        data->setCode(key);
        mStkRealInfo[key] = data;
    }
    return mStkRealInfo[key];

}

void HqInfoService::slotUpdateShareBasicInfo(const ShareBaseDataList &list)
{
    {
        //更新数据库避免锁，将锁添加到括号内的局部变量
        QMutexLocker locker(&mShareMutex);
        //更新本地缓存
        foreach (ShareBaseData data, list) {
            ShareData *res = mStkRealInfo[data.mCode];
            if(!res)
            {
                res = new ShareData(data);
                res->setCode(ShareBaseData::fullCode(QString::fromStdString(data.mCode)));
                mStkRealInfo[data.mCode] = res;
            }
            if(res)
            {
                res->mShareType = data.mShareType;
                res->mIsTop10 = data.mIsTop10;
                res->mMGJZC = data.mMGJZC ;
                res->mMGSY = data.mMGSY;
                res->mTotalShare = data.mTotalShare;
                res->mMutalShare = data.mMutalShare;
                res->mJZCSYL = data.mJZCSYL;
                res->mXJFH = data.mXJFH;
                res->mSZZG = data.mSZZG;
                res->mYAGGR = data.mYAGGR;
                res->mGQDJR = data.mGQDJR;
                res->mTop10Buy = data.mTop10Buy;
                res->mTop10Sell = data.mTop10Sell;
                res->setPY(QString::fromStdString(data.mPY));
                res->mProfit = data.mProfit;
                if(mFavCodeList.contains(ShareBaseData::fullCode(QString::fromStdString(data.mCode))))
                {
                    res->mIsFav = true;
                } else
                {
                    res->mIsFav = false;
                }
            }
        }
    }

    //更新数据库
    if(!mDataBase.clearAndUpdateBasicShareDataList(mStkRealInfo.values()))
    {
        qDebug()<<"error:"<<mDataBase.getErrorString();
    }

    qDebug()<<__FUNCTION__<<__LINE__;
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

void HqInfoService::slotUpdateStkProfitList(const ShareDataList &list)
{
    foreach (ShareData data, list) {
        mStkProfitMap[QString::fromStdString(data.mCode)] = data.mProfit;
    }
}

void HqInfoService::slotAddShareAmoutByForeigner(const ShareDataList &list)
{
#if 0
    //先检查表表是否存在，不存在，就添加
    if(!isTableExist(HSGT_TABLE)) createHSGTShareAmountTable();
    QSqlDatabase::database().transaction();
    foreach (ShareData info, list) {
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
    BlockData* data= mBlockDataMap.value(code, 0);
    if(!data)
    {
        data = new BlockData;
        data->mCode = code;
        data->mIsFav = false;
        mBlockDataMap[code] = data;
    }
    return data;

}

void   HqInfoService::setBlockData(BlockData *data)
{
    if(data == 0) return;
    QMutexLocker locker(&mBlockMutex);
    mBlockDataMap[data->mCode] = data;
}

void   HqInfoService::setBlockShareCodes(const QString &block, const QStringList &codes)
{
    QMutexLocker locker(&mBlockMutex);
    mBlockDataMap[block]->mShareCodeList = codes;
}

void   HqInfoService::setShareBlock(const QString &code, const QString &block)
{
    QMutexLocker locker(&mShareMutex);
    ShareData *data = mStkRealInfo[ShareBaseData::fullCode(code.right(6))];
    if(data)
    {
        if(!data->isContainsBlock(block.toInt()))
        {
            data->appendBlock(block.toInt());
        }
//        BlockData* blockptr = mBlockDataMap[block];
//        if(blockptr && (!data->mBlockList.contains(blockptr)))
//        {
//            data->mBlockList.append(blockptr);
//        }
    }
}

//从更新当前代码列表后，更新数据列表
void  HqInfoService::slotUpdateShareCodesList(const QStringList &list)
{
    QMutexLocker locker(&mShareMutex);
    foreach (QString code, list) {
        if(!mStkRealInfo.contains(code.right(6)))
        {
            ShareData *data = new ShareData;
            data->setCode(code.right(6));
            mStkRealInfo[code.right(6)] = data;
            //qDebug()<<__FUNCTION__<<code<<data;
        }
    }

    //qDebug()<<"list:"<<list.size()<<mStkRealInfo.size();
    //emit signalDbInitFinished();

}

void HqInfoService::slotSetFavCode(const QString &code)
{
    QMutexLocker locker(&mShareMutex);
    ShareData* data = mStkRealInfo[ShareBaseData::fullCode(code)];
    if(data)
    {
        data->mIsFav = !data->mIsFav;
        if(data->mIsFav && !mFavCodeList.contains(ShareBaseData::fullCode(code)))
        {
            mFavCodeList.append(ShareBaseData::fullCode(code));
        } else if(!data->mIsFav)
        {
            mFavCodeList.removeAll(ShareBaseData::fullCode(code));
        }
        Profiles::instance()->setValue(FAV_CODE_SEC, FAV_CODE, mFavCodeList);
    }
}

void HqInfoService::slotQueryShareForeignVol(const QString& code)
{
    ShareDataList list;
    mDataBase.getHistoryDataOfCode(list, code);
    emit signalSendShareForeignVol(list);
}

BlockDataPList HqInfoService::getAllBlock()
{
    return mBlockDataMap.values();
}

ShareDataList HqInfoService::getShareHistoryDataList(const QString& code)
{
    return mShareHistoryDataList[ShareBaseData::fullCode(code)];
}
