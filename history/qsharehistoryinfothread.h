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
    explicit QShareHistoryInfoThread(const QString& code, const QDate& date,QObject* parent = 0);
    ~QShareHistoryInfoThread();
    QString getCode();
public:
    void run();
private:
    QString         mCode;
    QDate           mStartDate;
    QObject         *mParent;
};

#endif // QSHAREHISTORYINFOTHREAD_H


//http://nufm.dfcfw.com/EM_Finance2014NumericApplication/JS.aspx?type=CT&cmd=P.[(x)]|3000592&sty=MPICT&st=z&sr=&p=&ps=&cb=callback&js=&token=aaf32a88de888ea7b0ea63e017ecb049&_=1481245370248
