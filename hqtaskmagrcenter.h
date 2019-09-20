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
public:
    explicit HQTaskMagrCenter(QObject *parent = 0);
    ~HQTaskMagrCenter();

signals:
    void        signalStart();
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
    void        slotStart();
    void        slotDBInitFinished();
    void        slotFinishUpdateWorkDays();
    void        slotNewWorDayChangeNow();

    void        slotShareCodesListFinished(const QStringList& codes);
    void        slotBaseDataListFinished(const QStringList& codes, const ShareDataList& list);
    void        slotUpdateHistoryFinished();
    void        slotSetFavCode(const QString& code);
    void        slotStartUpdateHSGTTop10(const ShareWorkingDate& date);

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
private:
    QThread                     mWorkThread;
    QList<QObject*>             mRealWorkObjList;
    QSinaStkResultMergeThread*   mShareInfoMergeThread;
    QEastMoneyBlockMangagerThread*  mBlockMgr;
    QShareActiveDateUpdateThread        *mWorkDayTimeMonitorThread;
    QShareHistoryInfoMgr                *mHistoryInfoMgr;
    QSinaStkInfoThread                  *mIndexThread;

};

#endif // HQTASKMAGRCENTER_H
