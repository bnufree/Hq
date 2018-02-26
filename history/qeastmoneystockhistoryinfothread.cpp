#include "qeastmoneystockhistoryinfothread.h"
#include <QDateTime>
#include <QDebug>
#include "dbservices/dbservices.h"
#include "qhttpget.h"
#include "utils/hqutils.h"

QEastmoneyStockHistoryInfoThread::QEastmoneyStockHistoryInfoThread(const QString& code, const QDate& date, StockDataList* list, QObject* parent) :
    mCode(code),
    mStartDate(date),
    mHistoryListPtr(list),
    mParent(parent),
    QRunnable()
{
    mCode = code;
    if(mCode.length() > 6)
    {
        mCode = mCode.right(6);
    }
}

QEastmoneyStockHistoryInfoThread::~QEastmoneyStockHistoryInfoThread()
{

}

void QEastmoneyStockHistoryInfoThread::run()
{
    QDate start = mStartDate;
    QDate end = QDate::currentDate();

    if(!mHistoryListPtr) goto FUNC_END;

    //只更新基本几只ETF
    if(mCode.left(1) == "5" || mCode.left(1) == "1")
    {
         QRegExp exp("(510(050|300|900|500))|(1599(15|20))");
         if(!exp.exactMatch(mCode))goto FUNC_END;
    }
    //检查日线数据是否需要更新    
    if(start < end)
    {
        QString wkCode;
        if(mCode.left(1) == "6" || mCode.left(1) == "5")
        {
            wkCode = "0" + mCode;
        } else
        {
            wkCode = "1" + mCode;
        }
        //取得日线数据
        QString wkURL = QString("http://quotes.money.163.com/service/chddata.html?code=%1&start=%2&end=%3")
                .arg(wkCode).arg(start.toString("yyyyMMdd")).arg(end.toString("yyyyMMdd"));
        QString result = QString::fromLocal8Bit(QHttpGet::getContentOfURL(wkURL));
        QStringList lines = result.split("\r\n");
        int index = 0;
        for(int i=1; i<lines.length(); i++)
        {
            QStringList cols = lines[i].split(",");
            if(cols.length() >= 15)
            {
                bool found = false;
                QDate curDate = QDate::fromString(cols[0], "yyyy-MM-dd");
                if(HqUtils::isWeekend(curDate)) continue;
                if(cols[3].toDouble() == 0) continue;
                StockData &data = mHistoryListPtr->valueOfDate(curDate);
                data.mDate = curDate;
                data.mCode = mCode;
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
                double price = data.mCur;
                data.mTotalShare = cols[13].toDouble() / price;
                data.mMutableShare = cols[14].toDouble() / price;
                list.append(data);
            }
        }
        if(list.size() > 0)
        {
            emit DATA_SERVICE->signalRecvShareHistoryInfos(mCode, list, mDelDB);
        }
    }
    emit DATA_SERVICE->signalUpdateShareinfoWithHistory(mCode);
FUNC_END:
    if(mParent)
    {
        QMetaObject::invokeMethod(mParent, "slotUpdateShareHistoryProcess", Qt::DirectConnection, Q_ARG(QString,mCode ));
    }
    return;
}

QString QEastmoneyStockHistoryInfoThread::getCode()
{
    return mCode;
}



