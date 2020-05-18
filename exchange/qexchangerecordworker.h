#ifndef QEXCHANGERECORDWORKER_H
#define QEXCHANGERECORDWORKER_H

#include <QObject>
#include <QThread>
#include "data_structure/sharedata.h"


enum Col{
    Date = 0,
    Code,
    Name,
    Type,
    Number,
    Price,
    Money,
    Brokerage,
    StampTax,
    Other,
    Net,
    TotalCount,
    Account,
    SerialNum,
    Reserved,
};

class QExchangeRecordWorker : public QObject
{
    Q_OBJECT
public:
    explicit QExchangeRecordWorker(QObject *parent = 0);


signals:
    void signalStartImport(const QString& file);
    void signalSendCodeList(const QStringList& list);
public slots:
    void slotStartImport(const QString& sFilePathName);
private:
    QThread         mWorkThread;
    QMap<int, int>  mColMap;
};

#endif // QEXCHANGERECORDWORKER_H
