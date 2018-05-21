#ifndef QSHAREHISTORYINFOTHREAD_H
#define QSHAREHISTORYINFOTHREAD_H

#include <QStringList>
#include <QDate>
#include <QRunnable>
#include <QObject>
#include "utils/sharedata.h"

class QShareHistoryInfoThread : public QRunnable
{
public:
    explicit QShareHistoryInfoThread(const QString& code, const QMap<qint64, qint64>& foreign_map, QObject* parent = 0);
    ~QShareHistoryInfoThread();
    QString getCode();
public:
    void run();
private:
    QDate lastUpdateDate();
    bool  write(const QList<SHARE_HISTORY_INFO>& list);
private:
    QString         mCode;
    QMap<qint64, qint64> mForeignMap;
    QObject         *mParent;
    QString         mFileName;
};

#endif // QSHAREHISTORYINFOTHREAD_H


//http://nufm.dfcfw.com/EM_Finance2014NumericApplication/JS.aspx?type=CT&cmd=P.[(x)]|3000592&sty=MPICT&st=z&sr=&p=&ps=&cb=callback&js=&token=aaf32a88de888ea7b0ea63e017ecb049&_=1481245370248
