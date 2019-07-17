#ifndef QSHAREHISTORYCOUNTERWORK_H
#define QSHAREHISTORYCOUNTERWORK_H

#include <QRunnable>
#include <QObject>
#include "data_structure/sharedata.h"

class QShareHistoryCounterWork : public QRunnable
{
public:
    QShareHistoryCounterWork(const QString& code, const ShareHistoryFileDataList& list = ShareHistoryFileDataList(), QObject* parent = 0);
    ~QShareHistoryCounterWork();
    void run();
    QString getFileName();
    bool readFile(ShareHistoryFileDataList& list);
private:
    ShareHistoryFileDataList mList;
    QObject*    mParent;
    QString     mCode;
};

#endif // QSHAREHISTORYCOUNTERWORK_H
