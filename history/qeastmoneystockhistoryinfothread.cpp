#include "qeastmoneystockhistoryinfothread.h"
#include <QDateTime>
#include <QDebug>
#include "hqdatadefines.h"
#include "dbservices.h"
#include "qhttpget.h"

QEastmoneyStockHistoryInfoThread::QEastmoneyStockHistoryInfoThread(const QString& code, const QDate& date,  QObject *parent) : mCode(code), QThread(parent)
{
    mCode = code;
    if(mCode.length() != 6)
    {
        mCode = mCode.right(6);
    }
    mDate = date;
}

QEastmoneyStockHistoryInfoThread::~QEastmoneyStockHistoryInfoThread()
{

}

void QEastmoneyStockHistoryInfoThread::run()
{
    //QDate lastDate = DATA_SERVICE->getLastUpdateDateOfShareHistory(mCode);
    //检查是否需要更新
    if(mDate >= QDate::currentDate()) return;

    //开始更新
    QDate start = mDate.addDays(1);
    QDate end = QDate::currentDate();
    mCode = mCode.right(6);
    if(mCode.left(1) == "6" || mCode.left(1) == "5")
    {
        mCode = "0" + mCode;
    } else
    {
        mCode = "1" + mCode;
    }
    QString wkURL = QString("http://quotes.money.163.com/service/chddata.html?code=%1&start=%2&end=%3")
            .arg(mCode).arg(start.toString("yyyyMMdd")).arg(end.toString("yyyyMMdd"));
    QString result = QString::fromLocal8Bit(QHttpGet().getContent(wkURL));
    QStringList lines = result.split("\r\n");
    StockDataList list;
    for(int i=1; i<lines.length(); i++)
    {
        QStringList cols = lines[i].split(",");
        if(cols.length() >= 15)
        {
            StockData data;
            data.date = QDate::fromString(cols[0], "yyyy-MM-dd");
            data.code = cols[1].right(6);
            data.name = cols[2];
            data.cur = cols[3].toDouble();
            data.high = cols[4].toDouble();
            data.low = cols[5].toDouble();
            data.open = cols[6].toDouble();
            data.last_close = cols[7].toDouble();
            data.chg = cols[8].toDouble();
            data.per = cols[9].toDouble();
            data.hsl = cols[10].toDouble();
            data.vol = cols[11].toLongLong();
            data.money = cols[12].toDouble();
            data.totalshare = cols[13].toDouble() / data.cur;
            data.mutableshare = cols[14].toDouble() / data.cur;
            list.append(data);
        }
    }

    emit DATA_SERVICE->signalRecvShareHistoryInfos(list);
}




