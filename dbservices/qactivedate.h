#ifndef QACTIVEDATETIME_H
#define QACTIVEDATETIME_H

#include <QDateTime>
#include <QStringList>

#define         DATE_FORMAT         "yyyy-MM-dd"
#define         TIME_FORMAT         "hh:mm:ss"
#define         DATETIME_FORMAT     "yyyy-MM-dd hh:mm:ss"

class QActiveDateTime : public QDateTime
{
public:
    QActiveDateTime();
    QActiveDateTime(const QDate& date);
    bool        isWeekend();
    bool        isDayActive();
    bool        isActive();
    bool        isTimeActive();
    QDate       preActiveDay();
    QDate       nextActiveDay();

    static      QDate       latestActiveDay();
    static      int         activeDaysNum(const QDate& start);
    static      QDate       getActiveDayBefore1HYear();
    static QStringList     mCloseDateList;
    static      bool        isCurDateTimeActive();

private:
    //QDate       mDate;
};

#endif // QACTIVEDATETIME_H
