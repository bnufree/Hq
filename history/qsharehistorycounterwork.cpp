#include "qsharehistorycounterwork.h"
#include "dbservices/dbservices.h"
#include "dbservices/qactivedate.h"
#include "date/shareworkingdatetime.h"
#include "utils/qhttpget.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>

QShareHistoryCounterWork::QShareHistoryCounterWork(const QString& code,const ShareDailyDataList& list, QObject* parent)
    :mCode(code),mList(list),mParent(parent),QRunnable()
{

}

QShareHistoryCounterWork::~QShareHistoryCounterWork()
{

}

QString QShareHistoryCounterWork::getFileName()
{
    //设定保存的文件名
    return QString("%1/%2").arg(HQ_DAY_HISTORY_DIR).arg(mCode);
}

bool QShareHistoryCounterWork::readFile(ShareDailyDataList& list)
{
    QString fileName = getFileName();
    if(!QFile::exists(fileName))
    {
        qDebug()<<__FUNCTION__<<__LINE__<<fileName<<" not exist";
        return false;
    }
    //读取文件
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<__FUNCTION__<<__LINE__<<fileName<<" open error";
        return false;
    }
    while (!file.atEnd() ) {
        ShareDailyData data;
        file.read((char*)(&data), sizeof(ShareDailyData));
        list.append(data);
    }
    file.close();

    return true;
}

void QShareHistoryCounterWork::run()
{
#if 0
    if(mList.size() == 0) readFile(mList);
    //获取当前交易日的日期
    QDate now = TradeDateMgr::instance()->currentTradeDay();
    QDate last_day = TradeDateMgr::instance()->lastTradeDay();
    //获取对应的年,月,周对应的参考基准日
    //周参考日对应上周的星期五
    QDate week = now.addDays(-1*(now.dayOfWeek()) - 2);
    //月参考日对应上月的最后一个交易日
    QDate month = QDate(now.year(), now.month(), 1).addDays(-1);
    //年参考日
    QDate year = QDate(now.year(), 1, 1).addDays(-1);

    //开始获取参考日对应的复权价
    int size = mList.size();
    if(size == 0) return;
    double week_p = 0.0, month_p = 0.0, year_p = 0.0;
    bool week_found = false, month_found = false, year_found = false;
    double last_money = mList.last().mMoney;
    //从后开始往前找,找到对应日期或者最靠近日期
    QDate real_week, real_month, real_year;
    for(int i = size - 1; i >= 0; i--)
    {
        ShareHistoryFileData data = mList[i];
        QDate wkDate = QDateTime::fromTime_t(data.mDate).date();
        if(wkDate == last_day) last_money = data.mMoney;
        if((!week_found) && (wkDate <= week))
        {
            week_found = true;
            week_p = data.mCloseAdjust;
            real_week = wkDate;
        }
        if((!month_found) &&(wkDate <= month))
        {
            month_found = true;
            month_p = data.mCloseAdjust;
            real_month = wkDate;
        }

        if((!year_found) && (wkDate <= year))
        {
            year_found = true;
            year_p = data.mCloseAdjust;
            real_year = wkDate;
        }
        if(week_found && month_found && year_found) break;
    }
    //最后仍然没有找到
    if(!week_found)
    {        
        week_p = mList.first().mLastClose;
        real_week = QDateTime::fromTime_t(mList.first().mDate).date();
    }

    if(!month_found)
    {
        month_p = mList.first().mLastClose;
        real_month = QDateTime::fromTime_t(mList.first().mDate).date();
    }

    if(!year_found)
    {
        year_p = mList.first().mLastClose;
        real_year = QDateTime::fromTime_t(mList.first().mDate).date();
    }


    //获取当前陆股通持股信息的变化情况 1日 5日 10日
    double foreign_now = mList.last().mForeignVol;
    double foreign_mutaul = mList.last().mForeignMututablePercent;
    double foreign_chg1 = 0.0, foreign_chg5 = 0.0, foreign_chg10 = 0.0;
//    if(mList.size() > 2) qDebug()<<mList[size-1].mForeignVol<<mList[size-1].mForeignMututablePercent<<mList[size-2].mForeignVol<<mList[size-2].mForeignMututablePercent;
    if(size >= 2)
    {
        foreign_chg1 = mList[size-1].mForeignVol - mList[size-2].mForeignVol;
    } else
    {
        foreign_chg1 = mList[size-1].mForeignVol - mList[0].mForeignVol;
    }
    if(size >= 5)
    {
        foreign_chg5 = mList[size-1].mForeignVol - mList[size-5].mForeignVol;
    } else
    {
        foreign_chg5 = mList[size-1].mForeignVol - mList[0].mForeignVol;
    }

    if(size >= 10)
    {
        foreign_chg10 = mList[size-1].mForeignVol - mList[size-10].mForeignVol;
    } else
    {
        foreign_chg10 = mList[size-1].mForeignVol - mList[0].mForeignVol;
    }

    ShareHistoryCounter counter;
    counter.code = mCode;
    counter.foreign_percent = foreign_mutaul;
    counter.foreign_ch1 = foreign_chg1;
    counter.foreign_ch5 = foreign_chg5;
    counter.foreign_ch10 = foreign_chg10;
    counter.foreign_vol = foreign_now;
    counter.lastMoney = last_money;
    counter.monthDay = real_month;
    counter.monthP = month_p;
    counter.weekDay = real_week;
    counter.weekP = week_p;
    counter.yearDay = real_year;
    counter.yearP = year_p;
//    qDebug()<<mCode<<real_week.toString("yyyyMMdd")<<week_p<<real_month.toString("yyyyMMdd")<<month_p<<real_year.toString("yyyyMMdd")<<year_p<<foreign_chg1<<foreign_chg5<<foreign_chg10<<last_day.toString("yyyy-MM-dd")<<last_money;

    emit DATA_SERVICE->signalUpdateShareCounter(counter);
#endif
}

