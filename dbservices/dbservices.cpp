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
//#include "date/shareworkingdatetime.h"

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
    initSignalSlot();
    //3、开启异步通讯
//    m_threadWork.setStackSize(20*1000000);
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
        initShareData(false);
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
    qRegisterMetaType<QDate>("const QDate&");
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
    qRegisterMetaType<QList<ShareHistoryCounter>>("const QList<ShareHistoryCounter>&");
    qRegisterMetaType<QList<ShareExchangeData> >("const QList<ShareExchangeData>&");

     connect(this, SIGNAL(signalUpdateShareCloseDate(QList<QDate>)), this, SLOT(slotUpdateShareCloseDate(QList<QDate>)));
    connect(this, SIGNAL(signalInitDBTables()), this, SLOT(slotInitDBTables()));    
    connect(this, SIGNAL(signalUpdateShareBasicInfo(ShareDataList)), this, SLOT(slotUpdateShareBasicInfo(ShareDataList)));
    connect(this, SIGNAL(signalUpdateShareBonusInfo(ShareBonusList)), this, SLOT(slotUpdateShareBonusInfo(ShareBonusList)));
    connect(this, SIGNAL(signalUpdateShareHsgtTop10Info(ShareHsgtList)),
            this, SLOT(slotUpdateHsgtTop10Info(ShareHsgtList)));
    connect(this, SIGNAL(signalUpdateShareHsgtCounter(ShareHsgtList)),
            this, SLOT(slotUpdateShareHsgtCounter(ShareHsgtList)));
    connect(this, SIGNAL(signalUpdateHsgtTop10Keys(QDate)),
            this, SLOT(slotUpdateHsgtTop10Keys(QDate)));
    connect(this, SIGNAL(signalQueryShareHsgtTop10List(QString,QDate)),
            this, SLOT(slotQueryShareHsgtTop10List(QString,QDate)));
    connect(this, SIGNAL(signalQueryLatestHsgtData()),
            this, SLOT(slotQueryLatestHsgtData()));

    connect(this, SIGNAL(signalRecvShareHistoryInfos(ShareDataList, int)),
            this, SLOT(slotRecvShareHistoryInfos(ShareDataList, int)));
    connect(this, SIGNAL(signalSendShareHistoryUpdateDate(QDate, bool)),
            this, SLOT(slotSendShareHistoryUpdateDate(QDate, bool)));
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

    connect(this, SIGNAL(signalUpdateShareCounter(QList<ShareHistoryCounter>)), this, SLOT(slotUpdateShareCounter(QList<ShareHistoryCounter>)));
    connect(this, SIGNAL(signalQueryShareForeignVol(QString)), this, SLOT(slotQueryShareForeignVol(QString)));
    connect(this, SIGNAL(signalSetFavCode(QString)), this, SLOT(slotSetFavCode(QString)));
    connect(this, SIGNAL(signalSaveFavCode(QString,bool)), this, SLOT(slotSaveFavCode(QString,bool)));
    connect(this, SIGNAL(signalSearchCodesOfText(QString)), this, SLOT(slotSearchCodesOfText(QString)));
    connect(this, SIGNAL(signalUpdateShareFinanceInfo(FinancialDataList)), this, SLOT(slotUpdateShareFinanceInfo(FinancialDataList)));
    connect(this, SIGNAL(signalQueryShareFinanceInfo(QStringList)), this, SLOT(slotQueryShareFinanceList(QStringList)));
    connect(this, SIGNAL(signalQueryShareFHSP(QString,QDate)), this, SLOT(slotQueryShareFHSP(QString,QDate)));
    connect(this, SIGNAL(signalUpdateShareExchangeRecord(QList<ShareExchangeData>)), this, SLOT(slotUpdateShareExchangeRecord(QList<ShareExchangeData>)));
    connect(this, SIGNAL(signalQueryShareExchangeRecord(int, QString,QString,QString)), this, SLOT(slotQueryShareExchangeRecord(int, QString,QString,QString)));
    connect(this, SIGNAL(signalDeleteShareExchangeRecord(QString,QString,QString)), this, SLOT(slotDeleteShareExchangeRecord(QString,QString,QString)));

    connect(this, SIGNAL(signalSendForeignDataList(QList<ShareForeignVolCounter>, QString )),
            this, SLOT(slotRecvForeignCounterList(QList<ShareForeignVolCounter>, QString)));
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
void HqInfoService::initShareData(bool send)
{
    QMutexLocker locker(&mShareMutex);
    mRealShareData.clear();
    mDataBase.queryShareBasicInfo(mRealShareData, mFavCodeList);
    if(send)    emit signalAllShareCodeList(mRealShareData.keys());
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
    if(date.isNull())
    {
        date = QDate::currentDate().addDays(-30);
    }

    return date;
}

QDate HqInfoService::getLastUpdateDateOfBasicInfo()
{
    return getLastUpdateDateOfTable(TABLE_SHARE_BASIC_INFO);
}

