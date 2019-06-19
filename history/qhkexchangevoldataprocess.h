﻿#ifndef QHKEXCHANGEVOLDATAPROCESS_H
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
    void getMktVolInfo(ShareDataList& list,int& num, const QDate& date, int mkt = 0);
    void getMktVolInfo(QStringList& list, const QDate& date, const QString& fileName);
private:
    void getVolofDate(ShareDataList& list, const QDate& date);
    void getVolofDateFromEastMoney(ShareDataList& list, const QDate& date);
private:
    QDate       mStartDate;
    QDate       mEndDate;
    QObject*    mParent;
};

#endif // QHKEXCHANGEVOLDATAPROCESS_H
