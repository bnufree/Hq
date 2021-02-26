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
    QString utf8_result = QString::fromUtf8(http);
    utf8_result.remove(QRegExp("[\\(\\[\\]\\)]"));
    QStringList resultlist = utf8_result.split("\",\"", QString::SkipEmptyParts);
    for(int i=0; i<resultlist.size(); i++)
    {
        QString result = resultlist[i];

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
        data.mRealInfo.mClose = detailList[3].toDouble();
        data.mRealInfo.mChg = detailList[4].toDouble();
        data.mRealInfo.mVol = detailList[6].toInt();
        data.mRealInfo.mMoney = detailList[7].toDouble();
        data.mRealInfo.mChgPercent = detailList[5].toDouble();
        data.mRealInfo.mHigh = detailList[9].toDouble();
        data.mRealInfo.mLow = detailList[10].toDouble();
        data.mRealInfo.mOpen = detailList[11].toDouble();
        data.mRealInfo.mLastClose = detailList[12].toDouble();
        data.mRealInfo.mHsl = detailList[15].toDouble();
        data.mRealInfo.mMoneyRatio = 0.0;
        if(data.mCounterInfo.mLastMoney> 0){
            data.mRealInfo.mMoneyRatio = data.mRealInfo.mMoney / data.mCounterInfo.mLastMoney;
        }

        if(data.mRealInfo.mClose != 0)
        {
//            data.mGXL = data.mBonusData.mXJFH / data.mCur;
        }
        data.mRealInfo.mTotalCap = detailList[18].toDouble();
        data.mRealInfo.mMutalbleCap = detailList[19].toDouble();
        if(data.mLTGB > 0)
        {
            data.mRealInfo.mHsl = data.mRealInfo.mVol / data.mLTGB;
        }
        if(data.mProfit == 0)
        {
            data.mProfit = DATA_SERVICE->getProfit(code);
        }
        data.mForeignInfo.mCap = data.mForeignInfo.mVol * data.mRealInfo.mClose ;
//        data.mForeignCapChg = data.mHsgtData.volChg("1") * data.mCur ;
        if(data.mCounterInfo.mWeekDayPrice > 0)
        {
            data.mCounterInfo.mWeekChgPer = (data.mRealInfo.mClose - data.mCounterInfo.mWeekDayPrice) * 100.0 / data.mCounterInfo.mWeekDayPrice;
        } else
        {
//            data.mHistory.mChgPersFromWeek = data.mChgPercent;
        }
        if(data.mCounterInfo.mMonthDayPrice> 0)
        {
            data.mCounterInfo.mMonthChgPer= (data.mRealInfo.mClose - data.mCounterInfo.mMonthDayPrice) * 100.0 / data.mCounterInfo.mMonthDayPrice;
        } else
        {
//            data.mHistory.mChgPersFromMonth= data.mChgPercent;
        }

        if(data.mCounterInfo.mYearDayPrice > 0)
        {
            data.mCounterInfo.mYearChgPer = (data.mRealInfo.mClose - data.mCounterInfo.mYearDayPrice) * 100.0 / data.mCounterInfo.mYearDayPrice;
        } else
        {
//            data.mHistory.mChgPersFromYear = data.mChgPercent;
        }
//        data.mUpdateTime = QDateTime::currentMSecsSinceEpoch();
        if(top10Keys.contains(data.mCode.right(6)))
        {
            data.mForeignInfo.mIsTop10 = true;
        } else
        {
            data.mForeignInfo.mIsTop10 = false;
        }
    }
    qDebug()<<"total size:"<<resultlist.size();
}
