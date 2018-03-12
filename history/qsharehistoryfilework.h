#ifndef QSHAREHISTORYFILEWORK_H
#define QSHAREHISTORYFILEWORK_H

#include <QRunnable>
#include <QObject>
#include "utils/sharedata.h"

#define     FILE_READ           0
#define     FILE_WRITE          1

class QShareHistoryFileWork : public QRunnable
{
public:
    explicit QShareHistoryFileWork(int mode, const QString& fileName, \
                                    const ShareDataList& list = ShareDataList(), QObject* parent = 0);
    ~QShareHistoryFileWork();
public:
    void run();
private:
    void write();
    void read();
private:
    int           mMode;
    ShareDataList   mHistoryList;
    QObject         *mParent;
    QString         mFileName;
};

#endif // QSHAREHISTORYFILEWORK_H
