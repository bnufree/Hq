#include "shareworkingdatetime.h"
#include "utils/qhttpget.h"
#include <QTextCodec>
#include <QMutex>

#define         DATE_STR_FORMAT         "yyyy-MM-dd"

TradeDateMgr*        TradeDateMgr::mInstance = 0;
QMutex               global_trade_mutex;

TradeDateMgr* TradeDateMgr::instance()
{
    if(mInstance == 0)
    {
        QMutexLocker locker(&global_trade_mutex);
        if(mInstance == 0) mInstance = new TradeDateMgr;
    }

    return mInstance;
}

QDate  TradeDateMgr::firstTradeDayOfYear() const
{
    int year  = mCurTradeDay.year();
    QDate now(year, 1, 1);
    while (1) {
        if(isTradeDay(now)) break;
        now = now.addDays(1);
        if(now == mCurTradeDay) break;
    }

    return now;
}

QDate  TradeDateMgr::firstTradeDayOfMonth() const
{
    int year  = mCurTradeDay.year();
    int month = mCurTradeDay.month();
    QDate now(year, month, 1);
    while (1) {
        if(isTradeDay(now)) break;
        now = now.addDays(1);
        if(now == mCurTradeDay) break;
    }

    return now;
}

QDate  TradeDateMgr::firstTradeDayOfWeek() const
{
    QDate now = mCurTradeDay;
    now = now.addDays(1 - now.dayOfWeek());
    while (1) {
        if(isTradeDay(now)) break;
        now = now.addDays(1);
        if(now == mCurTradeDay) break;
    }

    return now;
}

QDate  ShareTradeDateTime::nextTradeDay() const
{
    QDate next = this->date();
    while (1) {
        if(next == TradeDateMgr::instance()->currentTradeDay()) break;
        next = next.addDays(1);
        if(TradeDateMgr::instance()->isTradeDay(next)) break;
    }
    return next;
}

QDate  ShareTradeDateTime::previousTradeDay() const
{
    QDate previous = this->date();
    while (1) {
        if(previous == TradeDateMgr::instance()->firstTradeDayOfYear()) break;
        previous = previous.addDays(-1);
        if(TradeDateMgr::instance()->isTradeDay(previous)) break;
    }
    return previous;
}


#if 0

void ShareWorkingDate::updateShareHistoryWoringDate()
{
    //历史数据记录当前时间前一年的数据
    ShareWorkingDate cur = ShareWorkingDate::currentDate();
    ShareWorkingDate start(cur.date().addYears(-1));
    QString wkURL = QString("http://quotes.money.163.com/service/chddata.html?code=0000001&start=%1&end=%2")
            .arg(start.toString("yyyyMMdd")).arg(cur.toString("yyyyMMdd"));
    QByteArray recv = QHttpGet::getContentOfURL(wkURL);
    QTextCodec* gbk = QTextCodec::codecForName("GBK");
    QTextCodec* utf8 = QTextCodec::codecForName("UTF-8");
    QString result = QString::fromUtf8(utf8->fromUnicode(gbk->toUnicode(recv)));

    QStringList lines = result.split("\r\n");
    QList<QDate> list;
    int size = lines.length();
    for(int i=size-1; i>0; i--)
    {
        QStringList cols = lines[i].split(",");
        if(cols.length() >= 15)
        {
            //if(mCode == "000400")qDebug()<<recv.mid(20, 200);
            ShareWorkingDate curDate = ShareWorkingDate::fromString(cols[0]);
            if(curDate.isWeekend()) continue;
            list.append(curDate.date());
        }
    }
    setHisWorkingDay(list);
}

ShareWorkingDate ShareWorkingDate::latestWorkDay()
{
    ShareWorkingDate date = ShareWorkingDate::currentDate();
    if(!date.isWeekend()) return date;
    int day = date.date().dayOfWeek();
    return ShareWorkingDate(date.date().addDays(5-day));
}

ShareWorkingDate ShareWorkingDate::nextWorkDay()
{
    ShareWorkingDate nextDay(this->date().addDays(1));
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

ShareWorkingDate ShareWorkingDate::previousWorkDay()
{
    ShareWorkingDate preDay(this->date().addDays(-1));
    while (preDay.isWeekend() || !mHisWorkingDayList.contains(preDay.date()))
    {
        preDay.previous();
    }
    return preDay;
}

void ShareWorkingDate::setCurWorkDate(const QDate& date)
{
    mCurWorkDay = date;
    int index = mHisWorkingDayList.indexOf(mCurWorkDay);
    if(index < 0)
    {
        if(mHisWorkingDayList.size() > 0)
        {
            mLastWorkDay = mHisWorkingDayList.first();
        } else
        {
            mLastWorkDay = QDate();
        }
    } else
    {
        if(index == 0)
        {
            index++;
        } else
        {
            index--;
        }
        mLastWorkDay = mHisWorkingDayList.value(index);
    }
}

#endif

