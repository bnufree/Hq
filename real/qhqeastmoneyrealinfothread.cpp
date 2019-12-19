#include "qhqeastmoneyrealinfothread.h"
#include "utils/qhttpget.h"
#include <QTextCodec>
#include "dbservices/dbservices.h"

QHqEastMoneyRealInfoThread::QHqEastMoneyRealInfoThread(QObject *parent) : QThread(parent)
{

}


void QHqEastMoneyRealInfoThread::run()
{
    while (1) {
        parseHttpUrl("http://nufm.dfcfw.com/EM_Finance2014NumericApplication/JS.aspx?cb=&type=CT&token=4f1862fc3b5e77c150a2b985b12db0fd&sty=FCOIATC&js=&cmd=C._A&st=(Code)&sr=-1&p=1&ps=20000");
        sleep(3);
    }
}

void QHqEastMoneyRealInfoThread::parseHttpUrl(const QString &url)
{
    QTime t;
    t.start();
    QStringList top10Keys = DATA_SERVICE->getHshtTop10List();
    QByteArray http = QHttpGet::getContentOfURL(url);
    QTextCodec *UTF8 = QTextCodec::codecForName("UTF-8");
    QString result = QString::fromUtf8(http);
    QString utf8_result = QString::fromUtf8(UTF8->fromUnicode(result));
    utf8_result.remove(QRegExp("[\\(\\[\\]\\)]"));
    QStringList resultlist = utf8_result.split("\",\"", QString::SkipEmptyParts);
    for(int i=0; i<resultlist.size(); i++)
    {
        result = resultlist[i];

        //\"1,688399,硕世生物,50.75,0.22,0.44,10416,52176804,2.75,50.99,49.60,50.10,50.53,0.34,0.77,7.81,44.83,3.01,2974965000,676927352,-9.51%,-9.51%,0.26,2019-12-05,2019-12-10 10:16:56,10416"
//result: "1,688389,普门科技,15.73,0.12,0.77,10854,16928156,2.31,15.78,15.42,15.60,15.61,0.00,1.78,3.12,75.35,6.54,6641205807,546645877,-21.09%,-20.48%,-0.06,2019-11-05,2019-12-10 10:16:56,10854"
        result.replace("\"", "");
        QStringList detailList = result.split(",", QString::SkipEmptyParts);
        if(detailList.length() < 20) continue;
//        "2,			000001,	平安银行,	15.41,	-0.19,	-1.22,		605993,		935135952,		1.73,	15.62,	15.35,	15.62,	15.60,
//        市场类型	代码	 名称		 现价	涨跌     涨跌幅		 成交量			成交额		振幅	最高	最低	开盘	昨收
//        0.06,       1.08,    0.31,       9.50,     1.12,   299045196470,299042644235,   4.9%,   67.68%,  -0.06,1991-04-03,2019-12-09 15:00:00,605993"
//                    量比	  换手      市盈率    市净率	总市值		流通市值
        QString code = detailList[1];
        ShareData &data = DATA_SERVICE->getShareData(code);
        data.mName = detailList[2];
        data.mCur = detailList[3].toDouble();
        data.mChg = detailList[4].toDouble();
        data.mVol = detailList[6].toInt();
        data.mMoney = detailList[7].toDouble();
        data.mChgPercent = detailList[5].toDouble();
        data.mHigh = detailList[9].toDouble();
        data.mLow = detailList[10].toDouble();
        data.mOpen = detailList[11].toDouble();
        data.mLastClose = detailList[12].toDouble();
        data.mHsl = detailList[15].toDouble();
        data.mMoneyRatio = 0.0;
        if(data.mHistory.mLastMoney> 0){
            data.mMoneyRatio = data.mMoney / data.mHistory.mLastMoney;
        }

        if(data.mCur != 0)
        {
            data.mGXL = data.mBonusData.mXJFH / data.mCur;
        }
        data.mTotalCap = detailList[18].toDouble();
        data.mMutalbleCap = detailList[19].toDouble();
        if(data.mFinanceData.mMutalShare > 0)
        {
            data.mHsl = data.mVol / (double)(data.mFinanceData.mMutalShare);
        }
        if(data.mProfit == 0)
        {
            data.mProfit = DATA_SERVICE->getProfit(code);
        }
        data.mForeignCap = data.mHsgtData.mVolTotal * data.mCur ;
        data.mHsgtData.mVolChange = data.mHsgtData.mVolCh1;
        data.mForeignCapChg = data.mHsgtData.mVolChange * data.mCur ;
        if(data.mHistory.mWeekDayPrice > 0)
        {
            data.mHistory.mChgPersFromWeek = (data.mCur - data.mHistory.mWeekDayPrice) * 100.0 / data.mHistory.mWeekDayPrice;
        } else
        {
//            data.mHistory.mChgPersFromWeek = data.mChgPercent;
        }
        if(data.mHistory.mMonthDayPrice> 0)
        {
            data.mHistory.mChgPersFromMonth= (data.mCur - data.mHistory.mMonthDayPrice) * 100.0 / data.mHistory.mMonthDayPrice;
        } else
        {
//            data.mHistory.mChgPersFromMonth= data.mChgPercent;
        }

        if(data.mHistory.mYearDayPrice > 0)
        {
            data.mHistory.mChgPersFromYear = (data.mCur - data.mHistory.mYearDayPrice) * 100.0 / data.mHistory.mYearDayPrice;
        } else
        {
//            data.mHistory.mChgPersFromYear = data.mChgPercent;
        }
//        data.mUpdateTime = QDateTime::currentMSecsSinceEpoch();
        if(top10Keys.contains(data.mCode.right(6)))
        {
            data.mHsgtData.mIsTop10 = true;
        } else
        {
            data.mHsgtData.mIsTop10 = false;
        }
    }
    qDebug()<<"total size:"<<resultlist.size();
}
