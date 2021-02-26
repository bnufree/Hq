#include "hqinfoparseutil.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonArray>
#include "qhttpget.h"
#include <QTextCodec>
#include "date/shareworkingdatetime.h"
#include <math.h>


QDateList  HqInfoParseUtil::dateListFromStringList(const QStringList &list)
{
    QDateList res;
    foreach (QString str, list) {
        res.append(QDate::fromString(str, "yyyy-MM-dd"));
    }

    return res;
}
QStringList HqInfoParseUtil::dateListToStringList(const QDateList& list )
{
    QStringList res;
    for(int i=0; i<list.size(); i++)
    {
        res.append(list[i].toString("yyyy-MM-dd"));
    }

    return res;
}

HqInfoParseUtil::HqInfoParseUtil()
{

}

ShareDailyDataList HqInfoParseUtil::getShareHistoryData(const QDate& start, const QString& code)
{
    ShareDailyDataList list;
//    if(ShareData::shareType(code) & SHARE_FUND)
//    {
//        list = getShareHistoryDataFromXueqiu(start, code);
//    } else
//    {
//        list = getShareHistoryDataFrom163(start, code);
//        if(list.size() == 0) list = getShareHistoryDataFromHexun(start, code);
//    }
    list = getShareHistoryDataFromXueqiu(start, code);
    if(list.size() == 0)    list = getShareHistoryDataFrom163(start, code);
    if(list.size() == 0)    list = getShareHistoryDataFromHexun(start, code);
    return list;
}

//雪球同步获取港股通和日线数据
//https://stock.xueqiu.com/v5/stock/chart/kline.json?symbol=SH600036&begin=1614417752550&period=day&type=normal&count=-284&indicator=kline,agt,ggt,balance

ShareDailyDataList HqInfoParseUtil::getShareHistoryDataFromXueqiu(const QDate &start, const QString &code)
{
    ShareDailyDataList list;
    //再次从雪球获取
    QStringList urlList;
    urlList.append("https://xueqiu.com/");
    QNetworkAccessManager mgr;
    QString wkCode = (ShareData::shareType(code) & SHARE_SH_TOTAL ? "SH" : "SZ") + code;
    QDate wkDate(start);
    while (1) {
        QString url = QString("https://stock.xueqiu.com/v5/stock/chart/kline.json?symbol=%1&begin=%2&period=day&type=normal&count=10000").arg(wkCode).arg(QDateTime(wkDate).toMSecsSinceEpoch());
        urlList.append(url);
        int index = 0;
        bool isOver = false;
        foreach (QString url, urlList)
        {
            QNetworkReply *reply = mgr.get(QNetworkRequest(url));
            if(!reply)  continue;
            QEventLoop subloop;
            QObject::connect(reply, SIGNAL(finished()), &subloop, SLOT(quit()));
            QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &subloop, SLOT(quit()));
            QTimer::singleShot(10000, &subloop, SLOT(quit()));
            subloop.exec();
            if(reply->error() == QNetworkReply::NoError && reply->isFinished() && index  == urlList.size()-1)
            {
                QByteArray recv = reply->readAll();
                QJsonDocument doc = QJsonDocument::fromJson(recv);
                if(doc.isObject())
                {
                    QJsonArray array = doc.object().value("data").toObject().value("item").toArray();
                    if(array.size() == 0) isOver = true;
                    for(int i=0; i<array.size(); i++)
                    {
                        QJsonArray subarray = array[i].toArray();
                        if(subarray.size() >= 10)
                        {
// "timestamp",  "volume", "open", "high", "low", "close", "chg",  "percent",  "turnoverrate",  "amount","volume_post","amount_post"],
// 1609689600000,154952285, 43.23,  43.78,  42.02, 43.17,  -0.78,    -1.77,        0.75,         6.608868849E9,null,null]
                            ShareDailyData data;
                            data.mDate = QDateTime::fromMSecsSinceEpoch(subarray[0].toVariant().toLongLong()).toTime_t();
                            data.mVol = subarray[1].toDouble();
                            data.mOpen = subarray[2].toDouble();
                            data.mHigh = subarray[3].toDouble();
                            data.mLow = subarray[4].toDouble();
                            data.mClose = subarray[5].toDouble();
                            double chg = subarray[6].toDouble();
                            data.mLastClose = data.mClose - chg;
                            data.mMoney = subarray[9].toDouble();
                            list.append(data);
                            wkDate = QDateTime::fromTime_t(data.mDate).date().addDays(1);
//                            qDebug()<<QDateTime::fromTime_t(data.mDate).date()<<code<<data.mClose<<data.mRealInfo.mLastClose<<data.mMoney;
                        }
                    }
                }
                reply->abort();
                reply->close();
                delete reply;

                if(wkDate >= QDate::currentDate()) isOver = true;
            }

            index++;

            if(isOver) break;
        }
        urlList.clear();
        if(isOver) break;
    }


    return list;
}

