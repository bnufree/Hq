#include "qsinastkinfothread.h"
#include <QDebug>
#include <QDateTime>
#include "qexchangedatamanage.h"
#include "dbservices/dbservices.h"
#include "data_structure/hqutils.h"
#include "data_structure/sharedata.h"
#include "utils/qhttpget.h"


QSinaStkInfoThread::QSinaStkInfoThread(const QStringList& list, bool send, QObject *parent)
  : QThread(parent)
  , mSendResFlag(send)
  , mCancel(false)
{
    mStkList.clear();
    setStkList(list);
    //setStackSize(20 * 1000000);
}

QSinaStkInfoThread::~QSinaStkInfoThread()
{
    qDebug()<<"hq info thread discontructor now"<<this;

}

void QSinaStkInfoThread::run()
{
    //开始更新

    while(1)
    {
        if(mCancel) break;
        QString url("http://hq.sinajs.cn/?list=%1");
        QTime t;
        t.start();
        int index  = 0;
        while (index < mStkList.size()) {
            QStringList list = mStkList[index];
            QString wkURL = url.arg(list.join(","));

            slotRecvHttpContent(QHttpGet::getContentOfURL(wkURL));
            index++;
        }        
//        qDebug()<<"parse real info elapsed:"<<t.elapsed()<<mStkList.size();
    }

}


void QSinaStkInfoThread::setStkList(const QStringList &list)
{
    //首先给数据添加交易所标识
    QStringList resList;
    foreach (QString code, list) {
        code = code.toLower();
        if(code.startsWith("sh") || code.startsWith("sz") || code.startsWith("hk"))
        {
            resList.append(code);
        }
        if(code.length() == 6)
        {
            resList.append(ShareData::prefixCode(code)+code);
        } else if(code.length() == 5)
        {
            resList.append("hk" + code);
        }
    }
    //开始进行分组
    int group_size = 100;
    int index = 0;
    while (1) {
        mStkList.append(resList.mid(index, group_size));
        index += group_size;
        if(index >= resList.size()) break;
    }
}


void QSinaStkInfoThread::slotRecvHttpContent(const QByteArray &bytes)
{
    QString result =  QString::fromUtf8(bytes);
    //先换行
    QStringList resultlist = result.split(QRegExp("[\\n;]"), QString::SkipEmptyParts);
    //再分割具体的字段
    QStringList top10Keys = DATA_SERVICE->getHshtTop10List();
    ShareDataList datalist;
    foreach (QString detail, resultlist)
    {
        //var hq_str_sz399006="创业板指,1647.848,1692.416,1680.387,1718.384,1635.450,0.000,0.000,10414641478,126326323478.810
        detail.replace("var hq_str_", "");
//        qDebug()<<detail;
        QStringList detailList = detail.split(QRegExp("[\",=]"), QString::SkipEmptyParts);
        if(detailList.length() < 11) continue;
        QString code = detailList[0];
        ShareData &data = DATA_SERVICE->getShareData(code);
        //qDebug()<<data->mCode<<data->mName<<data->mShareType;
        data.mName = detailList[1];
        data.mRealInfo.mClose = detailList[4].toDouble();
        data.mRealInfo.mLastClose = detailList[3].toDouble();
        data.mRealInfo.mChg = detailList[4].toDouble() - data.mRealInfo.mLastClose;
        data.mRealInfo.mChgPercent = data.mRealInfo.mChg * 100 / detailList[3].toDouble() ;
        data.mRealInfo.mHigh = detailList[5].toDouble();
        data.mRealInfo.mLow = detailList[6].toDouble();
        double buy = detailList[7].toDouble();
        double sell = detailList[8].toDouble();
        double buy1 = detailList[12].toDouble();
        double sell1 = detailList[22].toDouble();

        //竞价时段的特殊处理
        if(QTime::currentTime().toString("hhmmss") >= "091500" && QTime::currentTime().toString("hhmmss") <= "092500")
        {
            double temp = fmax(buy, buy1);
            if(temp == 0) temp = data.mRealInfo.mLastClose;
            data.mRealInfo.mClose = temp;
            data.mRealInfo.mChg = detailList[8].toDouble() - data.mRealInfo.mLastClose;
            data.mRealInfo.mChgPercent = data.mRealInfo.mChg * 100 / detailList[3].toDouble() ;
        }
        data.mRealInfo.mVol = detailList[9].toInt();
        data.mRealInfo.mMoney = detailList[10].toDouble();
        data.mRealInfo.mHsl = 0.0;
        data.mRealInfo.mMoneyRatio = 0.0;
        if(data.mCounterInfo.mLastMoney> 0){
            data.mRealInfo.mMoneyRatio = data.mRealInfo.mMoney / data.mCounterInfo.mLastMoney;
        }

        if(data.mRealInfo.mClose != 0)
        {
//            data.mGXL = data.mBonusData.mXJFH / data.mRealInfo.mClose;
        }
        data.mRealInfo.mTotalCap = data.mRealInfo.mClose * data.mZGB;
        data.mRealInfo.mMutalbleCap = data.mRealInfo.mClose * data.mLTGB;
        if(data.mLTGB > 0)
        {
            data.mRealInfo.mHsl = data.mRealInfo.mVol / (double)(data.mLTGB);
        }
        if(data.mProfit == 0)
        {
            data.mProfit = DATA_SERVICE->getProfit(code);
        }
        data.mForeignInfo.mCap = data.mForeignInfo.mVol * data.mRealInfo.mClose ;
//        data.mForeignCapChg = data.mHsgtData.volChg("1") * data.mRealInfo.mClose ;
        if(data.mCounterInfo.mWeekDayPrice > 0)
        {
            data.mCounterInfo.mWeekChgPer = (data.mRealInfo.mClose - data.mCounterInfo.mWeekDayPrice) * 100.0 / data.mCounterInfo.mWeekDayPrice;
        } else
        {
//            data.mHistory.mChgPersFromWeek = data.mChgPercent;
        }
        if(data.mCounterInfo.mMonthDayPrice> 0)
        {
            data.mCounterInfo.mMonthChgPer = (data.mRealInfo.mClose - data.mCounterInfo.mMonthDayPrice) * 100.0 / data.mCounterInfo.mMonthDayPrice;
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
        if(mSendResFlag) datalist.append(data);
    }
    if(mSendResFlag)  emit sendStkDataList(datalist);
}

int QSinaStkInfoThread::getStkCount()
{
    return mStkList.count();
}


