#ifndef DBSERVICE_H
#define DBSERVICE_H

#include <QObject>
#include <QThread>
#include "hqdbdatabase.h"
#include "utils/profiles.h"


enum HqInfoSysStatus{
    HQ_NotOpen = 0,
    HQ_InCharge,
    HQ_Closed,
};


#define DATA_SERVICE HqInfoService::instance()

class HqInfoService : public QObject
{
    Q_OBJECT
protected:
    HqInfoService(QObject *parent = 0);
    ~HqInfoService();

public:
    QDate  getLastUpdateDateOfTable(const QString& table);
    void        setHistoryInfoCount(int count) {mHistoryInfoCount = count;}

    QStringList getHshtTop10List() const {return mHsgtTop10Kyes;}
    int         getSystemStatus() const {return mSystemStatus;}
    void        setSystemStatus(int sts) {mSystemStatus = sts;}
public:
    bool   isDBInitOk();
    friend class CGarbo;
    static HqInfoService* instance();
    ShareData& getShareData(const QString& code);
    ShareDataList   getShareDataList(int type);
    ShareDataList getShareDataList();
    double getProfit(const QString& code);
    foreignHolder amountForeigner(const QString& code);
    QStringList  getExchangeCodeList();
    QStringList  getAllShareCodes() {return mRealShareData.keys();}
    QDate  getLastUpdateDateOfHSGT();
    QDate  getLastUpdateDateOfHSGTVol();
    QDate  getLastUpdateDateOfBasicInfo();
    QDate  getLastUpdateDateOfBonusInfo();
    QDate  getLastUpdateDateOfHsgtTop10();
    QDate  getLastUpdateDateOfFinanceInfo();
    QDate  getLastUpdateDateOfHistoryInfo();
    bool   GetHistoryInfoWithDate(const QString& table, const QDate& date, double& close, double& money, qint64& total_share, qint64& mutalble_share);
    double   GetMultiDaysChangePercent(const QString& table, int days);
    void   GetForeignVolChange(const QString& code, qint64& cur, qint64& pre);
    BlockData*   getBlockDataOfCode(const QString& code);
    //BlockData*   setBlockData(const BlockData& data);
    void         setShareBlock(const QString& code, const QString& block);
    void         setBlockShareCodes(const QString& block, const QStringList& codes);
    void         saveShares();
    BlockDataList  getAllBlock();
    ShareDataList   getShareHistoryDataList(const QString& code);
    //交易日期处理
#if 0
    bool        weekend(const QDate& date);
    bool        activeDay(const QDate& date);
    bool        isCurrentActive();
    bool        isActiveTime(const QTime& time);
    QDate       latestActiveDay();
    QDate       lastActiveDay();
    QDate       preActiveDay(const QDate& date);
    QDate       nextActiveDay(const QDate& date);
    int         activeDaysNum(const QDate& start);
    QDate       getActiveDayBefore1HYear();
    QString     date2Str(const QDate& date);
    QDate       dateFromStr(const QString& str);
#endif
    QDate       getLgtStartDate();


signals:
    //开始创建数据库需要的表信息，初始化数据库
    void signalInitDBTables();
    void signalDbInitFinished();
    void signalAllShareCodeList(const QStringList& codes);
    void signalUpdateShareBasicInfo(const ShareDataList& list);

    //从数据库查询板块信息
    void signalQueryBlockInfo(int type = 1);
    void signalRecvRealBlockInfo(const QList<BlockRealInfo>& list);
    void signalSendBlockInfoList(const QList<BlockRealInfo>& list);
     void signalQueryShareForeignVol(const QString& code);
    void signalSendShareForeignVol(const ShareDataList& list);
    //历史数据写入数据库
    void signalSendShareHistoryCloseInfo(const ShareDataList& list);
    void signalSendShareHistoryForeignVolInfo(const ShareDataList &list);
    void signalSendShareHistoryRzrqInfo(const ShareDataList &list);
    void signalSendShareHistoryZjlxInfo(const ShareDataList &list);

