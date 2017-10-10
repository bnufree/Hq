#ifndef QSINASTKINFOTHREAD_H
#define QSINASTKINFOTHREAD_H

#include <QThread>
#include <QList>
#include <QMap>
#include "stockdata.h"
#include <QStringList>



class QSinaStkInfoThread : public QThread
{
    Q_OBJECT
public:
    explicit QSinaStkInfoThread(QObject *parent = 0);
    ~QSinaStkInfoThread();

    bool isActive();
    int  getStkCount();
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
