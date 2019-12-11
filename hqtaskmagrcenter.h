#ifndef HQTASKMAGRCENTER_H
#define HQTASKMAGRCENTER_H

#include <QObject>
#include <QThread>
#include "data_structure/sharedata.h"
#include "data_structure/hqblockdata.h"


class QSinaStkResultMergeThread;
class QEastMoneyBlockMangagerThread;
class QShareBasicInfoWorker;
class QShareActiveDateUpdateThread;
class QShareHistoryInfoMgr;
class QSinaStkInfoThread;

class HQTaskMagrCenter : public QObject
{
    Q_OBJECT
protected:
    explicit HQTaskMagrCenter(QObject *parent = 0);
    static HQTaskMagrCenter* instance();
    QShareActiveDateUpdateThread* activeDateThread() {return mWorkDayTimeMonitorThread;}
public:
    ~HQTaskMagrCenter();
    void start();

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

public slots:
    void        slotDBInitFinished();
    void        slotFinishUpdateWorkDays();
    void        slotNewWorDayChangeNow();

    void        slotShareCodesListFinished(const QStringList& codes);
    void        slotUpdateHistoryFinished();
    void        slotSetFavCode(const QString& code);
    //
    void        setMktType(int type);
    void        setSortType(int type);
    void        setActive(bool active);
    void        setSelfCodesList(const QStringList& list );
    void        setDisplayPage(int val);
    void        slotUpdateHSGTOfCode(const QString& code);
    void        reverseSortRule();
    void        setCurBlockType(int type);
    void        setDisplayChinaTop10();
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
    QList<QObject*>             mRealWorkObjList;
    QSinaStkResultMergeThread*   mShareInfoMergeThread;
    QEastMoneyBlockMangagerThread*  mBlockMgr;
    QShareActiveDateUpdateThread        *mWorkDayTimeMonitorThread;
    QShareHistoryInfoMgr                *mHistoryInfoMgr;

};

#endif // HQTASKMAGRCENTER_H