ShareDailyDataList HqInfoParseUtil::getShareHistoryDataFromHexun(const QDate& start, const QString& code)
{
    QString wkCode = code.right(6);
    QString market = (ShareData::shareType(code) & SHARE_SH_TOTAL ? "sse" : "szse");
    wkCode.insert(0, market);

    ShareDailyDataList list;
    QDate last_update_date = start;
    while (1) {
        QString wkURL = QString("http://webstock.quote.hermes.hexun.com/a/kline?code=%1&start=%2&number=1000&type=5")
                .arg(wkCode)
                .arg(last_update_date.toString("yyyyMMdd000000"));

        QRegExp weight_reg("\\,(\\d{1,})\\]\\}");
        QRegExp data_reg("\\[([0-9\,]{1,})\\]");
        QString result = QString::fromUtf8(QHttpGet::getContentOfURL(wkURL));

        int pos = weight_reg.lastIndexIn(result, -1);
        int weight = 0;
        if(pos != -1)
        {
            weight = weight_reg.cap(1).toInt();
        }
        if(weight == 0) return list;   //没有找到的情况
        pos = 0;
        while ((pos = data_reg.indexIn(result, pos)) != -1) {
            pos += data_reg.matchedLength();
            QString src = data_reg.cap(1);
            QStringList src_list = src.split(",");
            if(src_list.size() < 8) continue;
            //{"Time":"时间"},{"LastClose":"前收盘价"},{"Open":"开盘价"},{"Close":"收盘价"},{"High":"最高价"},{"Low":"最低价"},{"Volume":"成交量"},{"Amount":"成交额"}],
            QDate date = QDate::fromString(src_list[0], "yyyyMMdd000000");
            if(date < start) continue;
            if(date == QDate::currentDate()) break;

            ShareDailyData data;
            data.mDate = QDateTime(date).toTime_t();
            data.mClose = src_list[3].toDouble() / weight;
            data.mLastClose = src_list[1].toDouble() / weight;
            data.mMoney = src_list[7].toDouble();
            data.mOpen = src_list[2].toDouble() / weight;
            data.mHigh = src_list[3].toDouble() / weight;
            data.mLow = src_list[4].toDouble() / weight;
            data.mVol = src_list[5].toDouble();

            list.append(data);
//            qDebug()<<date<<code<<data.mClose<<data.mRealInfo.mLastClose<<data.mMoney;
        }
        if(list.size() > 0)
        {
            last_update_date = QDateTime::fromTime_t(list.last().mDate).date().addDays(1);
        }
    }

    return list;

}


