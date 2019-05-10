#include <QDebug>
#include <QSharedPointer>
#include <QSqlError>
#include <QDir>
#include "dbservices.h"
#include <QMap>
#include "data_structure/sharedata.h"
#include "utils/profiles.h"
#include "qactivedate.h"
#include "utils/comdatadefines.h"

#define         DATE_STR_FORMAT         "yyyy-MM-dd"

#define     HSGT_TABLE          "hsgvol"
HqInfoService* HqInfoService::m_pInstance = 0;
HqInfoService::CGarbo HqInfoService::s_Garbo;
QMutex HqInfoService::mutex;


HqInfoService::HqInfoService(QObject *parent) :
    QObject(parent)
{
    mFavCodeList = Profiles::instance()->value(FAV_CODE_SEC, FAV_CODE_KEY, QStringList()).toStringList();
    QActiveDateTime::mCloseDateList = PROFILES_INS->value(CLOSE_DATE_SEC, CLOSE_DATE_KEY, QStringList()).toStringList();
    qDebug()<<"close:"<<QActiveDateTime::mCloseDateList;
    initSignalSlot();
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
//        initBlockData();
//        initShareData();
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

void HqInfoService::initSignalSlot()
{
    qRegisterMetaType<ShareDate>("const ShareDate&");
    qRegisterMetaType<ShareData>("const ShareData&");
    qRegisterMetaType<ShareDataList>("const ShareDataList&");
    qRegisterMetaType<ShareHsgtList >("const ShareHsgtList&");
    qRegisterMetaType<ShareHsgt>("const ShareHsgt&");
    qRegisterMetaType<ShareBonusList >("const ShareBonusList&");
    qRegisterMetaType<ShareBonus>("const ShareBonus&");
    qRegisterMetaType<FinancialDataList >("const FinancialDataList&");
    qRegisterMetaType<BlockDataList>("const BlockDataList&");
    qRegisterMetaType<QMap<QString, BlockDataList> >("const QMap<QString, BlockDataList>&");
    qRegisterMetaType<QMap<QString, BlockData*> >("const QMap<QString, BlockData*>&");

    connect(this, SIGNAL(signalInitDBTables()), this, SLOT(slotInitDBTables()));    
    connect(this, SIGNAL(signalUpdateShareBasicInfo(ShareDataList)), this, SLOT(slotUpdateShareBasicInfo(ShareDataList)));
    connect(this, SIGNAL(signalUpdateShareBonusInfo(ShareBonusList)), this, SLOT(slotUpdateShareBonusInfo(ShareBonusList)));
    connect(this, SIGNAL(signalUpdateShareHsgtTop10Info(ShareHsgtList)),
            this, SLOT(slotUpdateHsgtTop10Info(ShareHsgtList)));
    connect(this, SIGNAL(signalQueryShareHsgtTop10List(QString,ShareDate)),
            this, SLOT(slotQueryShareHsgtTop10List(QString,ShareDate)));

    connect(this, SIGNAL(signalRecvShareHistoryInfos(ShareDataList, int)), this, SLOT(slotRecvShareHistoryInfos(ShareDataList, int)));
    connect(this, SIGNAL(signalUpdateShareinfoWithHistory(QString,double,double,double,double,double,double, qint64, qint64,ShareHistoryList)),\
            this, SLOT(slotUpdateShareinfoWithHistory(QString,double,double,double,double,double,double, qint64, qint64,ShareHistoryList)));
    connect(this, SIGNAL(signalQueryShareForeignVol(QString)), this, SLOT(slotQueryShareForeignVol(QString)));
    connect(this, SIGNAL(signalSetFavCode(QString)), this, SLOT(slotSetFavCode(QString)));
    connect(this, SIGNAL(signalSearchCodesOfText(QString)), this, SLOT(slotSearchCodesOfText(QString)));
    connect(this, SIGNAL(signalUpdateShareFinanceInfo(FinancialDataList)), this, SLOT(slotUpdateShareFinanceInfo(FinancialDataList)));
    connect(this, SIGNAL(signalQueryShareFinanceInfo(QStringList)), this, SLOT(slotQueryShareFinanceList(QStringList)));
    connect(this, SIGNAL(signalQueryShareFHSP(QString,ShareDate)), this, SLOT(slotQueryShareFHSP(QString,ShareDate)));
}


void HqInfoService::initBlockData(int type)
{
    QMutexLocker locker(&mBlockMutex);
    mDataBase.queryBlockDataList(mRealBlockMap, type);
}

void HqInfoService::slotSearchCodesOfText(const QString &text)
{
    QStringList list;
    if(text.length() > 0)
    {
        if(!mDataBase.getSimilarCodeOfText(list, text))
        {
            qDebug()<<"error:"<<mDataBase.errMsg();
        }
    }
    qDebug()<<"codes:"<<list;
    emit signalSendSearchCodesOfText(list);
}

//从数据库中获取已经保存的数据
void HqInfoService::initShareData()
{
    QMutexLocker locker(&mShareMutex);
    mRealShareMap.clear();
    mDataBase.queryShareBasicInfo(mRealShareMap);
    emit signalAllShareCodeList(mRealShareMap.keys());
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



ShareDate HqInfoService::getLastUpdateDateOfHSGT()
{
    return mDataBase.getLastUpdateDateOfTable("hstop10");
}

ShareDate HqInfoService::getLastUpdateDateOfHSGTVol()
{
    ShareDate date = mDataBase.getLastUpdateDateOfTable(HSGT_TABLE);
    if(date.isNull())
    {
        date = QDate::currentDate().addDays(-30);
    }

    return date;
}

ShareDate HqInfoService::getLastUpdateDateOfBasicInfo()
{
    return getLastUpdateDateOfTable(TABLE_SHARE_BASIC_INFO);
}

ShareDate HqInfoService::getLastUpdateDateOfBonusInfo()
{
    return getLastUpdateDateOfTable(TABLE_SHARE_BONUS);
}

ShareDate HqInfoService::getLastUpdateDateOfHsgtTop10()
{
    return getLastUpdateDateOfTable(TABLE_HSGT_TOP10);
}

ShareDate HqInfoService::getLastUpdateDateOfFinanceInfo()
{
    return getLastUpdateDateOfTable(TABLE_SHARE_FINANCE);
}


ShareDate HqInfoService::getLastUpdateDateOfHistoryInfo(const QString& code)
{
    return mDataBase.getLastHistoryDateOfShare(code);
}

ShareDate HqInfoService::getLastUpdateDateOfTable(const QString& table)
{
    return mDataBase.getLastUpdateDateOfTable(table);
}



void HqInfoService::slotRecvShareHistoryInfos(const ShareDataList &list, int mode)
{
    //更新到数据库
    if(!mDataBase.updateShareHistory(list, mode))
    {
        qDebug()<<mDataBase.errMsg();
    }
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

//更新历史变化信息
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
    ShareData& data = mRealShareMap[code];
    data.mCode = code;
    data.mHistory.mLastMoney = lastMoney * 0.0001;
    data.mHistory.mLast3DaysChgPers = last3Change;
    data.mHistory.mLast5DaysChgPers = last5Change;
    data.mHistory.mLast10DaysChgPers = last10Change;
    data.mHistory.mLastMonthChgPers = lastMonthChange;
    data.mHistory.mChgPersFromYear = lastYearChange;
    data.mHsgtData.mVol = vol;
    data.mHsgtData.mVolDelta = vol_chnage;
    emit signalUpdateShareHistoryFinished(code);
}

void HqInfoService::slotUpdateHistoryChange(const QString &code)
{

}

ShareData* HqInfoService::getShareData(const QString &code)
{
    QMutexLocker locker(&mShareMutex);
    QString wkCode = code.right(6);
    if(!mRealShareMap.contains(wkCode))
    {
        ShareData data;
        data.mCode = wkCode;
        mRealShareMap[wkCode] = data;
    }
    return (ShareData*)(&mRealShareMap[wkCode]);
}

void HqInfoService::slotUpdateShareBasicInfo(const ShareDataList &list)
{
    //更新数据库
    if(!mDataBase.updateShareBasicInfo(list))
    {
        qDebug()<<"update share basic info error:"<<mDataBase.getErrorString();
        return;
    }
    initShareData();
    return;
}

void HqInfoService::slotUpdateShareBonusInfo(const ShareBonusList &list)
{
    if(!mDataBase.updateShareBonus(list))
    {
        qDebug()<<"update share bonus info error:"<<mDataBase.getErrorString();
        return;
    }
    //更新最近的消息
    signalQueryShareFHSP();

}

void HqInfoService::slotUpdateHsgtTop10Info(const ShareHsgtList &list)
{
    if(!mDataBase.updateShareHsgtTop10List(list))
    {
        qDebug()<<"update share hsgt info error:"<<mDataBase.getErrorString();
    }
    ShareDate last_update_date = DATA_SERVICE->getLastUpdateDateOfHsgtTop10();
    emit signalSendLastHSGTUpdateDate(last_update_date);
}

void HqInfoService::slotQueryShareHsgtTop10List(const QString &code, const ShareDate &date)
{
    ShareHsgtList list;
    if(!mDataBase.queryShareHsgtTop10List(list, code, date))
    {
        qDebug()<<"update share hsgt info error:"<<mDataBase.getErrorString();
        return;
    }

    emit signalSendShareHsgtTop10List(list);

}

void HqInfoService::slotUpdateShareFinanceInfo(const FinancialDataList& list)
{
    if(!mDataBase.updateShareFinance(list))
    {
        qDebug()<<"update share finance info error:"<<mDataBase.getErrorString();
        return;
    }
    //更新对应的财务数据
    emit signalQueryShareFinanceInfo();
}

void HqInfoService::slotQueryShareFinanceList(const QStringList& codes)
{
    FinancialDataList list;
    if(!mDataBase.queryShareFinance(list, codes)) return;
    foreach (FinancialData data, list) {
        ShareData* share = getShareData(data.mCode.right(6));
        if(share)
        {
            share->mFinanceData = data;
            //qDebug()<<share->mFinanceData.mCode<<share->mFinanceData.mROE<<share->mFinanceData.mEPS;
        }
    }

}

void HqInfoService::slotQueryShareFHSP(const QString &code, const ShareDate &date)
{
    ShareBonusList list;
    //根据各种情况查询分红送配信息, 如果两个都不设定,查询对应的单独的最近的送配信息,否则就查询指定的信息并且推送出去
    if(!mDataBase.queryShareBonus(list, code, date)) return;
    if(code.length() == 0 && date.isNull())
    {
        QDateTime cur = QDateTime::currentDateTime();
        int year = cur.date().year();
        QStringList dateList;
        dateList.append(QDate(year-1, 12, 31).toString("yyyy-MM-dd"));
        dateList.append(QDate(year-1, 6, 30).toString("yyyy-MM-dd"));
        dateList.append(QDate(year, 12, 31).toString("yyyy-MM-dd"));
        foreach (ShareBonus data, list) {
            if(!dateList.contains(data.mDate.toString())) continue;
            ShareData* share = getShareData(data.mCode.right(6));
            if(share)
            {
                share->mBonusData  = data;
            }
        }
    }


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
    if(!mDataBase.updateShareProfitInfo(list))
    {
        emit signalDBErrorMsg(tr("update db profit error:%1").arg(mDataBase.errMsg()));
        return;
    }
    if(!mDataBase.queryShareProfitInfo(mRealShareMap)) return;
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
    if(!mRealBlockMap.contains(code))
    {
        BlockData tar;
        tar.mCode = code;
        tar.mIsFav = false;
        mRealBlockMap[code] = tar;
    }
    return (BlockData*)(&mRealBlockMap[code]);
}

//BlockData*   HqInfoService::setBlockData(const BlockData& data)
//{
//    QMutexLocker locker(&mBlockMutex);
//    mBlockDataMap[data.mCode] = data;
//}

void   HqInfoService::setBlockShareCodes(const QString &block, const QStringList &codes)
{
    QMutexLocker locker(&mBlockMutex);
    mRealBlockMap[block].mShareCodeList = codes;
}

void   HqInfoService::setShareBlock(const QString &code, const QString &block)
{
    QMutexLocker locker(&mShareMutex);
    ShareData &data = mRealShareMap[code.right(6)];
    if(!data.isContainsBlock(block))
    {
        data.mBlockCodeList.append(block);
    }
//        BlockData* blockptr = mBlockDataMap[block];
//        if(blockptr && (!data->mBlockList.contains(blockptr)))
//        {
//            data->mBlockList.append(blockptr);
//        }
//    }
}


BlockDataList HqInfoService::getAllBlock()
{
    return mRealBlockMap.values();
}

ShareDataList HqInfoService::getShareHistoryDataList(const QString& code)
{
    ShareDataList list;
    mDataBase.queryShareHistory(list, code);
    return list;
}

#if 0
//交易日期的处理
bool HqInfoService::weekend(const QDate &date)
{
    return date.dayOfWeek() == 6 || date.dayOfWeek() == 7;
}

bool HqInfoService::activeDay(const QDate &date)
{
    return !(weekend(date) || mClosedDateList.contains(date2Str(date)));
}

bool HqInfoService::isActiveTime(const QTime &time)
{
    int act_start1 = 9*60+15;
    int act_end1 = 11*60+30;
    int act_start2 = 13*60;
    int act_end2 = 15*60;
    int hour = time.hour();
    int minute = time.minute();
    int res = hour *60 + minute;
    if((res >= act_start1 && res <= act_end1) || (res >= act_start2 && res <= act_end2))
    {
            return true;
    }
    return false;
}

bool HqInfoService::isCurrentActive()
{
    QDateTime cur = QDateTime::currentDateTime();
    if(!activeDay((cur.date()))) return false;
    return isActiveTime(cur.time());
}


QString  HqInfoService::date2Str(const QDate& date)
{
    return date.toString(DATE_STR_FORMAT);
}

QDate  HqInfoService::dateFromStr(const QString& str)
{
    return QDate::fromString(str, DATE_STR_FORMAT);
}


QDate   HqInfoService::latestActiveDay()
{
    QDate date = QDate::currentDate();
    while(!activeDay(date))
    {
        date = date.addDays(-1);
    }

    return date;
}

QDate   HqInfoService::lastActiveDay()
{
    QDate date =latestActiveDay().addDays(-1);
    while(!activeDay(date))
    {
        date = date.addDays(-1);
    }

    return date;
}



QDate HqInfoService::getActiveDayBefore1HYear()
{
    QDate date = QDate::currentDate();
    date = date.addDays(-182);
    while (!activeDay(date)) {
        date = date.addDays(1);
    }

    return date;
}

int   HqInfoService::activeDaysNum(const QDate &start)
{
    int num = 0;
    QDate wkdate = start;
    while (wkdate < latestActiveDay()) {
        if(activeDay(wkdate))
        {
            num++;
        }
        wkdate = wkdate.addDays(1);
    }

    return num;
}

QDate HqInfoService::preActiveDay(const QDate &date)
{

}

QDate HqInfoService::nextActiveDay(const QDate &date)
{
//    QDate wkdate = date.addDays(1);
//    while (wkdate.) {

//    }
}
#endif

QDate HqInfoService::getLgtStartDate()
{
    return QDate(2017,3,17);
}