    void signalRecvShareHistoryInfos(const ShareDataList& list, int mode);
    void signalSendShareHistoryUpdateDate(const QDate& date, bool update);
    void signalQueryShareHistoryUpdateDateList();
    void signalSendShareHistoryUpdateDateList(const QList<QDate>& list);

    void signalQueryShareHistoryLastDate(const QString& code);
    void signalSendShareHistoryLastDate(const QString& code, const QDate& date);
    void signalUpdateHistoryInfoFinished();
    void signalUpdateShareHistoryFinished(const QString &code);
    //基本信息相关的数据库操作
    void signalQueryAllShareBasicInfo();
    void signalAddShareBasicInfo(const ShareData& data);
    void signalAddShareBasicInfoList(const ShareDataList& list);
    void signalUpdateStkProfitList(const ShareDataList& list);
    void signalInitShareRealInfos(const QStringList& codes);
    void signalUpdateShareCounter(const QList<ShareHistoryCounter>& counter);
    //自选股
    void signalSetFavCode(const QString& code);
    void signalSaveFavCode(const QString& code, bool fav);
    //沪港通持股写入数据据
    void signalAddShareAmoutByForeigner(const ShareDataList& list);
    void signalUpdateShareAmountByForeigner();

    void signalUpdateShareHsgtTop10Info(const ShareHsgtList& list);
    void signalUpdateHsgtTop10Keys(const QDate& date);
    void signalSendLastHSGTUpdateDate(const QDate& date);
    void signalQueryShareHsgtTop10List(const QString& code, const QDate& date = QDate());
    void signalSendShareHsgtTop10List(const ShareHsgtList& list, const QString& code, const QDate& date);
    void signalQueryLatestHsgtData();
    void signalSendLGTVolDataList(const QList<ShareForeignVolCounter>& list, const QString& date );
    //实时数据查询信息获取
    void signalSearchCodesOfText(const QString& text);
    void signalSendSearchCodesOfText(const QStringList& codes);
    //错误信息输出
    void signalDBErrorMsg(const QString& msg);
    //财务数据
    void signalUpdateShareFinanceInfo(const FinancialDataList& list);
    void signalQueryShareFinanceInfo(const QStringList& list = QStringList());
    //分红数据
    void signalUpdateShareBonusInfo(const ShareBonusList& list);
    void signalQueryShareFHSP(const QString& code = QString(), const QDate& date = QDate());

    void signalUpdateShareCloseDate(const QList<QDate>& list );

    void signalUpdateShareExchangeRecord(const QList<ShareExchangeData>& list);
    void signalUpdateShareExchangeRecordSucceed();
    void signalSendShareExchangeRecord(int cur_page, int total_page, const QList<ShareExchangeData>& list);
    void signalQueryShareExchangeRecord(int cur_page, const QString& code, const QString& start_date,  const QString& end_date);
    void signalDeleteShareExchangeRecord(const QString& code, const QString& start_date,  const QString& end_date);
    void signalDeleteShareExchangeRecordSucceed();
    void signalUpdateShareHsgtCounter(const ShareHsgtList& list);

    void signalSendForeignDataList(const QList<ShareForeignVolCounter>&, const QString& );

public slots:
    void slotSearchCodesOfText(const QString &text);
    void slotSetFavCode(const QString& code);
    void slotSaveFavCode(const QString& code, bool fav);
    void slotInitDBTables();
    void slotUpdateShareCloseDate(const QList<QDate>& list);
    void slotUpdateShareHsgtCounter(const ShareHsgtList&list);
    //历史数据
    void slotRecvShareHistoryInfos(const ShareDataList& list, int mode);
    void slotSendShareHistoryUpdateDate(const QDate& date, bool update);
    void slotQueryShareHistoryUpdateDateList();
//    void slotSendShareHistoryCloseInfo(const ShareDataList& list);
//    void slotSendShareHistoryForeignVolInfo(const ShareDataList &list);
//    void slotSendShareHistoryRzrqInfo(const ShareDataList &list);
//    void slotSendShareHistoryZjlxInfo(const ShareDataList &list);

