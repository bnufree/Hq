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
    ShareWorkingDate  getLastUpdateDateOfTable(const QString& table);
    void        setHistoryInfoCount(int count) {mHistoryInfoCount = count;}

    QStringList getHshtTop10List() const {return mHsgtTop10Kyes;}
    int         getSystemStatus() const {return mSystemStatus;}
    void        setSystemStatus(int sts) {mSystemStatus = sts;}
public:
    bool   isDBInitOk();
    friend class CGarbo;
    static HqInfoService* instance();
    ShareData* getShareData(const QString& code);
    ShareDataList getShareDataList();
    double getProfit(const QString& code);
    foreignHolder amountForeigner(const QString& code);
    QStringList  getExchangeCodeList();
    QStringList  getAllShareCodes() {return mRealShareData.keys();}
    ShareWorkingDate  getLastUpdateDateOfHSGT();
    ShareWorkingDate  getLastUpdateDateOfHSGTVol();
    ShareWorkingDate  getLastUpdateDateOfBasicInfo();
    ShareWorkingDate  getLastUpdateDateOfBonusInfo();
    ShareWorkingDate  getLastUpdateDateOfHsgtTop10();
    ShareWorkingDate  getLastUpdateDateOfFinanceInfo();
    ShareWorkingDate  getLastUpdateDateOfHistoryInfo();
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
    void signalSendShareHistoryUpdateDate(const ShareWorkingDate& date, bool update);
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
    void signalUpdateShareinfoWithHistory(const QString& code,\
                                          double lastMoney,\
                                          double last3Change,\
                                          double last5Change,\
                                          double last10Change,\
                                          double lastMonthChange,\
                                          double lastYearChange,\
                                          qint64 vol,\
                                          qint64 vol_chnage,\
                                          const ShareDataList& list);
    void signalUpdateStkProfitList(const ShareDataList& list);
    void signalInitShareRealInfos(const QStringList& codes);
    void signalUpdateShareCounter(const ShareHistoryCounter& counter);
    //自选股
    void signalSetFavCode(const QString& code);
    void signalSaveFavCode(const QString& code, bool fav);
    //沪港通持股写入数据据
    void signalAddShareAmoutByForeigner(const ShareDataList& list);
    void signalUpdateShareAmountByForeigner();

    void signalUpdateShareHsgtTop10Info(const ShareHsgtList& list);
    void signalUpdateHsgtTop10Keys(const ShareWorkingDate& date);
    void signalSendLastHSGTUpdateDate(const ShareWorkingDate& date);
    void signalQueryShareHsgtTop10List(const QString& code, const ShareWorkingDate& date = ShareWorkingDate());
    void signalSendShareHsgtTop10List(const ShareHsgtList& list);

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
    void signalQueryShareFHSP(const QString& code = QString(), const ShareWorkingDate& date = ShareWorkingDate());

    void signalUpdateShareCloseDate(const QList<QDate>& list );

public slots:
    void slotSearchCodesOfText(const QString &text);
    void slotSetFavCode(const QString& code);
    void slotSaveFavCode(const QString& code, bool fav);
    void slotInitDBTables();
    void slotUpdateShareCloseDate(const QList<QDate>& list);
    //历史数据
    void slotRecvShareHistoryInfos(const ShareDataList& list, int mode);
    void slotSendShareHistoryUpdateDate(const ShareWorkingDate& date, bool update);
    void slotQueryShareHistoryUpdateDateList();
//    void slotSendShareHistoryCloseInfo(const ShareDataList& list);
//    void slotSendShareHistoryForeignVolInfo(const ShareDataList &list);
//    void slotSendShareHistoryRzrqInfo(const ShareDataList &list);
//    void slotSendShareHistoryZjlxInfo(const ShareDataList &list);

    //void slotUpdateShareHistoryInfos(const QMap<QString, ShareDataList> map);
    bool slotAddHistoryData(const ShareData& data);
    void initBlockData(int type = 0);
    void initShareData();
//    void slotQueryShareHistoryLastDate(const QString& code);
    void slotUpdateShareinfoWithHistory(const QString& code,\
                                        double lastMoney,\
                                        double last3Change,\
                                        double last5Change,\
                                        double last10Change,\
                                        double lastMonthChange,\
                                        double lastYearChange,\
                                        qint64 vol,\
                                        qint64 vol_chnage,\
                                        const ShareDataList& list);
    void slotUpdateShareCounter(const ShareHistoryCounter& counter);
    void slotUpdateHistoryChange(const QString& code);
    void slotUpdateStkProfitList(const ShareDataList& list);
    void slotAddShareAmoutByForeigner(const ShareDataList& list);
    void slotUpdateShareAmountByForeigner();
    void slotUpdateShareBasicInfo(const ShareDataList& list);
    void slotUpdateShareBonusInfo(const ShareBonusList& list);
    void slotUpdateHsgtTop10Info(const ShareHsgtList& list);
    void slotUpdateShareFinanceInfo(const FinancialDataList& list);
    void slotUpdateHsgtTop10Keys(const ShareWorkingDate& date);
    //查询
    void slotQueryShareHsgtTop10List(const QString& code, const ShareWorkingDate& date);
    void slotQueryShareFinanceList(const QStringList& list = QStringList());
    void slotQueryShareFHSP(const QString& code, const ShareWorkingDate& date);

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
