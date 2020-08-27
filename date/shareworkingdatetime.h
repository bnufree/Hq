#ifndef SHAREWORKINGDATETIME_H
#define SHAREWORKINGDATETIME_H

#include <QDateTime>
#include <QTime>

class TradeDateMgr
{
public:
    static TradeDateMgr* instance();
    QDate currentTradeDay() const
    {
        return mCurTradeDay;
    }

    void setCurrentTradeDay(const QDate &date)
    {
        mCurTradeDay = date;
    }

    void setLastTradeDay(const QDate &date)
    {
        mLastTradeDay = date;
    }

    QDate lastTradeDay() const
    {
        return mLastTradeDay;
    }

    bool isTradeDay( const QDate& day) const
    {
        return mHistoryTradeDaysOfYear.contains(day) || day == currentTradeDay();
    }

    void setHistoryTradeDays(const QList<QDate>& list)
    {
        mHistoryTradeDaysOfYear.clear();
        mHistoryTradeDaysOfYear.append(list);
    }

    QDate firstTradeDayOfYear() const;
    QDate firstTradeDayOfMonth() const;
    QDate firstTradeDayOfWeek() const;

private:
private:
    TradeDateMgr() {}
    static TradeDateMgr*          mInstance;
    QList<QDate>        mHistoryTradeDaysOfYear;           //今年以来法定闭市日
    QDate               mLastTradeDay;              //上一个交易日
    QDate               mCurTradeDay;               //当前交易日
};



class ShareTradeDateTime : public QDateTime
{
public:
    ShareTradeDateTime():QDateTime() {}
    ShareTradeDateTime(const QDate& date):QDateTime(date){}
    ShareTradeDateTime(const QDateTime& date):QDateTime(date){}
    ShareTradeDateTime(uint time_t):QDateTime(QDateTime::fromTime_t(time_t)){}
    static ShareTradeDateTime fromTime_t(uint time_t)
    {
        return ShareTradeDateTime(time_t);
    }
    QString toString(bool time = false) const
    {
        if(this->toTime_t() == 0) return "-";
        if(time) return QDateTime::toString("hh:mm:ss");
        else return QDateTime::toString("yyyy-MM-dd");
    }
    static ShareTradeDateTime fromString(const QString& str, bool secs = false)
    {
        if(!secs) return ShareTradeDateTime(QDate::fromString(str, "yyyy-MM-dd"));
        else return ShareTradeDateTime(QDateTime::fromString(str, "yyyy-MM-dd hh:mm:ss"));
    }

    static ShareTradeDateTime currentDateTime(){
        return ShareTradeDateTime(QDateTime::currentDateTime().toTime_t());
    }

    bool isTradeDay() const
    {
        return TradeDateMgr::instance()->isTradeDay(this->date());
    }

    bool isTradeNow() const
    {
        if(!isTradeDay()) return false;
        if(time().hour() < 9 || time().hour() >= 16) return false;
        if(time().hour() > 10) return true;
        if(time().hour() == 9 && time().minute() >= 15) return true;
        return false;
    }
    qint64  msecs() const {return this->toMSecsSinceEpoch();}
    qint64  secs() const {return this->toMSecsSinceEpoch() / 1000;}

    QDate  nextTradeDay() const;
    QDate  previousTradeDay() const;
};

typedef     ShareTradeDateTime       BlockDateTime;


#endif // SHAREWORKINGDATETIME_H
