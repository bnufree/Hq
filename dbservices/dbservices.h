#ifndef DBSERVICE_H
#define DBSERVICE_H

#include <QObject>
#include <QThread>
#include "hqdbdatabase.h"


#define DATA_SERVICE HqInfoService::instance()

class HqInfoService : public QObject
{
    Q_OBJECT
protected:
    HqInfoService(QObject *parent = 0);
    ~HqInfoService();

public:
    bool   isDBInitOk();
    friend class CGarbo;
    static HqInfoService* instance();
    StockData* getBasicStkData(const QString& code);
    double getProfit(const QString& code);
    foreignHolder amountForeigner(const QString& code);
    QStringList  getExchangeCodeList();
    QDate  getLastUpdateDateOfHSGT();
    QDate  getLastUpdateDateOfHSGTVol();
    QDate  getLastUpdateDateOfShareHistory(const QString& code);
    bool   GetHistoryInfoWithDate(const QString& table, const QDate& date, double& close, double& money, qint64& total_share, qint64& mutalble_share);
    double   GetMultiDaysChangePercent(const QString& table, int days);
    void   GetForeignVolChange(const QString& code, qint64& cur, qint64& pre);
    BlockData*   getBlockDataOfCode(const QString& code);
    void         setBlockData(BlockData* data);
    void         setShareBlock(const QString& code, const QString& block);

signals:
    //开始创建数据库需要的表信息
    void signalCreateDBTables();
    void signalCreateDBTablesFinished(bool sts, const QString& errMsg);
    //从数据库查询板块信息
    void signalQueryBlockInfo(int type = 1);
    void signalRecvRealBlockInfo(const QList<BlockRealInfo>& list);
    void signalSendBlockInfoList(const QList<BlockRealInfo>& list);
    void signalSendTop10ChinaStockInfos(const QList<ChinaShareExchange>& list);
    void signalRecvTop10ChinaStockInfos(const QList<ChinaShareExchange>& list);
    void signalQueryTop10ChinaStockInfos(const QDate& date = QDate(), const QString& share = QString(), int market = 0);
    //历史数据写入数据库
    void signalRecvShareHistoryInfos(const StockDataList& list);
    void signalQueryShareHistoryLastDate(const QString& code);
    void signalSendShareHistoryLastDate(const QString& code, const QDate& date);
    //基本信息相关的数据库操作
    void signalQueryAllShareBasicInfo();
    void signalAddShareBasicInfo(const StockData& data);
    void signalAddShareBasicInfoList(const StockDataList& list);
    void signalUpdateStkBaseinfoWithHistory(const QString& code);
    void signalUpdateStkBaseinfoWithHistoryFinished(const QString &code);
    void signalUpdateStkProfitList(const StockDataList& list);
    void signalInitStockRealInfos(const QStringList& codes);
    //沪港通持股写入数据据
    void signalAddShareAmoutByForeigner(const StockDataList& list);
    void signalUpdateShareAmountByForeigner();
public slots:
    void slotCreateDBTables();
    void slotRecvShareHistoryInfos(const StockDataList& list);
    //void slotUpdateShareHistoryInfos(const QMap<QString, StockDataList> map);
    bool slotAddHistoryData(const StockData& data);
    void initBlockData(int type = 0);
    void initShareData();
    void slotRecvTop10ChinaStockInfos(const QList<ChinaShareExchange>& list);
    void slotQueryTop10ChinaStockInfos(const QDate& date = QDate(), const QString& share = QString(), int market = 0);
    void slotQueryShareHistoryLastDate(const QString& code);
    void slotQueryAllShareBasicInfo();
    bool slotAddShareBasicInfo(const StockData& data);
    void slotAddShareBasicInfoList(const StockDataList& list);
    void slotUpdateStkBaseinfoWithHistory(const QString& code);
    void slotUpdateHistoryChange(const QString& code);
    void slotUpdateStkProfitList(const StockDataList& list);
    void slotAddShareAmoutByForeigner(const StockDataList& list);
    void slotUpdateShareAmountByForeigner();
    void slotUpdateStockRealInfos(const QStringList& list);


private:
    void initHistoryDates();
    void initSignalSlot();
    bool createHistoryTable(const QString& pTableName);
    //bool createStockBaseInfoTable(const QString& code);
    //bool createBlockTable();
    bool createHSGTShareAmountTable();
    bool isActive();
    QString errMsg();
    //数据库查询指定日期的沪深股通的TOP10交易
    bool queryTop10ChinaShareInfos(QList<ChinaShareExchange>& list, const QDate& date = QDate(), const QString& share = QString(), int market = 0);
    bool addTop10ChinaStockInfo(const ChinaShareExchange& info);

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
    QMap<QString, StockData*>    mStkRealInfo;
    QDate                       mLast3DaysDate;
    QDate                       mLast5DaysDate;
    QDate                       mLast10DaysDate;
    QDate                       mLast1MonthDate;
    QDate                       mLastActiveDate;
    QMap<QString, double>       mStkProfitMap;
    QMap<QString, foreignHolder>       mStkForeignerHoldMap;
    QMap<QString,   BlockData*> mBlockDataMap;
    QMap<QString,   QStringList> mShareBlockMap;
    QMutex                      mBlockMutex;
    QMutex                      mShareMutex;
    HQDBDataBase                mDataBase;
};

#endif // DBSERVICE_H
