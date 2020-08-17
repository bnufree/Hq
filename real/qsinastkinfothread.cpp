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
        data.mCur = detailList[4].toDouble();
        data.mLastClose = detailList[3].toDouble();
        data.mChg = detailList[4].toDouble() - data.mLastClose;
        data.mChgPercent = data.mChg * 100 / detailList[3].toDouble() ;
        double high = detailList[5].toDouble();
        double low = detailList[6].toDouble();
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
            data.mChg = detailList[8].toDouble() - data.mLastClose;
            data.mChgPercent = data.mChg * 100 / detailList[3].toDouble() ;
        }
        data.mVol = detailList[9].toInt();
        data.mMoney = detailList[10].toDouble();
        data.mHsl = 0.0;
        data.mMoneyRatio = 0.0;
        if(data.mHistory.mLastMoney> 0){
            data.mMoneyRatio = data.mMoney / data.mHistory.mLastMoney;
        }

        if(data.mCur != 0)
        {
            data.mGXL = data.mBonusData.mXJFH / data.mCur;
        }
        data.mTotalCap = data.mCur * data.mFinanceData.mTotalShare;
        data.mMutalbleCap = data.mCur * data.mFinanceData.mMutalShare;
        if(data.mFinanceData.mMutalShare > 0)
        {
            data.mHsl = data.mVol / (double)(data.mFinanceData.mMutalShare);
        }
        if(data.mProfit == 0)
        {
            data.mProfit = DATA_SERVICE->getProfit(code);
        }
        data.mForeignCap = data.mHsgtData.mVolTotal * data.mCur ;
        data.mHsgtData.mVolCh1 = data.mHsgtData.mVolCh1;
        data.mForeignCapChg = data.mHsgtData.mVolCh1 * data.mCur ;
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
        if(mSendResFlag) datalist.append(data);
    }
    if(mSendResFlag)  emit sendStkDataList(datalist);
}

int QSinaStkInfoThread::getStkCount()
{
    return mStkList.count();
}