QDate HqInfoService::getLastUpdateDateOfBonusInfo()
{
    return getLastUpdateDateOfTable(TABLE_SHARE_BONUS);
}

QDate HqInfoService::getLastUpdateDateOfHsgtTop10()
{
    return getLastUpdateDateOfTable(TABLE_HSGT_TOP10);
}

QDate HqInfoService::getLastUpdateDateOfFinanceInfo()
{
    return getLastUpdateDateOfTable(TABLE_SHARE_FINANCE);
}


QDate HqInfoService::getLastUpdateDateOfHistoryInfo()
{
#if 0
    return getLastUpdateDateOfTable(TABLE_SHARE_HISTORY);
#endif
    return QDate();
}

QDate HqInfoService::getLastUpdateDateOfTable(const QString& table)
{
    return mDataBase.getLastUpdateDateOfTable(table);
}

void HqInfoService::slotUpdateShareHsgtCounter(const ShareHsgtList &list)
{
    foreach (ShareHsgt hsgt, list) {
        QString code = hsgt.mCode.right(6);
        ShareData& data = getShareData(code);
//        data.mHsgtData = hsgt;
    }
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
//    if(!mDataBase.delShareHistory("", QDate(), QDate(QDate::latestActiveDay().date().addYears(-1))))
//    {
//        qDebug()<<mDataBase.errMsg();
//        return;
//    }
}

