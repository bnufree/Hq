#ifndef QEXCHANGERECORDWORKER_H
#define QEXCHANGERECORDWORKER_H

#include <QObject>
#include <QThread>
#include "data_structure/sharedata.h"
#include "xlsxdocument.h"


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

struct ShareExchangeDataMgr
{
    QString     mCode;
    QString     mName;
    double      mProfit;
    double      mVol;
    QList<ShareExchangeData> mList;     //记录数据

    ShareExchangeDataMgr()
    {
        mProfit = 0.0;
    }
};

Q_DECLARE_METATYPE(ShareExchangeDataMgr)

class QExchangeRecordWorker : public QObject
{
    Q_OBJECT
public:
    explicit QExchangeRecordWorker(QObject *parent = 0);
    ~QExchangeRecordWorker();


signals:
    void signalStartImport(const QString& file);
    void signalSendCodeList(const QStringList& list);
    void signalQueryShareProfitList(bool clear);
    void signalSendShareProfitList(const QList<ShareExchangeDataMgr>& list);
public slots:
    void slotStartImport();
    void slotUpdateRecordSucceed();
    void slotQueryShareProfitList(bool clear);

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
    QThread                                 mWorkThread;
    QMap<int, int>                          mColMap;
    QMap<QString, ShareExchangeDataMgr>     mShareDataMgrMap;
    QXlsx::Document*                        mXlsx;
};

#endif // QEXCHANGERECORDWORKER_H
