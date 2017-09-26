#include "qeastmoneystockhistoryinfothread.h"
#include <QDateTime>
#include <QDebug>
#include "../hqdatadefines.h"
#include "../dbservices.h"
#include "../qhttpget.h"

QEastmoneyStockHistoryInfoThread::QEastmoneyStockHistoryInfoThread(const QString& code, const QDate& date,  QObject *parent) :
    mCode(code),
    QThread(parent)
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
    QDate start = mDate.addDays(1);
    QDate end = QDate::currentDate();
    if(start < end)
    {

//        qDebug()<<__FUNCTION__<<__LINE__<<mCode;
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

        QString result = QString::fromLocal8Bit(QHttpGet::getContentOfURL(wkURL));
        QStringList lines = result.split("\r\n");
        StockDataList list;
        for(int i=1; i<lines.length(); i++)
        {
            QStringList cols = lines[i].split(",");
            if(cols.length() >= 15)
            {
                StockData data;
                data.mDate = QDate::fromString(cols[0], "yyyy-MM-dd");
                data.mCode = cols[1].right(6);
                data.mName = cols[2];
                data.mCur = cols[3].toDouble();
                data.mHigh = cols[4].toDouble();
                data.mLow = cols[5].toDouble();
                data.mOpen = cols[6].toDouble();
                data.mLastClose = cols[7].toDouble();
                data.mChg = cols[8].toDouble();
                data.mChgPercent = cols[9].toDouble();
                data.mHsl = cols[10].toDouble();
                data.mVol = cols[11].toLongLong();
                data.mMoney = cols[12].toDouble();
                data.mTotalShare = cols[13].toDouble() / data.mCur;
                data.mMutableShare = cols[14].toDouble() / data.mCur;
                list.append(data);
            }
        }

        emit DATA_SERVICE->signalRecvShareHistoryInfos(list);
    }

    //查询数据库更新历史信息
    emit DATA_SERVICE->signalUpdateStkBaseinfoWithHistory(mCode.right(6));

}

QString QEastmoneyStockHistoryInfoThread::getCode()
{
    return mCode;
}



