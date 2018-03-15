#ifndef HQTASKMAGRCENTER_H
#define HQTASKMAGRCENTER_H

#include <QObject>
#include <QThread>
#include "utils/sharedata.h"

class QSinaStkResultMergeThread;
class QEastMoneyBlockMangagerThread;

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
    void        signalBlockDataListUpdated(const BlockDataVList& list);
    void        signalSearchCodesOfText(const QString& input);
    void        signalSetFavCode(const QString& code);
    void        signalUpdateAllShareHistoryFromDate(bool deldb, const QDate& date);

public slots:
    void        slotStart();
    void        slotDBInitFinished();
    void        slotBaseDataListFinished(const QStringList& codes, const ShareBaseDataList& list);
    void        slotUpdateHistoryFinished();

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
private:
    QThread                     mWorkThread;
    QList<QObject*>             mRealWorkObjList;
    QSinaStkResultMergeThread*   mShareInfoMergeThread;
    QEastMoneyBlockMangagerThread*  mBlockMgr;
};

#endif // HQTASKMAGRCENTER_H
