#ifndef HQTASKMAGRCENTER_H
#define HQTASKMAGRCENTER_H

#include <QObject>
#include <QThread>
#include "data_structure/sharedata.h"
#include "data_structure/hqblockdata.h"


class QSinaStkResultMergeThread;
class QEastMoneyBlockMangagerThread;
class QShareBasicInfoWorker;
class QShareHistoryInfoMgr;
class QSinaStkInfoThread;
class QShareActiveDateUpdateThread;
class HqKuaixun;
class QShareForeignVolChangeCounterThread;

class HQTaskMagrCenter : public QObject
{
    Q_OBJECT
protected:
    explicit HQTaskMagrCenter(QObject *parent = 0);
public:
    static HQTaskMagrCenter* instance();
    QShareForeignVolChangeCounterThread* getForeignVolThread() {return mForeignVolThread;}

public:
    ~HQTaskMagrCenter();

    void  registerInfoDisplaywidget(QWidget* w);
    QSinaStkResultMergeThread*  hqCenter() {return mHqCenter;}
signals:
    void        signalUpdateHistoryMsg(const QString& msg);
    void        signalUpdateHistoryFinished();
    void        signalSendIndexCodesList(const QStringList& list);
    void        signalSendIndexRealDataList(const ShareDataList& list);
    void        signalSendShareRealDataList(const ShareDataList& list);
    void        signalSendNotrhBoundDataList(const ShareHsgtList& list);
    void        signalBlockDataListUpdated(const BlockDataList& list);
    void        signalSearchCodesOfText(const QString& input);
    void        signalSetFavCode(const QString& code);
    void        signalUpdateAllShareHistoryFromDate(bool deldb, const QDate& date);
    void        signalNewHsgtTop10Now();
    void        signalWorkingDayfinished();
    void        signalCurSystemInfo(qint64 time, int sts);
    void        signalSendAllShareCodesList(const QStringList& list);

public slots:
    void        slotStart();
    void        slotFinishNetworkCheck(bool sts);
    void        slotDBInitFinished();
    void        slotFinishUpdateWorkDays();
    void        slotNewWorDayChangeNow();

    void        slotShareCodesListFinished(const QStringList& codes);
    void        slotUpdateHistoryFinished();
    void        slotSetFavCode(const QString& code);
    void        slotUpdateHSGTOfCode(const QString& code);
    void        reverseSortRule();
    void        setCurBlockType(int type);
    void        addSpecialConcern(const QString& code);
private:    //本类使用的变量
    static HQTaskMagrCenter *m_pInstance;

    class CGarbo        // 它的唯一工作就是在析构函数中删除CSingleton的实例
    {
    public:
        ~CGarbo()
        {
            if (HQTaskMagrCenter::m_pInstance)
            {
                delete HQTaskMagrCenter::m_pInstance;
                HQTaskMagrCenter::m_pInstance = NULL;
            }
        }
    };
    static CGarbo s_Garbo; // 定义一个静态成员，在程序结束时，系统会调用它的析构函数

private:
    QThread                     mWorkThread;
    QShareHistoryInfoMgr                *mHistoryInfoMgr;
    QShareActiveDateUpdateThread*       mTimeMonitorThread;
    QSinaStkResultMergeThread*          mHqCenter;
    HqKuaixun*  mInfoThread724;
    QShareForeignVolChangeCounterThread  *mForeignVolThread;

};

#endif // HQTASKMAGRCENTER_H
