#ifndef QSINASTKINFOTHREAD_H
#define QSINASTKINFOTHREAD_H

#include <QThread>
#include <QList>
#include <QMap>
#include "stockdata.h"
#include <QStringList>

typedef enum enStockDisplayRule
{
    STK_DISPLAY_SORT_RULE_NONE = 0,
    STK_DISPLAY_SORT_RULE_DESCEND,
    STK_DISPLAY_SORT_RULE_ASCEND
}STK_DISPLAY_RULE;

typedef enum enStockDisplayType
{
    STK_DISPLAY_SORT_TYPE_NONE = -1,
    STK_DISPLAY_SORT_TYPE_PRICE = 0,
    STK_DISPLAY_SORT_TYPE_CHGPER,
    STK_DISPLAY_SORT_TYPE_CJE,
    STK_DISPLAY_SORT_TYPE_MONEYR,
    STK_DISPLAY_SORT_TYPE_LAST3,
    STK_DISPLAY_SORT_TYPE_ZJLX,
    STK_DISPLAY_SORT_TYPE_GXL,
    STK_DISPLAY_SORT_TYPE_SZZBL,    
    STK_DISPLAY_SORT_TYPE_TCAP,
    STK_DISPLAY_SORT_TYPE_MCAP,
    STK_DISPLAY_SORT_TYPE_GQDJR,
    STK_DISPLAY_SORT_TYPE_HSL,
}STK_DISPLAY_TYPE;

class QSinaStkInfoThread : public QThread
{
    Q_OBJECT
public:
    explicit QSinaStkInfoThread(QObject *parent = 0);
    ~QSinaStkInfoThread();

    bool isActive();
protected:
    void run();
    void RealtimeInfo();
signals:
    void    sendStkDataList(const StockDataList& list);
public slots:
    void    setOptType(STK_DISPLAY_TYPE type);
    void    setSortRule(STK_DISPLAY_RULE rule);
    void    reverseSortRule();
    void    setStkList(const QStringList& list);
    void    setActive(bool act);
private:
    int         mSortRule;
    int         mOptType;
    QStringList mStkList;
    QMap<QString, StockData>    mDataMap;
    //bool        mActive;
};

#endif // QSINASTKINFOTHREAD_H
