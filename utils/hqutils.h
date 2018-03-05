#ifndef HQUTILS_H
#define HQUTILS_H

#include <QDate>
#include <QDebug>
#include <QDateTime>
#include <QStringList>
enum SHARE_TYPE{
    SHARE_UNDEFINE = -1,
    SHARE_SH = 0,
    SHARE_SZ,
    SHARE_US,
    SHARE_HK,
    SHARE_FOUND,
};

class HqUtils
{

public:
    HqUtils();
    static bool weekend(const QDate& date);
    static bool activeDay(const QDate& date);
    static SHARE_TYPE shareType(const QString& code);
    static QString prefixCode(const QString& code);
    static bool isCurrentActive();
    static bool isActiveTime(const QTime& time);
    static QString GetFirstLetter( const QString& unicode );
    static bool writeString2File(const QString& str, FILE *fp);
    static QString  date2Str(const QDate& date);
    static QDate    dateFromStr(const QString& str);
    static QString  double2Str(double val);

public:
    static QStringList        mDatesList;
};

#endif // HQUTILS_H