bool HqInfoParseUtil::getShareDateRange(const QString& code, QDate& start, QDate& end)
{
    QString wkCode = code.right(6);
    bool found = false;
    //先从和讯网获取
    if(wkCode.size() == 6)
    {
        QString market = (ShareData::shareType(code) & SHARE_SZ_TOTAL? "szse" : "sse");
        wkCode.insert(0, market);
    }
    QString wkURL = QString("http://webstock.quote.hermes.hexun.com/a/kline?code=%1&start=%2&number=-%3&type=5")
            .arg(wkCode)
            .arg(QDate::currentDate().toString("yyyyMMdd000000"))
            .arg(3);

    QRegExp start_end_date_reg("([0-9]{8})0{6},([0-9]{8})0{6}");
    QString result = QString::fromUtf8(QHttpGet::getContentOfURL(wkURL));
    int pos = start_end_date_reg.indexIn(result, 0);
    if(pos >= 0)
    {
        start = QDate::fromString(start_end_date_reg.cap(1), "yyyyMMdd");
        end = QDate::fromString(start_end_date_reg.cap(2), "yyyyMMdd");
        found = true;
        if(start == end) found = false;
    }
    if(!found)
    {
        //再次从雪球获取
        QStringList urlList;
        urlList.append("https://xueqiu.com/");
        QString wkCode = (ShareData::shareType(code) & SHARE_SH_TOTAL? "SH" : "SZ") + code;
        QString url = QString("https://stock.xueqiu.com/v5/stock/chart/kline.json?symbol=%1&begin=%2&period=day&type=normal&count=-10000").arg(wkCode).arg(QDateTime::currentDateTime().toMSecsSinceEpoch());
        urlList.append(url);
        QNetworkAccessManager mgr;
        int index = 0;
        foreach (QString url, urlList) {
            QNetworkReply *reply = mgr.get(QNetworkRequest(url));
            if(!reply)  continue;
            QEventLoop subloop;
            QObject::connect(reply, SIGNAL(finished()), &subloop, SLOT(quit()));
            QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &subloop, SLOT(quit()));
            QTimer::singleShot(10000, &subloop, SLOT(quit()));
            subloop.exec();
            if(reply->error() == QNetworkReply::NoError && reply->isFinished() && index > 0)
            {
                QString result = QString::fromUtf8(reply->readAll());
                QRegExp dateExp("(\[([0-9]{8}0{5}),");
                int pos = dateExp.indexIn(result, 0);
                if(pos > 0)
                {
                    start = QDateTime::fromMSecsSinceEpoch(dateExp.cap(1).toLongLong()).date();
                    found = true;
                }
//                qDebug()<<"found from Xueqiu:"<<result.left(4000)<<pos;
                pos = dateExp.lastIndexIn(result);
                if(pos >= 0)
                {
                    end = QDateTime::fromMSecsSinceEpoch(dateExp.cap(1).toLongLong()).date();
                    found = true;
                }

            }
            if(found)
            {
//                qDebug()<<"found from Xueqiu:"<<start<<end;
            }

            reply->abort();
            reply->close();
            delete reply;
            index++;
        }
    }
    return found;
}

ShareDailyDataList HqInfoParseUtil::getShareHistoryDataFrom163(const QDate &start, const QString &code)
{
    ShareDailyDataList list;
    if(start < QDate::currentDate())
    {
        QString wkCode = (ShareData::shareType(code) & SHARE_SH_TOTAL ? "0" : "1") + code.right(6);
        //取得日线数据
        QString wkURL = QString("http://quotes.money.163.com/service/chddata.html?code=%1&start=%2&end=%3")
                .arg(wkCode).arg(start.toString("yyyyMMdd")).arg(QDate::currentDate().toString("yyyyMMdd"));
        QByteArray recv = QHttpGet::getContentOfURL(wkURL);
        QString result = QString::fromUtf8(recv);

        QStringList lines = result.split("\r\n");
        int size = lines.length();
        for(int i=size-1; i>0; i--)
        {
            QStringList cols = lines[i].split(",");
            if(cols.length() >= 15)
            {
//日期,       股票代码, 名称,  收盘价,  最高价, 最低价, 开盘价, 前收盘, 涨跌额, 涨跌幅,   换手率,  成交量,   成交金额,       总市值,   流通市值,   成交笔数
//2021-02-23,'600036,招商银行,55.55,  56.73, 54.2,  54.29, 54.2,  1.35,  2.4908,  0.462,  95299102, 5306140313.0, 1.40096242314e+12,1.14593786303e+12,None
                QDate curDate = QDate::fromString(cols[0], "yyyy-MM-dd");
                if(curDate.dayOfWeek() == 6 || curDate.dayOfWeek() == 7) continue;
                if(cols[3].toDouble() < 0.001) continue;
                ShareDailyData data;
                data.mDate = QDateTime(curDate).toTime_t();
                data.mClose = cols[3].toDouble();
                data.mHigh = cols[4].toDouble();
                data.mLow = cols[5].toDouble();
                data.mOpen = cols[6].toDouble();
                data.mLastClose = cols[7].toDouble();
                data.mCloseAdjust = data.mClose;
                data.mMoney = cols[12].toDouble();
                data.mVol = cols[11].toDouble();

//                data.mTotalShareCount = qint64(floor(cols[13].toDouble() / data.mClose));
//                qDebug()<<curDate<<code<<data.mClose;
                list.append(data);
            }
        }
    }

    return list;
}

