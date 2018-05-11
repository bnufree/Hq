#include "qactivedate.h"

QStringList QActiveDateTime::mCloseDateList;

QActiveDateTime::QActiveDateTime():QDateTime()
{

}

QActiveDateTime::QActiveDateTime(const QDate &date):QDateTime(date)
{
}

bool QActiveDateTime::isWeekend()
{
    return this->date().dayOfWeek() == 6 || this->date().dayOfWeek() == 7;
}

bool QActiveDateTime::isDayActive()
{
    return !(isWeekend() || QActiveDateTime::mCloseDateList.contains(this->date().toString("yyyy-MM-dd")));
}

bool QActiveDateTime::isActive()
{
    return isDayActive() && isTimeActive();
}

bool QActiveDateTime::isTimeActive()
{
    QTime time = this->time();
    int act_start1 = 9*60+15;
    int act_end1 = 11*60+30;
    int act_start2 = 13*60;
    int act_end2 = 15*60;
    int hour = time.hour();
    int minute = time.minute();
    int res = hour *60 + minute;
    if((res >= act_start1 && res <= act_end1) || (res >= act_start2 && res <= act_end2))
    {
            return true;
    }
    return false;
}

QDate QActiveDateTime::preActiveDay()
{
    QDate date = this->addDays(-1).date();
    while (!QActiveDateTime(date).isActive()) {
        date = date.addDays(-1);
    }
    return date;
}

QDate QActiveDateTime::nextActiveDay()
{
    QDate date = this->addDays(1).date();
    while (!QActiveDateTime(date).isActive()) {
        date = date.addDays(1);
    }
    return date;
}



QDate QActiveDateTime::latestActiveDay()
{
    QActiveDateTime datetime(QDate::currentDate());
    if(datetime.isDayActive()) return datetime.date();
    return datetime.preActiveDay();
}

QDate QActiveDateTime::getActiveDayBefore1HYear()
{
    QActiveDateTime date(QDate::currentDate().addDays(-182));
    if(date.isDayActive()) return date.date();
    return date.preActiveDay();
}

int   QActiveDateTime::activeDaysNum(const QDate& start)
{
    int num = 0;
    QDate wkdate = start;
    while (wkdate < latestActiveDay()) {
        if(QActiveDateTime(wkdate).isDayActive())
        {
            num++;
        }
        wkdate = wkdate.addDays(1);
    }

    return num;
}

bool QActiveDateTime::isCurDateTimeActive()
{
    QActiveDateTime dateTime;
    dateTime.setDate(QDate::currentDate());
    dateTime.setTime(QTime::currentTime());
    return dateTime.isActive();
}