QShareLGTHistoryCounterWork::QShareLGTHistoryCounterWork(const QDate &date, int type, QObject *parent)
{
    mDate = date;
    mType = type;
    mFilePath = QString("%1/lgt").arg(HQ_DAY_HISTORY_DIR);
    QDir dir(mFilePath);
    if(!dir.exists())
    {
        dir.mkpath(mFilePath);
    }
    
    mFileName = QString("%1/counter.dat").arg(mFilePath);
}
QString QShareLGTHistoryCounterWork::getFileName()
{
    return mFilePath;
}

bool QShareLGTHistoryCounterWork::readFromHisrotyFile()
{
    QString fileName = getFileName();
    if(!QFile::exists(fileName))
    {
        qDebug()<<__FUNCTION__<<__LINE__<<fileName<<" not exist";
        return false;
    }
    //读取文件
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<__FUNCTION__<<__LINE__<<fileName<<" open error";
        return false;
    }
    while (!file.atEnd() ) {
        ShareHsgtCounter data;
        file.read((char*)(&data), sizeof(ShareHsgtCounter));
        mDataMap[data.mCode] = data;
    }
    file.close();

    return true;
}

Counter counterFromObj(const QJsonObject& obj)
{
    Counter counter;
    counter.mMktCapChg = obj.value("ShareSZ_Chg_One").toDouble();
    counter.mVolChg = obj.value("ShareHold_Chg_One").toDouble();
    counter.mMktCapChgPercent = obj.value("ShareSZ_Chg_Rate_One").toDouble();
    counter.mVolMutableChgPercent = obj.value("LTZB_One").toDouble();
    counter.mVolTotalChgPercent = obj.value("ZZB_One").toDouble();
    return counter;
}

void QShareLGTHistoryCounterWork::run()
{
    //会自动根据日期获取
    int cur_page = 1;
    int total_page = 1;
    while (true)
    {
        QString url = "http://dcfm.eastmoney.com/EM_MutiSvcExpandInterface/api/js/get?type=HSGT20_GGTJ_SUM&token=894050c76af8597a853f5b408b759f5d&st=ShareSZ&sr=-1&ps=5000&js={pages:(tp),data:(x)}&filter=&rt=52937604&p=";
        QByteArray bytes = QHttpGet::getContentOfURL(url.append(QString::number(cur_page)));
        QJsonDocument doc = QJsonDocument::fromJson(bytes);
        if(!doc.isObject()) break;
        QJsonObject obj = doc.object();
        total_page = obj.value("pages").toInt();
        QJsonArray array = obj.value("data").toArray();
        for(int i=0; i<array.size(); i++)
        {
            obj = array[i].toObject();
            int code = obj.value("SCode").toInt();
            ShareHsgtCounter &data = mDataMap[code];
            data.mCode = code;
            data.mVolTotal = qint64(obj.value("ShareHold").toDouble());
            data.mMktCap = obj.value("ShareSZ").toDouble();
            data.mVolMutablePercent = obj.value("LTZB").toDouble();
            data.mVolTotalPercent = obj.value("ZZB").toDouble();
            data.mDate = QDateTime(QDate::fromString(obj.value("HdDate").toString(), "yyyy-MM-dd")).toMSecsSinceEpoch();
            QString type = obj.value("DateType").toString();
            if(type == "y")
            {
                data.mYear = counterFromObj(obj);
            } else if(type == "jd")
            {
                data.mSeason = counterFromObj(obj);
            } else if(type == "m")
            {
                data.mMonth = counterFromObj(obj);
            } else if(type == "10")
            {
                data.mDay10 = counterFromObj(obj);
            } else if(type == "5")
            {
                data.mDay5 = counterFromObj(obj);
            } else if(type == "3")
            {
                data.mDay3 = counterFromObj(obj);
            } else if(type == "1")
            {
                data.mDay1 = counterFromObj(obj);
            }
        }
        cur_page++;
        if(cur_page > total_page) break;
    }
}
