#ifndef QSHAREHISTORYCOUNTERWORK_H
#define QSHAREHISTORYCOUNTERWORK_H

#include <QRunnable>
#include <QObject>
#include "utils/sharedata.h"

class QShareHistoryCounterWork : public QRunnable
{
public:
    QShareHistoryCounterWork(const QString& code,const ShareDataList& list, QObject* parent = 0);
    ~QShareHistoryCounterWork();
    void run();
private:
    ShareDataList mList;
    QObject*    mParent;
    QString     mCode;
};

#endif // QSHAREHISTORYCOUNTERWORK_H
