#ifndef QHKEXCHANGEVOLDATAPROCESS_H
#define QHKEXCHANGEVOLDATAPROCESS_H

#include <QRunnable>
#include <QDate>
#include <QObject>
#include "data_structure/sharedata.h"
#include <QMap>

class QHKExchangeVolDataProcess : public QRunnable
{
public:
    explicit QHKExchangeVolDataProcess(const QDate& start, const QDate& end, QObject* parent = 0);
    ~QHKExchangeVolDataProcess() {}

public:
    void run();

private:
    void getVolofDate(ShareForignVolFileDataList& list, const QDate& date);
    void getVolInfoFromHKEX(ShareForignVolFileDataList& list,int& num, const QDate& date, int mkt = 0);
    bool getVolInfoFromFile(ShareForignVolFileDataList& list, const QDate& date);
    bool getVolInfoFromEastMoney(ShareForignVolFileDataList& list, const QDate& date);
    bool saveData(const QDate& date, const ShareForignVolFileDataList& list);
    QString getFileName(const QDate& date);
private:
    QDate       mStartDate;
    QDate       mEndDate;
    QObject*    mParent;
};

#endif // QHKEXCHANGEVOLDATAPROCESS_H
