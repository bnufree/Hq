#ifndef SHAREHISTORYREADTHREAD_H
#define SHAREHISTORYREADTHREAD_H

#include <QThread>
#include "data_structure/sharedata.h"

class ShareHistoryReadThread : public QThread
{
    Q_OBJECT
public:
    explicit ShareHistoryReadThread(const QString& code, const QDate& date = QDate(), QObject *parent = 0);
protected:
    void run();

signals:
    void signalSendGraphicDataList(const GRAPHIC_DATA_LIST& list);

signals:

public slots:
private:
    QString mCode;
    QDate   mDate;
};

#endif // SHAREHISTORYREADTHREAD_H
