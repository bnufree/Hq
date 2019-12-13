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
#include "data_structure/shareworkingdatetime.h"

#define         DATE_STR_FORMAT         "yyyy-MM-dd"

#define     HSGT_TABLE          "hsgvol"
HqInfoService* HqInfoService::m_pInstance = 0;
HqInfoService::CGarbo HqInfoService::s_Garbo;
QMutex HqInfoService::mutex;

#define         HISTORY_CLOSE           "share_history_close"
#define         HISTORY_FOREIGN           "share_history_foreign_vol"



HqInfoService::HqInfoService(QObject *parent) :
    QObject(parent)
{
    mHistoryInfoCount = 0;
    mFavCodeList = Profiles::instance()->value(GLOBAL_SETTING, FAV_CODE, QStringList()).toStringList();
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
    qRegisterMetaType<ShareWorkingDate>("const ShareWorkingDate&");
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
    qRegisterMetaType<QList<QDate> >("const QList<QDate>&");
    qRegisterMetaType<ShareHistoryCounter>("const ShareHistoryCounter&");

     connect(this, SIGNAL(signalUpdateShareCloseDate(QList<QDate>)), this, SLOT(slotUpdateShareCloseDate(QList<QDate>)));
    connect(this, SIGNAL(signalInitDBTables()), this, SLOT(slotInitDBTables()));    
    connect(this, SIGNAL(signalUpdateShareBasicInfo(ShareDataList)), this, SLOT(slotUpdateShareBasicInfo(ShareDataList)));
    connect(this, SIGNAL(signalUpdateShareBonusInfo(ShareBonusList)), this, SLOT(slotUpdateShareBonusInfo(ShareBonusList)));
    connect(this, SIGNAL(signalUpdateShareHsgtTop10Info(ShareHsgtList)),
            this, SLOT(slotUpdateHsgtTop10Info(ShareHsgtList)));
    connect(this, SIGNAL(signalUpdateHsgtTop10Keys(ShareWorkingDate)),
            this, SLOT(slotUpdateHsgtTop10Keys(ShareWorkingDate)));
    connect(this, SIGNAL(signalQueryShareHsgtTop10List(QString,ShareWorkingDate)),
            this, SLOT(slotQueryShareHsgtTop10List(QString,ShareWorkingDate)));

    connect(this, SIGNAL(signalRecvShareHistoryInfos(ShareDataList, int)),
            this, SLOT(slotRecvShareHistoryInfos(ShareDataList, int)));
    connect(this, SIGNAL(signalSendShareHistoryUpdateDate(ShareWorkingDate, bool)),
            this, SLOT(slotSendShareHistoryUpdateDate(ShareWorkingDate, bool)));
    connect(this, SIGNAL(signalQueryShareHistoryUpdateDateList()),
            this, SLOT(slotQueryShareHistoryUpdateDateList()));
//    connect(this, SIGNAL(signalSendShareHistoryCloseInfo(ShareDataList)),
//            this, SLOT(slotSendShareHistoryCloseInfo(ShareDataList)));
//    connect(this, SIGNAL(signalSendShareHistoryForeignVolInfo(ShareDataList)),
//            this, SLOT(slotSendShareHistoryForeignVolInfo(ShareDataList)));
//    connect(this, SIGNAL(signalSendShareHistoryRzrqInfo(ShareDataList)),
//            this, SLOT(slotSendShareHistoryRzrqInfo(ShareDataList)));
//    connect(this, SIGNAL(signalSendShareHistoryZjlxInfo(ShareDataList)),
//            this, SLOT(slotSendShareHistoryZjlxInfo(ShareDataList)));

    connect(this, SIGNAL(signalUpdateShareinfoWithHistory(QString,double,double,double,double,double,double, qint64, qint64,ShareHistoryList)),\
            this, SLOT(slotUpdateShareinfoWithHistory(QString,double,double,double,double,double,double, qint64, qint64,ShareHistoryList)));
    connect(this, SIGNAL(signalUpdateShareCounter(ShareHistoryCounter)), this, SLOT(slotUpdateShareCounter(ShareHistoryCounter)));
    connect(this, SIGNAL(signalQueryShareForeignVol(QString)), this, SLOT(slotQueryShareForeignVol(QString)));
    connect(this, SIGNAL(signalSetFavCode(QString)), this, SLOT(slotSetFavCode(QString)));
    connect(this, SIGNAL(signalSaveFavCode(QString,bool)), this, SLOT(slotSaveFavCode(QString,bool)));
    connect(this, SIGNAL(signalSearchCodesOfText(QString)), this, SLOT(slotSearchCodesOfText(QString)));
    connect(this, SIGNAL(signalUpdateShareFinanceInfo(FinancialDataList)), this, SLOT(slotUpdateShareFinanceInfo(FinancialDataList)));
    connect(this, SIGNAL(signalQueryShareFinanceInfo(QStringList)), this, SLOT(slotQueryShareFinanceList(QStringList)));
    connect(this, SIGNAL(signalQueryShareFHSP(QString,ShareWorkingDate)), this, SLOT(slotQueryShareFHSP(QString,ShareWorkingDate)));
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
    qDebug()<<"text:"<<text<<"codes:"<<list;
    emit signalSendSearchCodesOfText(list);
}

