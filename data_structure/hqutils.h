#ifndef HQUTILS_H
#define HQUTILS_H

#include <QDate>
#include <QDebug>
#include <QDateTime>
#include <QStringList>

#define         ANDROID_FILE_PATH       "/mnt/sdcard/com.hq.info"

class ShareDate
{
public:
    ShareDate() {}
    ShareDate(const QDate& date):mDate(date){}
    ShareDate(uint time_t){mDate = QDateTime::fromTime_t(time_t).date();}
    ShareDate(const ShareDate& other) {mDate = other.mDate;}
    ShareDate& operator=(const ShareDate& other){
        if(this != &other) {
            this->mDate = other.mDate;
        }
        return *this;
    }
    bool operator ==(const ShareDate& other) const
    {
        return mDate == other.mDate;
    }

    bool operator !=(const ShareDate& other) const
    {
        return mDate != other.mDate;
    }

    bool operator <(const ShareDate& other) const
    {
        return mDate < other.mDate;
    }

    bool operator <=(const ShareDate& other) const
    {
        return mDate <= other.mDate;
    }

    bool operator >(const ShareDate& other) const
    {
        return mDate > other.mDate;
    }

    bool operator >=(const ShareDate& other) const
    {
        return mDate >= other.mDate;
    }

    uint toTime_t() const {return QDateTime(mDate).toTime_t();}
    static ShareDate fromTime_t(uint time_t)
    {
        return ShareDate(time_t);
    }

    QString toString(const QString& format = "yyyy-MM-dd") const
    {
        if(toTime_t() == 0) return "-";
        return mDate.toString(format);
    }

    static ShareDate fromString(const QString& str)
    {
        return ShareDate(QDate::fromString(str, "yyyy-MM-dd"));
    }

    static ShareDate currentDate(){
        return ShareDate(QDate::currentDate());
    }
    QDate   date() const {return mDate;}
    bool    isNull() const {return mDate.isNull();}
    qint64  mSecs() const {return QDateTime(mDate).toMSecsSinceEpoch();}
    bool    isWeekend() const {return mDate.dayOfWeek() == 6 || mDate.dayOfWeek() == 7;}
    static ShareDate latestWorkDay()
    {
        ShareDate date = ShareDate::currentDate();
        if(!date.isWeekend()) return date;
        int day = date.date().dayOfWeek();
        return ShareDate(date.date().addDays(5-day));
    }
    void setDate(const QDate& date) {mDate = date;}
    void    next() {mDate = mDate.addDays(1);}
    void    previous() {mDate = mDate.addDays(-1);}
    ShareDate nextWorkDay()
    {
        ShareDate nextDay(this->date().addDays(1));
        while (nextDay.isWeekend() || !mHisWorkingDayList.contains(nextDay.date()))
        {
            nextDay.next();
            if(nextDay.date() == QDate::currentDate())
            {
                return nextDay.latestWorkDay();
            }
        }
        return nextDay;
    }

    ShareDate previousWorkDay()
    {
        ShareDate preDay(this->date().addDays(-1));
        while (preDay.isWeekend() || !mHisWorkingDayList.contains(preDay.date()))
        {
            preDay.previous();
        }
        return preDay;
    }

    static QList<QDate> getHisWorkingDay() {return mHisWorkingDayList;}
    static void appendHisWorkingDay(const QList<QDate>& list) {mHisWorkingDayList.append(list);}
    static void setHisWorkingDay(const QList<QDate>& list) {mHisWorkingDayList = list;}
    static bool isHisWorkingDay(const QDate& date) {return mHisWorkingDayList.contains(date);}
    static void setCurWorkDate(const QDate& date)
    {
        mCurWorkDay = date;
        mLastWorkDay = ShareDate(mCurWorkDay).previousWorkDay().date();
    }

    static ShareDate getCurWorkDay() {return ShareDate(mCurWorkDay);}
    static ShareDate getLastWorkDay() {return ShareDate(mLastWorkDay);}

private:
    QDate       mDate;
    static      QList<QDate>        mHisWorkingDayList;
    static      QDate               mLastWorkDay;
    static      QDate               mCurWorkDay;
};

QList<QDate> ShareDate::mHisWorkingDayList;
QDate        ShareDate::mLastWorkDay;
QDate        ShareDate::mCurWorkDay;

typedef   QList<ShareDate>      ShareDateList;


class ShareDateTime : public QDateTime
{
public:
    ShareDateTime():QDateTime() {}
    ShareDateTime(const QDate& date):QDateTime(date){}
    ShareDateTime(uint time_t):QDateTime(QDateTime::fromTime_t(time_t).date()){}
    static ShareDateTime fromTime_t(uint time_t)
    {
        return ShareDateTime(time_t);
    }
    QString toString(bool time = false){
        if(this->toTime_t() == 0) return "-";
        if(time) return QDateTime::toString("hh:mm:ss");
        else return QDateTime::toString("yyyy-MM-dd");
    }
    static ShareDateTime fromString(const QString& str, bool secs = false)
    {
        if(!secs) return ShareDateTime(QDate::fromString(str, "yyyy-MM-dd"));
        else return ShareDateTime(QDate::fromString(str, "yyyy-MM-dd hh:mm:ss"));
    }

    static ShareDateTime currentDateTime(){
        return ShareDateTime(QDateTime::currentDateTime().toTime_t());
    }
    //QDate   date() const {return this->date();}
    bool    isNull() const {return this->date().isNull();}
    qint64  mSecs() const {return this->toMSecsSinceEpoch();}
    ShareDate shareDate() const {return ShareDate(date());}
};

typedef     ShareDateTime       BlockDateTime;


class HqUtils
{

public:
    HqUtils();
    static QString prefixCode(const QString& code);
    static QString GetFirstLetter( const QString& unicode );
    static bool writeString2File(const QString& str, FILE *fp);
    static bool writeInt2File(int val, FILE* fp);
    static QString  double2Str(double val);
    static bool makePath(const QString& path);

public:
    //static QStringList        mDatesList;
};

#endif // HQUTILS_H