    //void slotUpdateShareHistoryInfos(const QMap<QString, ShareDataList> map);
    bool slotAddHistoryData(const ShareData& data);
    void initBlockData(int type = 0);
    void initShareData(bool send);
//    void slotQueryShareHistoryLastDate(const QString& code);
    void slotUpdateShareCounter(const QList<ShareHistoryCounter>& counter);
    void slotUpdateHistoryChange(const QString& code);
    void slotUpdateStkProfitList(const ShareDataList& list);
    void slotAddShareAmoutByForeigner(const ShareDataList& list);
    void slotUpdateShareAmountByForeigner();
    void slotUpdateShareBasicInfo(const ShareDataList& list);
    void slotUpdateShareBonusInfo(const ShareBonusList& list);
    void slotUpdateHsgtTop10Info(const ShareHsgtList& list);
    void slotUpdateShareFinanceInfo(const FinancialDataList& list);
    void slotUpdateHsgtTop10Keys(const QDate& date);
    void slotQueryLatestHsgtData();
    //查询
    void slotQueryShareHsgtTop10List(const QString& code, const QDate& date);
    void slotQueryShareFinanceList(const QStringList& list = QStringList());
    void slotQueryShareFHSP(const QString& code, const QDate& date);

    //
    void slotUpdateShareExchangeRecord(const QList<ShareExchangeData>& list);
    void slotQueryShareExchangeRecord(int cur_page, const QString& code, const QString& start_date,  const QString& end_date);
    void slotDeleteShareExchangeRecord(const QString& code, const QString& start_date,  const QString& end_date);
    //
    void slotRecvForeignCounterList(const QList<ShareForeignVolCounter>& list, const QString& date );

private:
    void initSignalSlot();
    bool createHistoryTable(const QString& pTableName);
    //bool createShareBaseInfoTable(const QString& code);
    //bool createBlockTable();
    bool createHSGTShareAmountTable();
    bool isActive();
    QString errMsg();

private:    //本类使用的变量
    static HqInfoService *m_pInstance;
    static QMutex mutex;//实例互斥锁。
    //static QAtomicPointer<HqInfoService> m_pInstance;/*!<使用原子指针,默认初始化为0。*/

    class CGarbo        // 它的唯一工作就是在析构函数中删除CSingleton的实例
    {
    public:
        ~CGarbo()
        {
            if (HqInfoService::m_pInstance)
            {
                delete HqInfoService::m_pInstance;
                HqInfoService::m_pInstance = NULL;
            }
        }
    };
    static CGarbo s_Garbo; // 定义一个静态成员，在程序结束时，系统会调用它的析构函数
    QThread             m_threadWork;       //工作线程
    QStringList                 mNotExchangeDaysList;
    ShareDataMap                mRealShareData;
    QDate                       mLast3DaysDate;
    QDate                       mLast5DaysDate;
    QDate                       mLast10DaysDate;
    QDate                       mLast1MonthDate;
    QDate                       mLastActiveDate;
    QMap<QString, double>       mStkProfitMap;
    QMap<QString, foreignHolder>       mStkForeignerHoldMap;
    BlockDataMap                mRealBlockMap;
    QMutex                      mBlockMutex;
    QMutex                      mShareMutex;
    HQDBDataBase                mDataBase;
    QStringList                 mFavCodeList;
    QStringList                 mClosedDateList;
    int                         mHistoryInfoCount;
    //QList<QDate>                mShareCloseDateList;
    QStringList                 mHsgtTop10Kyes;
    QDate                       mHsgtDate;
    int                         mSystemStatus;
};

#endif // DBSERVICE_H
