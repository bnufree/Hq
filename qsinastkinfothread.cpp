#include "qsinastkinfothread.h"
#include <QDebug>
#include <QDateTime>
#include "qexchangedatamanage.h"
#include "dbservices/dbservices.h"
#include "utils/hqutils.h"
#include "utils/sharedata.h"


QSinaStkInfoThread::QSinaStkInfoThread(QObject *parent) : QObject(parent), mHttp(0)
{
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
    foreach (QString code, list) {
        if(code.length() == 6)
        {
            mStkList.append("s_"+ShareBaseData::prefixCode(code)+code);
        } else if(code.length() == 8)
        {
            mStkList.append("s_"+code);
        } else
        {
            mStkList.append(code);
        }
    }
    //开始更新
    QString url("http://hq.sinajs.cn/?list=%1");
    if(mStkList.length() > 0)
    {
        QString wkURL = url.arg(mStkList.join(","));
        mHttp = new QHttpGet(wkURL, true);
        connect(mHttp, SIGNAL(signalSendHttpConent(QByteArray)), this, SLOT(slotRecvHttpContent(QByteArray)));
        connect(mHttp, SIGNAL(signalErrorOccured(QNetworkReply::NetworkError)), this, SLOT(slotRecvError(QNetworkReply::NetworkError)));
        mHttp->start();
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

    if(mHttp) mHttp->start();
}

void QSinaStkInfoThread::slotRecvHttpContent(const QByteArray &bytes)
{
    QString result = QString::fromLocal8Bit(bytes.data());
    //先换行
    QStringList resultlist = result.split(QRegExp("[\\n;]"), QString::SkipEmptyParts);
    //再分割具体的字段
    ShareDataList datalist;
    foreach (QString detail, resultlist)
    {
        //qDebug()<<detail;
        detail.replace("var hq_str_s_", "");
        //qDebug()<<detail;
        QStringList detailList = detail.split(QRegExp("[\",=]"), QString::SkipEmptyParts);
        if(detailList.length() < 7) continue;
        QString code = detailList[0];
        ShareData * data = DATA_SERVICE->getBasicStkData(code);
        if(!data) continue;
        data->setName(detailList[1]);
        data->mCur = detailList[2].toDouble();
        data->mChg = detailList[3].toDouble();
        data->mChgPercent = detailList[4].toDouble();
        data->mVol = detailList[5].toInt() * 100;
        data->mMoney = detailList[6].toDouble();
        data->mHsl = 0.0;
        data->mMoneyRatio = 0.0;
        if(data->mLastMoney> 0){
            data->mMoneyRatio = data->mMoney / data->mLastMoney;
        }
        if(data->mCur == 0)
        {
            data->mCur = data->mLastClose;
        }
        if(data->mCur != 0)
        {
            data->mGXL = data->mXJFH / data->mCur;
        }
        data->mTotalCap = data->mCur * data->mTotalShare;
        data->mMutalbleCap = data->mCur * data->mMutalShare;
        if(data->mMutalShare > 0)
        {
            data->mHsl = data->mVol / (double)(data->mMutalShare);
        }
        if(data->mProfit == 0)
        {
            data->mProfit = DATA_SERVICE->getProfit(code);
        }
        data->mForeignCap = data->mForeignVol * data->mCur ;
        data->mForeignCapChg = data->mForeignVolChg * data->mCur ;
//        data->mUpdateTime = QDateTime::currentMSecsSinceEpoch();
        datalist.append(*data);
    }
    emit sendStkDataList(datalist);
}

int QSinaStkInfoThread::getStkCount()
{
    return mStkList.count();
}

void QSinaStkInfoThread::slotRecvError(QNetworkReply::NetworkError e)
{
    qDebug()<<"error:"<<e;
}