//从数据库中获取已经保存的数据
void HqInfoService::initShareData()
{
    QMutexLocker locker(&mShareMutex);
    mRealShareData.clear();
    mDataBase.queryShareBasicInfo(mRealShareData);
    emit signalAllShareCodeList(mRealShareData.keys());
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



ShareWorkingDate HqInfoService::getLastUpdateDateOfHSGT()
{
    return mDataBase.getLastUpdateDateOfTable("hstop10");
}

ShareWorkingDate HqInfoService::getLastUpdateDateOfHSGTVol()
{
    ShareWorkingDate date = mDataBase.getLastUpdateDateOfTable(HSGT_TABLE);
    if(date.isNull())
    {
        date = QDate::currentDate().addDays(-30);
    }

    return date;
}

ShareWorkingDate HqInfoService::getLastUpdateDateOfBasicInfo()
{
    return getLastUpdateDateOfTable(TABLE_SHARE_BASIC_INFO);
}

ShareWorkingDate HqInfoService::getLastUpdateDateOfBonusInfo()
{
    return getLastUpdateDateOfTable(TABLE_SHARE_BONUS);
}

ShareWorkingDate HqInfoService::getLastUpdateDateOfHsgtTop10()
{
    return getLastUpdateDateOfTable(TABLE_HSGT_TOP10);
}

ShareWorkingDate HqInfoService::getLastUpdateDateOfFinanceInfo()
{
    return getLastUpdateDateOfTable(TABLE_SHARE_FINANCE);
}


ShareWorkingDate HqInfoService::getLastUpdateDateOfHistoryInfo()
{
#if 0
    return getLastUpdateDateOfTable(TABLE_SHARE_HISTORY);
#endif
    return ShareWorkingDate();
}

ShareWorkingDate HqInfoService::getLastUpdateDateOfTable(const QString& table)
{
    return mDataBase.getLastUpdateDateOfTable(table);
}



void HqInfoService::slotRecvShareHistoryInfos(const ShareDataList &list, int mode)
{
    qDebug()<<"start update history:"<<list.size();
    if(list.size() > 0)
    {
        QTime t;
        t.start();
        if(!mDataBase.updateShareHistory(list, mode))
        {
            qDebug()<<mDataBase.errMsg();
            return;
        }
        qDebug()<<"update history to db elapsed time:"<<t.elapsed() / 1000.0;
        //

    }
    //
//    if(!mDataBase.delShareHistory("", ShareWorkingDate(), ShareWorkingDate(ShareWorkingDate::latestActiveDay().date().addYears(-1))))
//    {
//        qDebug()<<mDataBase.errMsg();
//        return;
//    }
}

void HqInfoService::slotSendShareHistoryUpdateDate(const ShareWorkingDate &date, bool update)
{
#if 0
    if(update)
    {
        if(!mDataBase.updateDBUpdateDate(date, TABLE_SHARE_HISTORY))
        {
            qDebug()<<mDataBase.errMsg();
            emit signalUpdateHistoryInfoFinished();
            return;
        }
    }
    //开始统计
    //1)获取当前所有的代码数据
    QStringList codeList = mRealShareMap.keys();
    //2)根据每一个代码获取今年以来的成交信息
    ShareWorkingDate year_day(QDate(QDate::currentDate().year(), 1, 1));
    ShareWorkingDate month_date(QDate(QDate::currentDate().year(), QDate::currentDate().month(), 1));
    int dayNum = QDate::currentDate().dayOfWeek();
    ShareWorkingDate week_date(QDate::currentDate().addDays( -1* (dayNum-1)));
    foreach (QString code, codeList) {
        ShareDataList list;
        if(!mDataBase.queryHistoryInfoFromDate(list, code, year_day)) continue;
        if(list.size() == 0) continue;
        ShareData* data = getShareData(code);
        data->mHistory.mLastMoney = list.first().mMoney;
        data->mHistory.mLastClose = list.first().mClose;
        data->mHsgtData.mVolTotal = list.first().mHsgtData.mVolTotal;
        data->mHsgtData.mVolMutablePercent = list.first().mHsgtData.mVolMutablePercent;
        data->mHsgtData.mVolChange = list.first().mHsgtData.mVolChange;
        for(int i=0; i<list.size(); i++)
        {
            ShareData temp = list[i];
            if(temp.mTime.toString() >= year_day.toString()){
                data->mHistory.mChgPersFromYear_BAK *= (1+temp.mChgPercent * 0.01);
            }
            if(temp.mTime.toString() >= month_date.toString()){
                data->mHistory.mChgPersFromMonth_BAK *= (1+temp.mChgPercent * 0.01);
            }
            if(temp.mTime.toString() >= week_date.toString()){
                data->mHistory.mChgPersFromWeek_BAK *= (1+temp.mChgPercent * 0.01);
            }
        }
        if(list.size() > 2)
        {
            list = list.mid(0, 2);
            data->mHsgtData.mVolChange = list[1].mHsgtData.mVolTotal - list[0].mHsgtData.mVolTotal;
        }
//        data->mHistory.mChgPersFromMonth -= 1.0;
//        data->mHistory.mChgPersFromWeek -= 1.0;
//        data->mHistory.mChgPersFromYear -= 1.0;
//        qDebug()<<data->mCode<<data->mName<<data->mHistory.mChgPersFromWeek<<data->mHistory.mChgPersFromMonth<<data->mHistory.mChgPersFromYear;
    }
#endif
}

void HqInfoService::slotQueryShareHistoryUpdateDateList()
{
#if 0
    //获取上一次更新的日期
    QList<QDate> list;
    if(!mDataBase.queryShareHistroyUpdatedDates(list))
    {
        qDebug()<<mDataBase.errMsg();
        return;
    }    
    ShareWorkingDate last_update_date = getLastUpdateDateOfHistoryInfo();
    //检查数据表的更新日期和表的最后一次的日期是否相同，如果不相同，则将数据表的记录删除知道制定的日期
    if(list.size() > 0)
    {
        //清空数据记录
        if(!last_update_date.isNull())
        {
            if( !mDataBase.delShareHistory("", last_update_date, ShareWorkingDate()))
            {
                qDebug()<<mDataBase.errMsg();
                return;
            }
        }
    } else
    {
        //数据表中没有记录
        last_update_date.setDate(QDate());
    }
    //检查时间是否符合要求
    ShareWorkingDate cur_date = ShareWorkingDate::currentDate();
    ShareWorkingDate latest_act_date = ShareWorkingDate::latestActiveDay();
    ShareWorkingDate start_check_date = cur_date;
    if(cur_date == latest_act_date)
    {
        //今天是交易日，从今天的前一个交易日开始更新
        start_check_date = cur_date.previousActiveDay();
    } else
    {
        //今天不是交易日，从最近的交易日开始更新
        start_check_date = latest_act_date;
    }
    //检查数据库最后更新的日期是不是开始更新的check日期，如果是不处理；不是的情况，补足中间的日期
    QList<QDate> update_list;
    if(last_update_date.isNull())
    {
        last_update_date = ShareWorkingDate(QDate::currentDate().addYears(-1));
        if(last_update_date.isWeekend())
        {
            last_update_date = last_update_date.previousActiveDay();
        }
    }
    //补足中间需要更新的时间
    while(last_update_date != start_check_date)
    {
        if(!mShareCloseDateList.contains(start_check_date.date()))
            update_list.append(start_check_date.date());
        start_check_date = start_check_date.previousActiveDay();
    }
    //检查已经更新的时间中是否中间有断开的，吧断开的也一并添加
    for(int i=0; i<list.size(); i++)
    {
        ShareWorkingDate cur(list[i]);
        if(i+1 < list.size())
        {
            cur = cur.previousActiveDay();
            ShareWorkingDate next(list[i+1]);
            while (cur!= next)
            {
                if(!mShareCloseDateList.contains(cur.date()))
                {
                    update_list.append(cur.date());
                }
                cur = cur.previousActiveDay();
            }
        }
    }

    qSort(update_list);

    emit signalSendShareHistoryUpdateDateList(update_list);
#endif
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
    QString wkCode = ShareData::fullCode(code);
    if(!mRealShareData.contains(wkCode)) return;
    ShareData& data = mRealShareData[code];
    data.mHistory.mLastMoney = lastMoney * 0.0001;
    data.mHistory.mLast3DaysChgPers = last3Change;
    data.mHistory.mLast5DaysChgPers = last5Change;
    data.mHistory.mLast10DaysChgPers = last10Change;
    //data.mHistory.mLastMonthChgPers = lastMonthChange;
    data.mHistory.mChgPersFromYear = lastYearChange;
    data.mHsgtData.mVolTotal = vol;
    data.mHsgtData.mVolChange = vol_chnage;
    emit signalUpdateShareHistoryFinished(code);
}

//更新历史变化信息
void HqInfoService::slotUpdateShareCounter(const ShareHistoryCounter& counter)
{
    QString code = counter.code.right(6);
    ShareData *data = getShareData(code);
    if(!data) return;
    data->mHistory.mLastMoney = counter.lastMoney;
    data->mHistory.mYearDayPrice= counter.yearP;
    data->mHistory.mMonthDayPrice = counter.monthP;
    data->mHistory.mWeekDayPrice = counter.weekP;
    data->mHsgtData.mVolTotal = counter.foreign_vol;
    data->mHsgtData.mVolMutablePercent = counter.foreign_percent;
    data->mHsgtData.mVolCh1 = counter.foreign_ch1;
    data->mHsgtData.mVolCh5 = counter.foreign_ch5;
    data->mHsgtData.mVolCh10 = counter.foreign_ch10;
    //emit signalUpdateShareHistoryFinished(code);
}

void HqInfoService::slotUpdateHistoryChange(const QString &code)
{

}

void HqInfoService::slotSetFavCode(const QString &code)
{
    ShareData* data = getShareData(code);
    data->mIsFav = !(data->mIsFav);
    emit signalSaveFavCode(code, data->mIsFav);
}

void HqInfoService::slotSaveFavCode(const QString &code, bool fav)
{
    ShareDataList list;
    ShareData data;
    data.mCode = code.right(6);
    data.mIsFav = fav;
    list.append(data);
    if(!mDataBase.updateShareFavInfo(list))
    {
        qDebug()<<"update share fav info error:"<<mDataBase.getErrorString();
    }

    return;
}

ShareData* HqInfoService::getShareData(const QString &code)
{
    QMutexLocker locker(&mShareMutex);
    SHARE_DATA_TYPE type = ShareData::shareType(code);
    QString wkCode = ShareData::fullCode(code);
    if(!mRealShareData.contains(wkCode))
    {
        ShareData data;
        data.mCode = wkCode;
        data.mShareType = type;
        mRealShareData[wkCode] = data;
    }
    return (ShareData*)(&mRealShareData[wkCode]);
}

 ShareDataList HqInfoService::getShareDataList()
 {
//     QMutexLocker locker(&mShareMutex);
     return mRealShareData.values();
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
    ShareWorkingDate last_update_date;
    if( list.size() > 0 && !mDataBase.updateShareHsgtTop10List(list))
    {
        qDebug()<<"update share hsgt info error:"<<mDataBase.getErrorString();
        last_update_date = DATA_SERVICE->getLastUpdateDateOfHsgtTop10();
    }

}

void HqInfoService::slotUpdateHsgtTop10Keys(const ShareWorkingDate& date)
{
    ShareHsgtList list2;
    mHsgtTop10Kyes.clear();
    if(mDataBase.queryShareHsgtTop10List(list2, "", date))
    {
        mHsgtTop10Kyes.clear();
        foreach (ShareHsgt data, list2) {
            mHsgtTop10Kyes.append(data.mCode.right(6));
        }
    }
    qDebug()<<"hstop10 keys:"<<mHsgtTop10Kyes;

}

void HqInfoService::slotQueryShareHsgtTop10List(const QString &code, const ShareWorkingDate &date)
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
//            qDebug()<<share->mFinanceData.mCode<<share->mFinanceData.mROE<<share->mFinanceData.mEPS<<;
        }
    }

}

void HqInfoService::slotUpdateShareCloseDate(const QList<QDate> &list)
{
#if 0
    if(list.size() > 0)
    {
        if(!mDataBase.updateShareCloseDates(list))
        {
            qDebug()<<mDataBase.errMsg();
            return;
        }

        QList<QDate> list;
        if(mDataBase.queryShareCloseDates(list))
        {
            ShareWorkingDate::setUnWorkingDay(list);
        }
    }
#endif
}

void HqInfoService::slotQueryShareFHSP(const QString &code, const ShareWorkingDate &date)
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
    if(!mDataBase.queryShareProfitInfo(mRealShareData)) return;
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
    ShareData &data = mRealShareData[ShareData::fullCode(code)];
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

