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
    void slotUpdateRecordSucceed();
private:
    bool    isSerialNumCol(const QString& title);
    bool    isAccountCol(const QString& title);
    bool    isTotalCountCol(const QString& title);
    bool    isNetCol(const QString& title);
    bool    isOtherCol(const QString& title);
    bool    isStamptaxCol(const QString& title);
    bool    isBrokerageCol(const QString& title);
    bool    isDateCol(const QString& title);
    bool    isTypeCol(const QString& title);
    bool    isCodeCol(const QString& title);
    bool    isNameCol(const QString& title);
    bool    isNumberCol(const QString& title);
    bool    isPriceCol(const QString& title);
    bool    isMoneyCol(const QString& title);
    int     parseTypeOfString(const QString& type);
private:
    QThread         mWorkThread;
    QMap<int, int>  mColMap;
};

#endif // QEXCHANGERECORDWORKER_H
