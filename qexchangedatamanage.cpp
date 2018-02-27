#include "qexchangedatamanage.h"
#include "utils/profiles.h"

QExchangeDataManage* QExchangeDataManage::minstance = 0;
QExchangeDataManage::QExchangeDataDeleter QExchangeDataManage::delobj;


QExchangeDataManage::QExchangeDataManage(QObject *parent) : QObject(parent)
{
    mUnexhangeDayList =  Profiles::instance()->value("DateManage", "Expired").toStringList();
}

QExchangeDataManage::~QExchangeDataManage()
{

}

QExchangeDataManage* QExchangeDataManage::instance()
{
    if(minstance == NULL)
    {
        minstance = new QExchangeDataManage;
    }
    return minstance;
}

void QExchangeDataManage::AddDay(const QDate& date)
{
    QString datestr = date.toString("yyyy-MM-dd");
    if(!mUnexhangeDayList.contains(datestr))
    {
        mUnexhangeDayList.append(datestr);
        Profiles::instance()->setValue("DateManage", "Expired", mUnexhangeDayList);
    }
}

void QExchangeDataManage::RemoveDay(const QDate& date)
{
    QString datestr = date.toString("yyyy-MM-dd");
    if(mUnexhangeDayList.contains(datestr))
    {
        mUnexhangeDayList.removeOne(datestr);
        Profiles::instance()->setValue("DateManage", "Expired", mUnexhangeDayList);
    }
}

bool QExchangeDataManage::IsActiveDay(const QDate& date)
{
    if(date.dayOfWeek() == 6 || date.dayOfWeek() == 7) return false;
    if(mUnexhangeDayList.contains(date.toString("yyyy-MM-dd"))) return false;
    return true;
}

QDate  QExchangeDataManage::GetLatestActiveDay(const QDate& date)
{
    QDate workDate = date;
    while(!IsActiveDay(workDate))
    {
        workDate = workDate.addDays(-1);
    }

    return workDate;
}

QStringList QExchangeDataManage::GetUnExchangeDays()
{
    return mUnexhangeDayList;
}

bool QExchangeDataManage::IsNeedUpdate(const QDate &last, const QDate &cur)
{
    if(last >= cur)
    {
        //当前日期和上次更新的日期相同，不同更新
        return false;
    }
//    if(last == GetLatestActiveDay(cur))
//    {
//        //上次更新就是最近的一个交易日，不用更新
//    }

    return true;


}

bool QExchangeDataManage::IsNeedUpdate(const QDateTime &time)
{
    if(!IsActiveDay(time.date())) return false;
    QTime wkTime = time.time();
    if(wkTime.hour() < 9 || wkTime.hour() >= 15) return false;

    return true;
}

