#ifndef QSHAREHISTORYINFOTHREAD_H
#define QSHAREHISTORYINFOTHREAD_H

#include <QStringList>
#include <QDate>
#include <QRunnable>
#include <QObject>
#include "data_structure/sharedata.h"
#include <QMap>

class QShareHistoryInfoThread : public QRunnable
{
public:
    explicit QShareHistoryInfoThread(const QString& code, const ShareWorkingDate& start, const ShareWorkingDate& end, QObject* parent = 0);
    explicit QShareHistoryInfoThread(const QString& code, QMap<QDate, ShareForignVolFileDataList>* map, QObject* parent = 0);
    ~QShareHistoryInfoThread();
    QString getCode();
public:
    void run();
private:
    void                        adjustDataList(ShareHistoryFileDataList& list, double price, double ratio);
    bool                        readFile(ShareHistoryFileDataList& list);
    QString                     getFileName();
    void                        writeFile(const ShareHistoryFileDataList& list, const char* mode);
private:
    QString         mCode;
    QObject         *mParent;
    ShareWorkingDate       mStart;
    ShareWorkingDate       mEnd;
    QMap<QDate, ShareForignVolFileDataList> *mExistForeignMap;
};

#endif // QSHAREHISTORYINFOTHREAD_H


//http://nufm.dfcfw.com/EM_Finance2014NumericApplication/JS.aspx?type=CT&cmd=P.[(x)]|3000592&sty=MPICT&st=z&sr=&p=&ps=&cb=callback&js=&token=aaf32a88de888ea7b0ea63e017ecb049&_=1481245370248
