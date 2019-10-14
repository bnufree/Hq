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
    static ShareHistoryFileDataList getShareHistoryDataFromHexun(const QDate& start, const QString& code);
    static ShareHistoryFileDataList getShareHistoryDataFrom163(const QDate& start, const QString& code);

private:

};

#endif // HQINFOPARSEUTIL_H
