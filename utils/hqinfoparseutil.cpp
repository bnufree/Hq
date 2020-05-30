#include "hqinfoparseutil.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonArray>
#include "qhttpget.h"
#include <QTextCodec>
#include "data_structure/shareworkingdatetime.h"
#include <math.h>

HqInfoParseUtil::HqInfoParseUtil()
{

}

ShareHistoryFileDataList HqInfoParseUtil::getShareHistoryData(const QDate& start, const QString& code)
{
    return getShareHistoryDataFromHexun(start, code);
}

ShareHistoryFileDataList HqInfoParseUtil::getShareHistoryDataFromXueqiu(const QDate &start, const QString &code)
{
    ShareHistoryFileDataList list;
    //再次从雪球获取
    QStringList urlList;
    urlList.append("https://xueqiu.com/");
    QString wkCode = (code.left(1).toInt() >= 5 ? "SH" : "SZ") + code;
    QString url = QString("https://stock.xueqiu.com/v5/stock/chart/kline.json?symbol=%1&begin=%2&period=day&type=normal&count=10000").arg(wkCode).arg(QDateTime(start).toMSecsSinceEpoch());
    urlList.append(url);
    QNetworkAccessManager mgr;
    int index = 0;
    foreach (QString url, urlList)
    {
        QNetworkReply *reply = mgr.get(QNetworkRequest(url));
        if(!reply)  continue;
        QEventLoop subloop;
        QObject::connect(reply, SIGNAL(finished()), &subloop, SLOT(quit()));
        QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &subloop, SLOT(quit()));
        QTimer::singleShot(10000, &subloop, SLOT(quit()));
        subloop.exec();
        if(reply->error() == QNetworkReply::NoError && reply->isFinished() && index > 0)
        {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            if(doc.isObject())
            {
                QJsonArray array = doc.object().value("item").toArray();
                for(int i=0; i<array.size(); i++)
                {
                    QJsonArray subarray = array[i].toArray();
                    if(subarray.size() >= 10)
                    {
                        ShareHistoryFileData data;
                        data.mDate = QDateTime::fromMSecsSinceEpoch(subarray[0].toVariant().toLongLong()).toTime_t();
                        data.mClose = subarray[5].toDouble();
                        double chg = subarray[6].toDouble();
                        data.mLastClose = data.mClose - chg;
                        data.mMoney = subarray[9].toDouble();
                        list.append(data);
                    }
                }
            }
            reply->abort();
            reply->close();
            delete reply;
            index++;
        }
    }

    return list;
}

ShareHistoryFileDataList HqInfoParseUtil::getShareHistoryDataFromHexun(const QDate& start, const QString& code)
{
    QString wkCode = code;
    if(wkCode.size() == 6)
    {
        QString market = code.toInt() < 500000 ? "szse" : "sse";
        wkCode.insert(0, market);
    }

    ShareHistoryFileDataList list;
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
            if(date == ShareWorkingDate::currentDate().date()) break;

            ShareHistoryFileData data;
            data.mDate = QDateTime(date).toTime_t();
            data.mClose = src_list[3].toDouble() / weight;
            data.mLastClose = src_list[1].toDouble() / weight;
            data.mMoney = src_list[7].toDouble();
            list.append(data);
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
        QString market = code.toInt() < 500000 ? "szse" : "sse";
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
        QString wkCode = (code.left(1).toInt() >= 5 ? "SH" : "SZ") + code;
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

ShareHistoryFileDataList HqInfoParseUtil::getShareHistoryDataFrom163(const QDate &start, const QString &code)
{
    ShareHistoryFileDataList list;
    if(start < ShareWorkingDate::currentDate().date())
    {
        QString wkCode;
        if(code.left(1) == "6" || code.left(1) == "5")
        {
            wkCode = "0" + code;
        } else
        {
            wkCode = "1" + code;
        }
        //取得日线数据
        QString wkURL = QString("http://quotes.money.163.com/service/chddata.html?code=%1&start=%2&end=%3")
                .arg(wkCode).arg(start.toString("yyyyMMdd")).arg(ShareWorkingDate::currentDate().date().toString("yyyyMMdd"));
        QByteArray recv = QHttpGet::getContentOfURL(wkURL);
        QTextCodec* gbk = QTextCodec::codecForName("GBK");
        QTextCodec* utf8 = QTextCodec::codecForName("UTF-8");
        QString result = QString::fromUtf8(utf8->fromUnicode(gbk->toUnicode(recv)));

        QStringList lines = result.split("\r\n");
        int size = lines.length();
        for(int i=size-1; i>0; i--)
        {
            QStringList cols = lines[i].split(",");
            if(cols.length() >= 15)
            {
                QDate curDate = QDate::fromString(cols[0], "yyyy-MM-dd");
                if(curDate.dayOfWeek() == 6 || curDate.dayOfWeek() == 7) continue;
                if(cols[3].toDouble() < 0.001) continue;
                ShareHistoryFileData data;
                data.mDate = QDateTime(curDate).toTime_t();
                data.mClose = cols[3].toDouble();
                data.mLastClose = cols[7].toDouble();
                data.mCloseAdjust = data.mClose;
                data.mMoney = cols[12].toDouble();
//                data.mTotalShareCount = qint64(floor(cols[13].toDouble() / data.mClose));
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
    QTextCodec* gbk = QTextCodec::codecForName("GBK");
    QTextCodec* utf8 = QTextCodec::codecForName("UTF-8");
    QString result = QString::fromUtf8(utf8->fromUnicode(gbk->toUnicode(recv)));
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
    data.mCur = detailList[4].toDouble();
    data.mLastClose = detailList[3].toDouble();
    data.mChg = data.mCur - data.mLastClose;
    data.mChgPercent = data.mChg* 100 / data.mLastClose ;
    data.mHigh = detailList[5].toDouble();
    data.mLow = detailList[6].toDouble();
    double buy = detailList[7].toDouble();
    double sell = detailList[8].toDouble();
    double buy1 = detailList[12].toDouble();
    double sell1 = detailList[22].toDouble();

    //竞价时段的特殊处理
    if(data.mCur == 0)
    {
        double temp = fmax(buy, buy1);
        if(temp == 0) temp = data.mLastClose;
        data.mCur = temp;
        data.mChg = temp - data.mLastClose;
        data.mChgPercent = data.mChg * 100 / data.mLastClose ;
    }
    data.mVol = detailList[9].toInt();
    data.mMoney = detailList[10].toDouble();
    return true;
}

bool HqInfoParseUtil::parseShareDataFromSinaHK(ShareData& data, const QString &detail)
{
    QStringList detailList = detail.split(QRegExp("[\",=]"), QString::SkipEmptyParts);
    if(detailList.length() < 13) return false;
    data.mCode = detailList[1];
    data.mName = detailList[2];
    data.mOpen = detailList[3].toDouble();
    data.mLastClose = detailList[4].toDouble();
    data.mHigh = detailList[5].toDouble();
    data.mLow = detailList[6].toDouble();
    data.mCur = detailList[7].toDouble();
    data.mChg = detailList[8].toDouble();
    data.mChgPercent = detailList[9].toDouble();
    data.mMoney = detailList[12].toDouble() * 1000;
    return true;
}