QDateList HqInfoParseUtil::getActiveDateListOfLatestYearPeriod()
{
    QDate cur = QDate::currentDate();
    QDate end = cur.addYears(-1);
    return getActiveDateList(cur, end);
}

//时间上是从from往前推
QDateList HqInfoParseUtil::getActiveDateList(const QDate &from, const QDate &end)
{
    QList<QDate> list;
    int total_gap = from.daysTo(end);
    QString wkURL = QString("http://webstock.quote.hermes.hexun.com/a/kline?code=sse000001&start=%1&number=%2&type=5")
            .arg(from.toString("yyyyMMdd000000"))
            .arg(total_gap);
    QByteArray recv = QHttpGet::getContentOfURL(wkURL);
    QRegExp exp("20\\d{6}000000");
    QString result = QString::fromUtf8(recv);
    int pos = 0;
    while ((pos = exp.indexIn(result, pos)) != -1) {
        QDate date = QDate::fromString(exp.cap(), "yyyyMMdd000000");
        if(date >= end && !list.contains(date)) list.append(date);
        pos += exp.matchedLength();
    }
    if(list.size() > 0) std::sort(list.begin(), list.end(), std::greater<QDate>());

    return list;
}

QDateList HqInfoParseUtil::getActiveDateListOfYear(int year)
{
    return getActiveDateList(QDate(year, 12, 31), QDate(year, 1, 1));
}

bool HqInfoParseUtil::parseShareDataFromSinaA(ShareData& data, const QString& detail)
{
    QStringList detailList = detail.split(QRegExp("[\",=]"), QString::SkipEmptyParts);
    if(detailList.length() < 23) return false;
    data.mCode = detailList[0];
    data.mName = detailList[1];
    data.mRealInfo.mClose = detailList[4].toDouble();
    data.mRealInfo.mLastClose = detailList[3].toDouble();
    data.mRealInfo.mChg = data.mRealInfo.mClose - data.mRealInfo.mLastClose;
    data.mRealInfo.mChgPercent = data.mRealInfo.mChg* 100 / data.mRealInfo.mLastClose ;
    data.mRealInfo.mHigh = detailList[5].toDouble();
    data.mRealInfo.mLow = detailList[6].toDouble();
    double buy = detailList[7].toDouble();
    double sell = detailList[8].toDouble();
    double buy1 = detailList[12].toDouble();
    double sell1 = detailList[22].toDouble();

    //竞价时段的特殊处理
    if(data.mRealInfo.mClose == 0)
    {
        double temp = fmax(buy, buy1);
        if(temp == 0) temp = data.mRealInfo.mLastClose;
        data.mRealInfo.mClose = temp;
        data.mRealInfo.mChg = temp - data.mRealInfo.mLastClose;
        data.mRealInfo.mChgPercent = data.mRealInfo.mChg * 100 / data.mRealInfo.mLastClose ;
    }
    data.mRealInfo.mVol = detailList[9].toInt();
    data.mRealInfo.mMoney = detailList[10].toDouble();
    return true;
}

bool HqInfoParseUtil::parseShareDataFromSinaHK(ShareData& data, const QString &detail)
{
    QStringList detailList = detail.split(QRegExp("[\",=]"), QString::SkipEmptyParts);
    if(detailList.length() < 13) return false;
    data.mCode = detailList[1];
    data.mName = detailList[2];
    data.mRealInfo.mOpen = detailList[3].toDouble();
    data.mRealInfo.mLastClose = detailList[4].toDouble();
    data.mRealInfo.mHigh = detailList[5].toDouble();
    data.mRealInfo.mLow = detailList[6].toDouble();
    data.mRealInfo.mClose = detailList[7].toDouble();
    data.mRealInfo.mChg = detailList[8].toDouble();
    data.mRealInfo.mChgPercent = detailList[9].toDouble();
    data.mRealInfo.mMoney = detailList[12].toDouble() * 1000;
    return true;
}
