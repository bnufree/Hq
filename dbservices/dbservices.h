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

    QDate  getLastUpdateDateOfTable(const QString& table);
public:
    bool   isDBInitOk();
    friend class CGarbo;
    static HqInfoService* instance();
    ShareData* getBasicStkData(const QString& code);
    double getProfit(const QString& code);
    foreignHolder amountForeigner(const QString& code);
    QStringList  getExchangeCodeList();
    QDate  getLastUpdateDateOfHSGT();
    QDate  getLastUpdateDateOfHSGTVol();
    QDate  getLastUpdateDateOfBasicInfo();
    QDate  getLastUpdateDateOfHistoryInfo();
    bool   GetHistoryInfoWithDate(const QString& table, const QDate& date, double& close, double& money, qint64& total_share, qint64& mutalble_share);
    double   GetMultiDaysChangePercent(const QString& table, int days);
    void   GetForeignVolChange(const QString& code, qint64& cur, qint64& pre);
    BlockData*   getBlockDataOfCode(const QString& code);
    void         setBlockData(BlockData* data);
    void         setShareBlock(const QString& code, const QString& block);
    void         saveShares();

signals:
    //开始创建数据库需要的表信息，初始化数据库
    void signalInitDBTables();
    void signalDbInitFinished();
    void signalUpdateShareCodesList(const QStringList& codes);
    void signalUpdateShareBasicInfo(const ShareBaseDataList& list);

    //从数据库查询板块信息
    void signalQueryBlockInfo(int type = 1);
    void signalRecvRealBlockInfo(const QList<BlockRealInfo>& list);
    void signalSendBlockInfoList(const QList<BlockRealInfo>& list);
     void signalQueryShareForeignVol(const QString& code);
    void signalSendShareForeignVol(const ShareDataList& list);
    //历史数据写入数据库
    void signalRecvAllShareHistoryInfos(const ShareDataList& list, bool deletedb);
    void signalRecvShareHistoryInfos(const QString& code, const ShareDataList& list, bool deletedb);
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
                                          qint64 vol_chnage);
    void signalUpdateStkProfitList(const ShareDataList& list);
    void signalInitShareRealInfos(const QStringList& codes);
    void signalSetFavCode(const QString& code);
    //沪港通持股写入数据据
    void signalAddShareAmoutByForeigner(const ShareDataList& list);
    void signalUpdateShareAmountByForeigner();
    //实时数据查询信息获取
    void signalSearchCodesOfText(const QString& text);
    void signalSendSearchCodesOfText(const QStringList& codes);

public slots:
    void slotSearchCodesOfText(const QString &text);
    void slotInitDBTables();
    void slotUpdateShareCodesList(const QStringList& list);
    void slotRecvAllShareHistoryInfos(const ShareDataList& list, bool deletedb);
    void slotRecvShareHistoryInfos(const QString& code, const ShareDataList& list, bool deleteDB);
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
                                        qint64 vol_chnage);
    void slotUpdateHistoryChange(const QString& code);
    void slotUpdateStkProfitList(const ShareDataList& list);
    void slotAddShareAmoutByForeigner(const ShareDataList& list);
    void slotUpdateShareAmountByForeigner();
    void slotUpdateShareBasicInfo(const ShareBaseDataList& list);
    void slotSetFavCode(const QString& code);
    void slotQueryShareForeignVol(const QString& code);

private:
    void initHistoryDates();
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
    QMap<QString, ShareData*>    mStkRealInfo;
    QDate                       mLast3DaysDate;
    QDate                       mLast5DaysDate;
    QDate                       mLast10DaysDate;
    QDate                       mLast1MonthDate;
    QDate                       mLastActiveDate;
    QMap<QString, double>       mStkProfitMap;
    QMap<QString, foreignHolder>       mStkForeignerHoldMap;
    QMap<QString,   BlockData*> mBlockDataMap;
    QMutex                      mBlockMutex;
    QMutex                      mShareMutex;
    HQDBDataBase                mDataBase;
};

#endif // DBSERVICE_H
