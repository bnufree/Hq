#include "qeastmoneyblockthread.h"
#include <QDebug>
//#include "profiles.h"
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "qexchangedatamanage.h"
#include <QRegularExpression>
#include "qeastmoneyblocksharethread.h"
#include "utils/qhttpget.h"
#include "dbservices/dbservices.h"
#include "data_structure/hqutils.h"
#include <QThreadPool>
#include "dbservices/qactivedate.h"

#define     BLOCK_NMAE          "name"
#define     BLOCK_CODE          "codes"
#define     BLOCK_UPDATE        "Update"
#define     BLOCK_TIME          "time"
#define     REPORT              "Report"

QString blockthread[4] = {"NONE", "DY", "HY", "GN"};

QEastMoneyBlockThread::QEastMoneyBlockThread(int pBlockID, QObject *parent) : QObject(parent)
{
    mUpdateRealInfo = false;
    mUserBlockType = pBlockID;
    if(mUserBlockType == BLOCK_HY)
    {
        mWebBlockType = 2;
    } else if(mUserBlockType == BLOCK_GN)
    {
        mWebBlockType = 3;
    } else if(mUserBlockType == BLOCK_DQ)
    {
        mWebBlockType = 1;
    }
    mSortRule = -1;
    mBlockDataList.clear();
    this->moveToThread(&mWorkthread);
    connect(this, SIGNAL(start()), this, SLOT(slotUpdateBlockShare()));
    mWorkthread.start();
}

QEastMoneyBlockThread::~QEastMoneyBlockThread()
{

}

void QEastMoneyBlockThread::setSortRule(BLOCK_DISPLAY_RULE rule)
{
    if(rule == BLOCK_DISPLAY_SORT_NONE) return;
    mSortRule = (rule == BLOCK_DISPLAY_SORT_DESCEND? -1 : 1);
}

void QEastMoneyBlockThread::reverseSortRule()
{
    mSortRule *= (-1);
}

void QEastMoneyBlockThread::slotUpdateBlockInfos()
{
    QString url("http://nufm.dfcfw.com/EM_Finance2014NumericApplication/JS.aspx?type=CT&cmd=C._BK%1&sty=FCCS&st=c&p=1&ps=100&cb=&token=d0075ac6916d4aa6ec8495db9efe7ede&bklb=%2&jsName=BKtrade&sr=%3&dt=%4");

    QString wkURL = url.arg(blockthread[mWebBlockType]).arg(mWebBlockType).arg(mSortRule).arg(QDateTime::currentDateTime().toMSecsSinceEpoch());

    //开始解析数据
    BlockDataList list;
    QByteArray bytes = QHttpGet::getContentOfURL(wkURL);
    QString result = QString::fromUtf8(bytes.data());
    //qDebug()<<"result:"<<result.split(QRegularExpression("[\(\[|\]\)|\"]"));
    QString replaceStr = (mWebBlockType == 3 ? "3" : mWebBlockType == 2 ? "2" : "1");
    QRegExp rx("(BK0[0-9]{3}),([\u4e00-\u9fa5]{1,}),(-?[0-9]{1}\\.[0-9]{2})");
    int index = 0;
    while((index = rx.indexIn(result,index)) != -1)
    {
        //qDebug()<<"rx:"<<rx.cap();
        QString code = rx.cap(1).replace("BK0", replaceStr);
        BlockData* data = DATA_SERVICE->getBlockDataOfCode(code);
        if(data){
            data->mName = rx.cap(2);
            data->mChangePercent = rx.cap(3).toDouble();
        }
        index += (rx.matchedLength()+2);
    }
    return;
}

void QEastMoneyBlockThread::slotUpdateBlockShareCodeList(const QString &pBlockCode, const QStringList &pShareCodesList)
{
    mBlockDataList[pBlockCode].mShareCodeList = pShareCodesList;
}

void QEastMoneyBlockThread::slotBlockShareThreadFinished()
{
//    QEastMoneyBlockShareThread * thread = static_cast<QEastMoneyBlockShareThread*>(sender());
//    if(!thread) return;
//    thread->deleteLater();
}

void QEastMoneyBlockThread::stop()
{
    mUpdateRealInfo = false;
}

void QEastMoneyBlockThread::slotUpdateBlockShare()
{
    QString url("http://nufm.dfcfw.com/EM_Finance2014NumericApplication/JS.aspx?type=CT&cmd=C._BK%1&sty=FCCS&st=c&p=1&ps=100&cb=&js=var%20BKtrade%20={Trade:[[(x)]]}&token=d0075ac6916d4aa6ec8495db9efe7ede&bklb=%2&jsName=BKtrade&sr=%3&dt=%4");
    QString wkURL = url.arg(blockthread[mWebBlockType]).arg(mWebBlockType).arg(mSortRule).arg(QDateTime::currentDateTime().toMSecsSinceEpoch());

    //开始解析数据,取得所有板块的代码
    QByteArray bytes = QHttpGet::getContentOfURL(wkURL);
    QString result = QString::fromUtf8(bytes.data());
    QString replaceStr = (mWebBlockType == 3? "3" : mWebBlockType == 2 ? "2" : "1");
    int index = 0;
    while ((index = result.indexOf(QRegularExpression("BK0[0-9]{3}"), index)) > 0) {
        QString code = result.mid(index, 6).replace("BK0", replaceStr);
        BlockData* data = DATA_SERVICE->getBlockDataOfCode(code);
        if(data){
            data->mBlockType |= mUserBlockType;
            mBlockDataList[data->mCode] = *data;
        }
        qDebug()<<"block code:"<<data->mCode;
        index += 6;
    }
    //开始根据板块代码，获取板块内的所有shares
    QThreadPool pool;
    pool.setMaxThreadCount(8);
    pool.setExpiryTimeout(-1);
    foreach (QString key, mBlockDataList.keys()) {
        QEastMoneyBlockShareThread *thread = new QEastMoneyBlockShareThread(key);
        pool.start(thread);
    }
    qDebug()<<"block share update finished.start update block realtime info";
    //开始更新实时板块信息
    bool active = true;
    while(1)
    {
        if(active) slotUpdateBlockInfos();
        QThread::sleep(3);
        active  = QActiveDateTime::isCurDateTimeActive();
    }
}




