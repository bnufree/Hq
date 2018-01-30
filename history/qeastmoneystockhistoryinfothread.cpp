#include "qeastmoneystockhistoryinfothread.h"
#include <QDateTime>
#include <QDebug>
#include "dbservices/dbservices.h"
#include "qhttpget.h"
#include "utils/hqutils.h"

QEastmoneyStockHistoryInfoThread::QEastmoneyStockHistoryInfoThread(const QString& code) :
    mCode(code),
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
    if(mCode.left(1) == "5" || mCode.left(1) == "1") return;
    QDate lastDate = DATA_SERVICE->getLastUpdateDateOfShareHistory(mCode);
    //检查日线数据是否需要更新
    QDate start = lastDate.addDays(1);
    QDate end = QDate::currentDate();
    if(start < end)
    {
        mCode = mCode.right(6);
//        QString wkCode;
//        if(mCode.left(1) == "6" || mCode.left(1) == "5")
//        {
//            wkCode = "0" + mCode;
//        } else
//        {
//            wkCode = "1" + mCode;
//        }
//        //取得日线数据
//        QString wkURL = QString("http://quotes.money.163.com/service/chddata.html?code=%1&start=%2&end=%3")
//                .arg(wkCode).arg(start.toString("yyyyMMdd")).arg(end.toString("yyyyMMdd"));

//        //qDebug()<<wkURL;
//        QString result = QString::fromLocal8Bit(QHttpGet::getContentOfURL(wkURL));
//        QStringList lines = result.split("\r\n");
        QMap<QString, StockData> list;
//        for(int i=1; i<lines.length(); i++)
//        {
//            QStringList cols = lines[i].split(",");
//            if(cols.length() >= 15)
//            {
//                StockData data;
//                data.mDate = QDate::fromString(cols[0], "yyyy-MM-dd");
//                if(HqUtils::isWeekend(data.mDate)) continue;
//                data.mCode = cols[1].right(6);
//                data.mName = cols[2];
//                data.mCur = cols[3].toDouble();
//                data.mHigh = cols[4].toDouble();
//                data.mLow = cols[5].toDouble();
//                data.mOpen = cols[6].toDouble();
//                data.mLastClose = cols[7].toDouble();
//                data.mChg = cols[8].toDouble();
//                data.mChgPercent = cols[9].toDouble();
//                data.mHsl = cols[10].toDouble();
//                data.mVol = cols[11].toLongLong();
//                data.mMoney = cols[12].toDouble();
//                double price = data.mCur;
//                if(price == 0) price = data.mLastClose;
//                data.mTotalShare = cols[13].toDouble() / price;
//                data.mMutableShare = cols[14].toDouble() / price;
//                list[data.mDate.toString("yyyy-MM-dd")] = data;
//            }
//        }

        //qDebug()<<mCode<<lastDate<<list.values().size();
        //取得外资此股情况更新
        QString wkURL = QString("http://dcfm.eastmoney.com//em_mutisvcexpandinterface/api/js/get?"
                        "type=HSGTHDSTA&token=70f12f2f4f091e459a279469fe49eca5&filter="
                        "(SCODE='%1')(HDDATE>=^%2^)&st=HDDATE&sr=-1&p=1&ps=5000&js=(x)")
                .arg(mCode).arg(start.toString("yyyy-MM-dd"));
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(QHttpGet::getContentOfURL(wkURL), &err);
        if(err.error == QJsonParseError::NoError)
        {
            if(doc.isArray())
            {
                //开始解析
                QJsonArray result = doc.array();
                for(int i=0; i<result.size(); i++)
                {
                    QJsonObject obj = result.at(i).toObject();
                    QString dateStr = obj.value("HDDATE").toString().left(10);
                    StockData data;
                    data.mDate = QDate::fromString(dateStr, "yyyy-MM-dd");
                    if(HqUtils::isWeekend(data.mDate)) continue;
                    data.mForeignVol = obj.value("SHAREHOLDSUM").toVariant().toLongLong();
                    data.mClose = obj.value("CLOSEPRICE").toDouble();
                    data.mChgPercent =obj.value("ZDF").toDouble();
                    list[dateStr] = data;

                }
            }
        }
        emit DATA_SERVICE->signalRecvShareHistoryInfos(mCode, list.values());
    }

    //查询数据库更新历史信息
    emit DATA_SERVICE->signalUpdateShareinfoWithHistory(mCode.right(6));
}

QString QEastmoneyStockHistoryInfoThread::getCode()
{
    return mCode;
}



