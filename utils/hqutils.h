﻿#ifndef HQUTILS_H
#define HQUTILS_H

#include <QDate>
#include <QDebug>
#include <QDateTime>
#include <QStringList>


#define         ANDROID_FILE_PATH       "/mnt/sdcard/com.hq.info"
class ShareDate: public QDate
{

};

class HqUtils
{

public:
    HqUtils();
    static bool weekend(const QDate& date);
    static bool activeDay(const QDate& date);
    static QString prefixCode(const QString& code);
    static bool isCurrentActive();
    static bool isActiveTime(const QTime& time);
    static QString GetFirstLetter( const QString& unicode );
    static bool writeString2File(const QString& str, FILE *fp);
    static bool writeInt2File(int val, FILE* fp);
    static QString  date2Str(const QDate& date);
    static QDate    dateFromStr(const QString& str);
    static QString  double2Str(double val);
    static QDate    latestActiveDay();
    static QDate    lastActiveDay();
    static int      activeDaysNum(const QDate& start);
    static QDate    getActiveDayBefore1HYear();
    static QDate    getLgtStartDate();

public:
    //static QStringList        mDatesList;
};

#endif // HQUTILS_H
