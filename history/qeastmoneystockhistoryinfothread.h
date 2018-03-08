#ifndef QEASTMONEYShareHISTORYINFOTHREAD_H
#define QEASTMONEYShareHISTORYINFOTHREAD_H

#include <QStringList>
#include <QDate>
#include <QRunnable>
#include <QObject>
#include "utils/sharedata.h"

class QEastmoneyShareHistoryInfoThread : public QRunnable
{
public:
    explicit QEastmoneyShareHistoryInfoThread(const QString& code, const QDate& date, const QString& dir, bool deldb, ShareDataList* list = 0, QObject* parent = 0);
    ~QEastmoneyShareHistoryInfoThread();
    QString getCode();
public:
    void run();
private:
    QString         mCode;
    QDate           mStartDate;
    ShareDataList*   mHistoryListPtr;
    QObject         *mParent;
    QString         mSaveDir;
    bool            mDelDB;
};

#endif // QEASTMONEYShareHISTORYINFOTHREAD_H


//http://nufm.dfcfw.com/EM_Finance2014NumericApplication/JS.aspx?type=CT&cmd=P.[(x)]|3000592&sty=MPICT&st=z&sr=&p=&ps=&cb=callback&js=&token=aaf32a88de888ea7b0ea63e017ecb049&_=1481245370248
