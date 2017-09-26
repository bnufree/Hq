#include "qsinastkinfothread.h"
#include <QDebug>
#include "profiles.h"
#include <QDateTime>
#include "stkmktcap.h"
#include "stkinfofilemanage.h"
#include "qexchangedatamanage.h"
#include "dbservices.h"
#include "qhttpget.h"


QSinaStkInfoThread::QSinaStkInfoThread(QObject *parent) : QThread(parent)
{
    mOptType =STK_DISPLAY_SORT_TYPE_CHGPER;
    mSortRule = -1;
    mStkList.clear();
    //mActive = true;
}

QSinaStkInfoThread::~QSinaStkInfoThread()
{

}
bool QSinaStkInfoThread::isActive()
{
    //return mActive;
    return true;
}

void QSinaStkInfoThread::setOptType(STK_DISPLAY_TYPE type)
{
    if(mOptType == type)
    {
        reverseSortRule();
    } else
    {
        mOptType = type;
        mSortRule = -1;
    }
}

void QSinaStkInfoThread::setSortRule(STK_DISPLAY_RULE rule)
{
    if(rule == STK_DISPLAY_SORT_RULE_NONE) return;
    mSortRule = (rule == STK_DISPLAY_SORT_RULE_DESCEND? -1 : 1);
}

void QSinaStkInfoThread::reverseSortRule()
{
    mSortRule = (mSortRule == -1 ? 1: -1);
}

void QSinaStkInfoThread::setStkList(const QStringList &list)
{
    mStkList = list;
    //取得配置文件保存的昨天的数据
    mDataMap.clear();
    foreach (QString wkcode, mStkList) {
        StockData data = DATA_SERVICE->getBasicStkData(wkcode.right(6));
        data.mCode = wkcode.right(6);
        foreignHolder holder = DATA_SERVICE->amountForeigner(data.mCode);
        data.mForeignVol = holder.last;
        data.mForeignVolChg = holder.last - holder.previous;
        mDataMap[data.mCode] = data;
    }

   // qDebug()<<"mdataMap:"<<mDataMap.keys();

}

void QSinaStkInfoThread::setActive(bool act)
{
    //mActive = act;
}

void QSinaStkInfoThread::RealtimeInfo()
{
    QString url("http://hq.sinajs.cn/?list=%1");
    while(true)
    {
        if(mStkList.length() == 0) continue;

        QString wkURL = url.arg(mStkList.join(","));
        //开始解析数据
        QByteArray bytes = QHttpGet::getContentOfURL(wkURL);
        QString result = QString::fromLocal8Bit(bytes.data());
        //先换行
        QStringList resultlist = result.split(QRegExp("[\\n|;]"), QString::SkipEmptyParts);
        //再分割具体的字段
        foreach (QString detail, resultlist) {
            detail.replace(QRegExp("([a-z]{1,} )"), "");
            QStringList detailList = detail.split(QRegExp("[a-z|\"|\"|,|=|_]"), QString::SkipEmptyParts);
            if(detailList.length() < 20) continue;
            QString code = detailList[0];
            mDataMap[code].mName = detailList[1];
            mDataMap[code].mOpen = detailList[2].toDouble();
            mDataMap[code].mLastClose = detailList[3].toDouble();
            mDataMap[code].mCur = detailList[4].toDouble();
            mDataMap[code].mHigh = detailList[5].toDouble();
            mDataMap[code].mLow = detailList[6].toDouble();
            //mDataMap[code].buy = detailList[7].toDouble();
            //mDataMap[code].sell = detailList[8].toDouble();
            mDataMap[code].mVol = detailList[9].toInt();
            //mDataMap[code].time = detailList[detailList.length()-2];
            qint64 total = mDataMap[code].mMutableShare;
            if(total > 0){
                mDataMap[code].mHsl = mDataMap[code].mVol / (double)total * 100;
            } else {
                mDataMap[code].mHsl = 0.00;
            }
            double lastmonry = mDataMap[code].mLastMoney;
            if(lastmonry > 0){
                mDataMap[code].mMoneyRatio = detailList[10].toDouble() / lastmonry;
            } else {
                mDataMap[code].mMoneyRatio = 0.00;
            }
            //MktCapFile::instance()->setValue(sec, key, detailList[10].toDouble());
            mDataMap[code].mMoney = detailList[10].toDouble() / 10000;
            int hour = QDateTime::currentDateTime().time().hour();
            int min = QDateTime::currentDateTime().time().minute();
            if( hour == 9){
                if(min>=15 && min<=25){
                    mDataMap[code].mCur = detailList[12].toDouble();
                }
            }
            if(mDataMap[code].mCur == 0)
            {
                mDataMap[code].mCur = mDataMap[code].mLastClose;
            }
            if(mDataMap[code].mCur != 0)
            {
                mDataMap[code].mGXL = mDataMap[code].mXJFH / mDataMap[code].mCur;
            }
            //if(data.cur == 0 ) continue;
            mDataMap[code].mChg = mDataMap[code].mCur - mDataMap[code].mLastClose;
            mDataMap[code].mChgPercent = mDataMap[code].mChg *100 / mDataMap[code].mLastClose;
            mDataMap[code].mTotalCap = mDataMap[code].mCur * mDataMap[code].mTotalShare;
            mDataMap[code].mMutalbleCap = mDataMap[code].mCur * mDataMap[code].mMutableShare;
            if(mDataMap[code].mProfit == 0)
            {
                mDataMap[code].mProfit = DATA_SERVICE->getProfit(code);
            }
            mDataMap[code].mForeignCap = mDataMap[code].mForeignVol * mDataMap[code].mCur ;
            mDataMap[code].mForeignCapChg = mDataMap[code].mForeignVolChg * mDataMap[code].mCur ;
        }
        emit sendStkDataList(mDataMap.values());
        QThread::sleep(1);
    }

}

void QSinaStkInfoThread::run()
{
    RealtimeInfo();
}

int QSinaStkInfoThread::getStkCount()
{
    return mStkList.count();
}


