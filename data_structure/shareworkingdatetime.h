#ifndef SHAREWORKINGDATETIME_H
#define SHAREWORKINGDATETIME_H

#include <QDateTime>

class ShareWorkingDate
{
public:
    ShareWorkingDate() {}
    ShareWorkingDate(const QDate& date):mDate(date){}
    ShareWorkingDate(uint time_t){mDate = QDateTime::fromTime_t(time_t).date();}
    ShareWorkingDate(const ShareWorkingDate& other) {mDate = other.mDate;}
    ShareWorkingDate& operator=(const ShareWorkingDate& other){
        if(this != &other) {
            this->mDate = other.mDate;
        }
        return *this;
    }
    bool operator ==(const ShareWorkingDate& other) const
    {
        return mDate == other.mDate;
    }

    bool operator !=(const ShareWorkingDate& other) const
    {
        return mDate != other.mDate;
    }

    bool operator <(const ShareWorkingDate& other) const
    {
        return mDate < other.mDate;
    }

    bool operator <=(const ShareWorkingDate& other) const
    {
        return mDate <= other.mDate;
    }

    bool operator >(const ShareWorkingDate& other) const
    {
        return mDate > other.mDate;
    }

    bool operator >=(const ShareWorkingDate& other) const
    {
        return mDate >= other.mDate;
    }

    uint toTime_t() const {return QDateTime(mDate).toTime_t();}
    static ShareWorkingDate fromTime_t(uint time_t)
    {
        return ShareWorkingDate(time_t);
    }

    QString toString(const QString& format = "yyyy-MM-dd") const
    {
        if(toTime_t() == 0) return "-";
        return mDate.toString(format);
    }

    static ShareWorkingDate fromString(const QString& str)
    {
        return ShareWorkingDate(QDate::fromString(str, "yyyy-MM-dd"));
    }

    static ShareWorkingDate currentDate(){
        return ShareWorkingDate(QDate::currentDate());
    }
    QDate   date() const {return mDate;}
    bool    isNull() const {return mDate.isNull();}
    qint64  mSecs() const {return QDateTime(mDate).toMSecsSinceEpoch();}
    bool    isWeekend() const {return mDate.dayOfWeek() == 6 || mDate.dayOfWeek() == 7;}
    void setDate(const QDate& date) {mDate = date;}
    void    next() {mDate = mDate.addDays(1);}
    void    previous() {mDate = mDate.addDays(-1);}

    ShareWorkingDate nextWorkDay();
    ShareWorkingDate previousWorkDay();
    static ShareWorkingDate latestWorkDay();
    static QList<QDate> getHisWorkingDay() {return mHisWorkingDayList;}
    static void appendHisWorkingDay(const QList<QDate>& list) {mHisWorkingDayList.append(list);}
    static void setHisWorkingDay(const QList<QDate>& list) {mHisWorkingDayList = list;}
    static bool isHisWorkingDay(const QDate& date) {return mHisWorkingDayList.contains(date);}
    static void setCurWorkDate(const QDate& date);
    static ShareWorkingDate getCurWorkDay() {return ShareWorkingDate(mCurWorkDay);}
    static ShareWorkingDate getLastWorkDay() {return ShareWorkingDate(mLastWorkDay);}
    static void updateShareHistoryWoringDate();

private:
    QDate       mDate;
    static      QList<QDate>        mHisWorkingDayList;
    static      QDate               mLastWorkDay;
    static      QDate               mCurWorkDay;
};



typedef   QList<ShareWorkingDate>      ShareWorkingDateList;


class ShareWorkingDateTime : public QDateTime
{
public:
    ShareWorkingDateTime():QDateTime() {}
    ShareWorkingDateTime(const QDate& date):QDateTime(date){}
    ShareWorkingDateTime(uint time_t):QDateTime(QDateTime::fromTime_t(time_t).date()){}
    static ShareWorkingDateTime fromTime_t(uint time_t)
    {
        return ShareWorkingDateTime(time_t);
    }
    QString toString(bool time = false){
        if(this->toTime_t() == 0) return "-";
        if(time) return QDateTime::toString("hh:mm:ss");
        else return QDateTime::toString("yyyy-MM-dd");
    }
    static ShareWorkingDateTime fromString(const QString& str, bool secs = false)
    {
        if(!secs) return ShareWorkingDateTime(QDate::fromString(str, "yyyy-MM-dd"));
        else return ShareWorkingDateTime(QDate::fromString(str, "yyyy-MM-dd hh:mm:ss"));
    }

    static ShareWorkingDateTime currentDateTime(){
        return ShareWorkingDateTime(QDateTime::currentDateTime().toTime_t());
    }
    //QDate   date() const {return this->date();}
    bool    isNull() const {return this->date().isNull();}
    qint64  mSecs() const {return this->toMSecsSinceEpoch();}
    ShareWorkingDate workingDate() const {return ShareWorkingDate(this->date());}
};

typedef     ShareWorkingDateTime       BlockDateTime;


#endif // SHAREWORKINGDATETIME_H