void HqInfoService::slotSendShareHistoryUpdateDate(const QDate &date, bool update)
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
    QDate year_day(QDate(QDate::currentDate().year(), 1, 1));
    QDate month_date(QDate(QDate::currentDate().year(), QDate::currentDate().month(), 1));
    int dayNum = QDate::currentDate().dayOfWeek();
    QDate week_date(QDate::currentDate().addDays( -1* (dayNum-1)));
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
    QDate last_update_date = getLastUpdateDateOfHistoryInfo();
    //检查数据表的更新日期和表的最后一次的日期是否相同，如果不相同，则将数据表的记录删除知道制定的日期
    if(list.size() > 0)
    {
        //清空数据记录
        if(!last_update_date.isNull())
        {
            if( !mDataBase.delShareHistory("", last_update_date, QDate()))
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
    QDate cur_date = QDate::currentDate();
    QDate latest_act_date = QDate::latestActiveDay();
    QDate start_check_date = cur_date;
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
        last_update_date = QDate(QDate::currentDate().addYears(-1));
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
        QDate cur(list[i]);
        if(i+1 < list.size())
        {
            cur = cur.previousActiveDay();
            QDate next(list[i+1]);
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
void HqInfoService::slotUpdateShareCounter(const QList<ShareHistoryCounter>& list)
{
    foreach(ShareHistoryCounter counter, list)
    {
        QString code = counter.code.right(6);
        ShareData& data = getShareData(code);
        data.mCounterInfo = counter.info;
    }
}

void HqInfoService::slotRecvForeignCounterList(const QList<ShareForeignVolCounter>& list, const QString& date )
{
    static QString wkDate;
    if(wkDate != date)
    {
        foreach(ShareForeignVolCounter counter, list)
        {
            QString code;
            code.sprintf("%06d", counter.mCode);
            ShareData& data = getShareData(code);
            data.mForeignInfo.mVol = counter.mShareHold;
            data.mForeignInfo.mPercent = counter.mLTZB;
            data.mForeignInfo.mJMR1 = counter.mChg1.mShareSZ_Change;
            data.mForeignInfo.mJMR5 = counter.mChg5.mShareSZ_Change;
            data.mForeignInfo.mJMR10 = counter.mChg10.mShareSZ_Change;
            data.mForeignInfo.mJMRM = counter.mChgM.mShareSZ_Change;

        }
    }
    //emit signalUpdateShareHistoryFinished(code);
}

void HqInfoService::slotUpdateHistoryChange(const QString &code)
{

}

void HqInfoService::slotSetFavCode(const QString &code)
{    
    ShareData& data = getShareData(code);
    data.mIsFav = !(data.mIsFav);
    qDebug()<<"set code:"<<code<<data.mCode;
    if(data.mIsFav)
    {
        if(!mFavCodeList.contains(data.mCode)) mFavCodeList.append(data.mCode);
    } else
    {
        mFavCodeList.removeOne(data.mCode);
    }

    Profiles::instance()->setValue(GLOBAL_SETTING, FAV_CODE, mFavCodeList);
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

ShareData& HqInfoService::getShareData(const QString &code)
{
    QMutexLocker locker(&mShareMutex);
    SHARE_DATA_TYPE type = ShareData::shareType(code);
    QString wkCode = ShareData::fullCode(code);
    if(!mRealShareData.contains(wkCode))
    {
        ShareData data;
        data.mCode = wkCode;
        data.mType = type;
        mRealShareData[wkCode] = data;
    }
    return mRealShareData[wkCode];
}

ShareDataList HqInfoService::getShareDataList(int type)
{
    QMutexLocker locker(&mShareMutex);
    ShareDataList list;
    foreach(ShareData data, mRealShareData)
    {
        if(data.mType & type)
        {
            list.append(data);
        }
    }
    return list;
}

 ShareDataList HqInfoService::getShareDataList()
 {
     QMutexLocker locker(&mShareMutex);
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
    initShareData(true);
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
    QDate last_update_date;
    if( list.size() > 0 && !mDataBase.updateShareHsgtTop10List(list))
    {
        qDebug()<<"update share hsgt info error:"<<mDataBase.getErrorString();
        last_update_date = DATA_SERVICE->getLastUpdateDateOfHsgtTop10();
    }

}

void HqInfoService::slotUpdateHsgtTop10Keys(const QDate& date)
{
    ShareHsgtList list2;
    mHsgtTop10Kyes.clear();
    if(mDataBase.queryShareHsgtTop10List(list2, "", date))
    {
        mHsgtTop10Kyes.clear();
        foreach (ShareHsgt data, list2) {
            mHsgtTop10Kyes.append(data.mCode.right(6));
        }

        emit signalSendShareHsgtTop10List(list2, "", date);
    }
    qDebug()<<"hstop10 keys:"<<mHsgtTop10Kyes;

}

void HqInfoService::slotQueryLatestHsgtData()
{
    QDate date = getLastUpdateDateOfHsgtTop10();
    slotQueryShareHsgtTop10List("", date);
}

void HqInfoService::slotQueryShareHsgtTop10List(const QString &code, const QDate &date)
{
    ShareHsgtList list;
    if(!mDataBase.queryShareHsgtTop10List(list, code, date))
    {
        qDebug()<<"update share hsgt info error:"<<mDataBase.getErrorString();
        return;
    }


    emit signalSendShareHsgtTop10List(list, code, date);

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
//    FinancialDataList list;
//    if(!mDataBase.queryShareFinance(list, codes)) return;
//    foreach (FinancialData data, list) {
//        ShareData& share = getShareData(data.mCode.right(6));
//        share.mFinanceData = data;
////            qDebug()<<share->mFinanceData.mCode<<share->mFinanceData.mROE<<share->mFinanceData.mEPS<<;
//    }

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
            QDate::setUnWorkingDay(list);
        }
    }
#endif
}

void HqInfoService::slotQueryShareFHSP(const QString &code, const QDate &date)
{
//    ShareBonusList list;
//    //根据各种情况查询分红送配信息, 如果两个都不设定,查询对应的单独的最近的送配信息,否则就查询指定的信息并且推送出去
//    if(!mDataBase.queryShareBonus(list, code, date)) return;
//    if(code.length() == 0 && date.isNull())
//    {
//        QDateTime cur = QDateTime::currentDateTime();
//        int year = cur.date().year();
//        QStringList dateList;
//        dateList.append(QDate(year-1, 12, 31).toString("yyyy-MM-dd"));
////        dateList.append(QDate(year-1, 6, 30).toString("yyyy-MM-dd"));
//        dateList.append(QDate(year, 12, 31).toString("yyyy-MM-dd"));
//        foreach (ShareBonus data, list) {
//            if(!dateList.contains(data.mDate.toString())) continue;
//            ShareData& share = getShareData(data.mCode.right(6));
//            share.mBonusData  = data;
//        }
//    }


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
        data.mReferCodeList.append(block);
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

void HqInfoService::slotUpdateShareExchangeRecord(const QList<ShareExchangeData> &list)
{
    if(list.size() == 0) return;
    QString start_date = list.first().mDateTime;
    QString end_date = list.last().mDateTime;
    qDebug()<<__FUNCTION__<<list.size()<<start_date<<end_date;
    if(!mDataBase.updateExhangeRecordList(list))
    {
        emit signalDBErrorMsg(tr("update db exchange record error:%1").arg(mDataBase.errMsg()));
        return;
    }

    emit signalUpdateShareExchangeRecordSucceed();
}

void HqInfoService::slotDeleteShareExchangeRecord(const QString &code, const QString &start_date, const QString &end_date)
{
    if(!mDataBase.deleteExchangeRecord(code, start_date, end_date))
    {
        emit signalDBErrorMsg(tr("update db exchange record error:%1").arg(mDataBase.errMsg()));
        return;
    }

    emit signalDeleteShareExchangeRecordSucceed();
}

void HqInfoService::slotQueryShareExchangeRecord(int page, const QString &code, const QString &start_date, const QString &end_date)
{
    QList<ShareExchangeData> list;
    int total_page = -1;
    mDataBase.queryExchangeRecord(list, total_page, page, code, start_date, end_date);
    emit signalSendShareExchangeRecord(page, total_page, list);
}

