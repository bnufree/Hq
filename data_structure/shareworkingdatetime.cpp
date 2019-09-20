#include "shareworkingdatetime.h"
#include "utils/qhttpget.h"
#include <QTextCodec>

#define         DATE_STR_FORMAT         "yyyy-MM-dd"

QList<QDate> ShareWorkingDate::mHisWorkingDayList = QList<QDate>();
QDate        ShareWorkingDate::mLastWorkDay = QDate();
QDate        ShareWorkingDate::mCurWorkDay = QDate();


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

