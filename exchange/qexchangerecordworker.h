#ifndef QEXCHANGERECORDWORKER_H
#define QEXCHANGERECORDWORKER_H

#include <QObject>
#include <QThread>
#include "stockdata.h"
class QExchangeRecordWorker : public QObject
{
    Q_OBJECT
public:
    explicit QExchangeRecordWorker(QObject *parent = 0);

signals:
    void signalStartImport(const QString& file);
    void signalSendStkProfitList(const StockDataList& list);
    void signalSendCodeList(const QStringList& list);
public slots:
    void slotStartImport(const QString& sFilePathName);
private:
    QThread         mWorkThread;
};

#endif // QEXCHANGERECORDWORKER_H
