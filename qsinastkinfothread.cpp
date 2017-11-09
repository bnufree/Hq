#include "qsinastkinfothread.h"
#include <QDebug>
#include "profiles.h"
#include <QDateTime>
#include "qexchangedatamanage.h"
#include "dbservices/dbservices.h"


QSinaStkInfoThread::QSinaStkInfoThread(QObject *parent) : QObject(parent), mHttp(0)
{
    qRegisterMetaType<StockDataList>("const StockDataList&");
    mStkList.clear();
    connect(this, SIGNAL(signalSetStkList(QStringList)), this, SLOT(setStkList(QStringList)));
//    mUpdateTimer = new QTimer;
//    mUpdateTimer->setInterval(2);
//    connect(mUpdateTimer, SIGNAL(timeout()), this, SLOT(slotUpdateInfo()));
    this->moveToThread(&mWorkThread);
    mWorkThread.start();
    //mActive = true;
}

QSinaStkInfoThread::~QSinaStkInfoThread()
{
//    if(mUpdateTimer)
//    {
//        mUpdateTimer->stop();
//        mUpdateTimer->deleteLater();
//    }
    if(mHttp)
    {
        mHttp->deleteLater();
    }

    mWorkThread.quit();
    mWorkThread.wait();

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
    //开始更新
    QString url("http://hq.sinajs.cn/?list=%1");
    if(mStkList.length() > 0)
    {
        QString wkURL = url.arg(mStkList.join(","));
        mHttp = new QHttpGet(wkURL, true);
        connect(mHttp, SIGNAL(signalSendHttpConent(QByteArray)), this, SLOT(slotRecvHttpContent(QByteArray)));
        connect(mHttp, SIGNAL(signalErrorOccured(QNetworkReply::NetworkError)), this, SLOT(slotRecvError(QNetworkReply::NetworkError)));
        mHttp->startGet();
    }
}


void QSinaStkInfoThread::slotUpdateInfo()
{
    QDateTime now = QDateTime::currentDateTime();
    if(now.date().dayOfWeek() == 6 || now.date().dayOfWeek() == 7 ||
            now.time().hour() < 9 || now.time().hour() >= 15)
    {
       return;
    }

    if(mHttp) mHttp->startGet();
}

void QSinaStkInfoThread::slotRecvHttpContent(const QByteArray &bytes)
{
    QString result = QString::fromLocal8Bit(bytes.data());
    //先换行
    QStringList resultlist = result.split(QRegExp("[\\n|;]"), QString::SkipEmptyParts);
    //再分割具体的字段
    foreach (QString detail, resultlist)
    {
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
        mDataMap[code].mVol = detailList[9].toInt();
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
}

int QSinaStkInfoThread::getStkCount()
{
    return mStkList.count();
}

void QSinaStkInfoThread::slotRecvError(QNetworkReply::NetworkError e)
{
    qDebug()<<"error:"<<e;
}


