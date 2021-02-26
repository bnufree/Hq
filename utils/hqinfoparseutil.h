#ifndef HQINFOPARSEUTIL_H
#define HQINFOPARSEUTIL_H

#include "data_structure/sharedata.h"
typedef QList<QDate>        QDateList;

class HqInfoParseUtil
{
public:
    HqInfoParseUtil();
    static QDateList getActiveDateListOfLatestYearPeriod();
    static QDateList getActiveDateList(const QDate& from, const QDate& end);
    static QDateList getActiveDateListOfYear(int year = QDate::currentDate().year());
    static ShareDailyDataList getShareHistoryDataFromHexun(const QDate& start, const QString& code);
    static ShareDailyDataList getShareHistoryDataFrom163(const QDate& start, const QString& code);
    static ShareDailyDataList getShareHistoryDataFromXueqiu(const QDate& start, const QString& code);
    static ShareDailyDataList getShareHistoryData(const QDate& start, const QString& code);
    static bool     getShareDateRange(const QString& code, QDate& start, QDate& end);
    static bool parseShareDataFromSinaA(ShareData& data, const QString& hqstr);
    static bool parseShareDataFromSinaHK(ShareData& data, const QString &hqstr);
    static QDateList  dateListFromStringList(const QStringList& list);
    static QStringList dateListToStringList(const QDateList& list );


private:

};

#endif // HQINFOPARSEUTIL_H
