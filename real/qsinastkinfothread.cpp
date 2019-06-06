#include "qsinastkinfothread.h"
#include <QDebug>
#include <QDateTime>
#include "qexchangedatamanage.h"
#include "dbservices/dbservices.h"
#include "data_structure/hqutils.h"
#include "data_structure/sharedata.h"


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
            mStkList.append(ShareData::prefixCode(code)+code);
        } else if(code.length() == 8)
        {
            mStkList.append(code);
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
    QTextCodec *codes = QTextCodec::codecForName("GBK");
    QString result = codes->toUnicode(bytes);
    //先换行
    QStringList resultlist = result.split(QRegExp("[\\n;]"), QString::SkipEmptyParts);
    //再分割具体的字段
    ShareDataList datalist;
    foreach (QString detail, resultlist)
    {
        //var hq_str_sz399006="创业板指,1647.848,1692.416,1680.387,1718.384,1635.450,0.000,0.000,10414641478,126326323478.810
        detail.replace("var hq_str_", "");
        //qDebug()<<detail;
        QStringList detailList = detail.split(QRegExp("[\",=]"), QString::SkipEmptyParts);
        if(detailList.length() < 11) continue;
        QString code = detailList[0];
        ShareData * data = DATA_SERVICE->getShareData(code);
        if(!data) continue;
        //qDebug()<<data->mCode<<data->mName<<data->mShareType;
        data->mName = detailList[1];
        data->mCur = detailList[4].toDouble();
        data->mChg = detailList[4].toDouble() - detailList[3].toDouble();
        data->mChgPercent = data->mChg * 100 / detailList[3].toDouble() ;
        data->mVol = detailList[10].toInt();
        data->mMoney = detailList[10].toDouble() / 10000;
        data->mHsl = 0.0;
        data->mMoneyRatio = 0.0;
        if(data->mHistory.mLastMoney> 0){
            data->mMoneyRatio = data->mMoney / data->mHistory.mLastMoney;
        }
        //qDebug()<<data->mCode<<data->mName<<data->mMoney<<data->mLastMoney<<data->mMoneyRatio;
        if(data->mCur == 0)
        {
            data->mCur = data->mHistory.mLastClose;
        }
        if(data->mCur != 0)
        {
            data->mGXL = data->mBonusData.mXJFH / data->mCur;
        }
        data->mTotalCap = data->mCur * data->mFinanceData.mTotalShare;
        data->mMutalbleCap = data->mCur * data->mFinanceData.mMutalShare;
        if(data->mFinanceData.mMutalShare > 0)
        {
            data->mHsl = data->mVol / (double)(data->mFinanceData.mMutalShare);
        }
        if(data->mProfit == 0)
        {
            data->mProfit = DATA_SERVICE->getProfit(code);
        }
        data->mForeignCap = data->mHsgtData.mVolTotal * data->mCur ;
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


