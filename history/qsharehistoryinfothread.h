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
    explicit QShareHistoryInfoThread(const QString& code, const QMap<QDate, ShareForignVolFileData>& list, bool counter = true, QObject* parent = 0);
    ~QShareHistoryInfoThread();
    QString getCode();
    void    setConuter(bool sts) {mCounter = sts;}
public:
    void run();
private:
    void                        adjustDataList(ShareDailyDataList& list, double price, double ratio);
    bool                        readFile(ShareDailyDataList& list, bool& adjust);
    QString                     getFileName();
    QString                     getPath();
    void                        writeFile(const ShareDailyDataList& list, const char* mode);
private:
    QString         mCode;
    QObject         *mParent;
    bool            mCounter;
    QMap<QDate, ShareForignVolFileData>      mForeignList;
};

#endif // QSHAREHISTORYINFOTHREAD_H


//http://nufm.dfcfw.com/EM_Finance2014NumericApplication/JS.aspx?type=CT&cmd=P.[(x)]|3000592&sty=MPICT&st=z&sr=&p=&ps=&cb=callback&js=&token=aaf32a88de888ea7b0ea63e017ecb049&_=1481245370248
