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


ShareHistoryFileDataList HqInfoParseUtil::getShareHistoryDataFromHexun(const QDate& start, const QString& code)
{
    ShareHistoryFileDataList list;
    QDateList dates = ShareWorkingDate::getHisWorkingDay();
    if(dates.size() > 0)
    {
        if(dates.first() == QDate::currentDate()) dates.removeFirst();
    }
    int num = 0;
    for(int i=0; i<dates.size(); i++)
    {
        if(dates[i] >= start) num++;
    }
    if(num  == 0) return list;
    num *= (-1);
    QString wkCode = code;
    if(wkCode.size() == 6)
    {
        QString market = code.toInt() < 500000 ? "szse" : "sse";
        wkCode.insert(0, market);
    }
    QString wkURL = QString("http://webstock.quote.hermes.hexun.com/a/kline?code=%1&start=%2&number=%3&type=5")
            .arg(wkCode)
            .arg(dates.first().toString("yyyyMMdd000000"))
            .arg(num);

    QRegExp weight_reg("\\,(\\d{1,})\\]\\}");
    QRegExp data_reg("\\[([0-9\,]{1,})\\]");
    QString result = QString::fromUtf8(QHttpGet::getContentOfURL(wkURL));

    int pos = weight_reg.lastIndexIn(result, -1);
    int weight = 0;
    if(pos != -1)
    {
        weight = weight_reg.cap(1).toInt();
    }
    if(weight == 0) return list;
    pos = 0;
    while ((pos = data_reg.indexIn(result, pos)) != -1) {
        pos += data_reg.matchedLength();
        QString src = data_reg.cap(1);
        QStringList src_list = src.split(",");
        if(src_list.size() != 8) continue;
        //{"Time":"时间"},{"LastClose":"前收盘价"},{"Open":"开盘价"},{"Close":"收盘价"},{"High":"最高价"},{"Low":"最低价"},{"Volume":"成交量"},{"Amount":"成交额"}],
        QDate date = QDate::fromString(src_list[0], "yyyyMMdd000000");
        if(date < start) continue;

        ShareHistoryFileData data;
        data.mDate = QDateTime(date).toTime_t();
        data.mClose = src_list[3].toDouble() / weight;
        data.mLastClose = src_list[1].toDouble() / weight;
        data.mMoney = src_list[7].toDouble();
        list.append(data);
    }

    return list;
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
                data.mTotalShareCount = qint64(floor(cols[13].toDouble() / data.mClose));
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
