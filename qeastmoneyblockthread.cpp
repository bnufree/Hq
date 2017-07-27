#include "qeastmoneyblockthread.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>
#include <QEventLoop>
#include "profiles.h"
#include <QDateTime>
//#include "qeastmoneystockhistoryinfothread.h"
#include "stkinfofilemanage.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "qexchangedatamanage.h"
#include <QRegularExpression>
#include "qeastmoneyblocksharethread.h"

#define     BLOCK_NMAE          "name"
#define     BLOCK_CODE          "codes"
#define     BLOCK_UPDATE        "Update"
#define     BLOCK_TIME          "time"
#define     REPORT              "Report"

QString blockthread[4] = {"NONE", "DY", "HY", "GN"};

QEastMoneyBlockThread::QEastMoneyBlockThread(int pBlockID, QObject *parent) : QObject(parent)
{
    mBlockType = pBlockID;
    mSortRule = -1;
    mBlockDataList.clear();
    qRegisterMetaType<BlockDataList>("const BlockDataList&");
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
    QNetworkAccessManager *mgr = new QNetworkAccessManager;
    QString url("http://nufm.dfcfw.com/EM_Finance2014NumericApplication/JS.aspx?type=CT&cmd=C._BK%1&sty=FCCS&st=c&p=1&ps=100&cb=&token=d0075ac6916d4aa6ec8495db9efe7ede&bklb=%2&jsName=BKtrade&sr=%3&dt=%4");

    QString wkURL = url.arg(blockthread[mBlockType]).arg(mBlockType).arg(mSortRule).arg(QDateTime::currentDateTime().toMSecsSinceEpoch());
    //qDebug()<<"wkURL:"<<wkURL;
    QNetworkReply *reply  = mgr->get(QNetworkRequest(wkURL));
    if(!reply)
    {
        mgr->deleteLater();
        return;
    }
    QEventLoop loop; // 使用事件循环使得网络通讯同步进行
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec(); // 进入事件循环， 直到reply的finished()信号发出， 这个语句才能退出
    if(reply->error())
    {
        qDebug()<<"err occured:"<<reply->errorString();
        reply->deleteLater();
        mgr->deleteLater();
        return;
    }
    //开始解析数据
    BlockDataList list;
    QByteArray bytes = reply->readAll();
    QString result = QString::fromUtf8(bytes.data());
    //qDebug()<<"result:"<<result.split(QRegularExpression("[\(\[|\]\)|\"]"));
    QString replaceStr = (mBlockType == BLOCK_CONCEPT ? "3" : mBlockType == BLOCK_INDUSTORY ? "2" : "1");
    QRegExp rx("(BK0[0-9]{3}),([\u4e00-\u9fa5]{1,}),(-?[0-9]{1}\\.[0-9]{2})");
    int index = 0;
    while((index = rx.indexIn(result,index)) != -1)
    {
        //qDebug()<<"rx:"<<rx.cap();
        BlockData &data = mBlockDataList[rx.cap(1).replace("BK0", replaceStr)];
        data.name = rx.cap(2);
        data.changePer = rx.cap(3).toDouble();
        index += (rx.matchedLength()+2);
        list.append(data);
    }
    reply->deleteLater();
    mgr->deleteLater();


    //qDebug()<<__FUNCTION__<<__LINE__<<mBlockDataList.values().length();
    emit sendBlockDataList(mBlockType, list);

    return;
}

void QEastMoneyBlockThread::slotUpdateBlockShareCodeList(const QString &pBlockCode, const QStringList &pShareCodesList)
{
    //qDebug()<<"block:"<<pBlockCode<<" list:"<<pShareCodesList;
    mBlockDataList[pBlockCode].stklist = pShareCodesList;
}

void QEastMoneyBlockThread::slotBlockShareThreadFinished()
{
    QEastMoneyBlockShareThread * thread = static_cast<QEastMoneyBlockShareThread*>(sender());
    if(!thread) return;
    mWorkThreadList.removeOne(thread);
    thread->deleteLater();
    if(mWorkThreadList.isEmpty())
    {
       qDebug()<<"update share codes finished.............................";
    }

    while (true) {
        slotUpdateBlockInfos();
        QThread::sleep(2);
    }
}

void QEastMoneyBlockThread::slotUpdateBlockShare()
{
    QNetworkAccessManager *mgr = new QNetworkAccessManager;
    QString url("http://nufm.dfcfw.com/EM_Finance2014NumericApplication/JS.aspx?type=CT&cmd=C._BK%1&sty=FCCS&st=c&p=1&ps=100&cb=&js=var%20BKtrade%20={Trade:[[(x)]]}&token=d0075ac6916d4aa6ec8495db9efe7ede&bklb=%2&jsName=BKtrade&sr=%3&dt=%4");
    QString wkURL = url.arg(blockthread[mBlockType]).arg(mBlockType).arg(mSortRule).arg(QDateTime::currentDateTime().toMSecsSinceEpoch());
    qDebug()<<"wkURL:"<<wkURL;
    QNetworkReply *reply  = mgr->get(QNetworkRequest(wkURL));
    if(!reply)
    {
        mgr->deleteLater();
        return;
    }
    QEventLoop loop; // 使用事件循环使得网络通讯同步进行
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec(); // 进入事件循环， 直到reply的finished()信号发出， 这个语句才能退出
    if(reply->error())
    {
        reply->deleteLater();
        return;
    }
    //开始解析数据,取得所有板块的代码
    QByteArray bytes = reply->readAll();
    QString result = QString::fromUtf8(bytes.data());
    QString replaceStr = (mBlockType == BLOCK_CONCEPT ? "3" : mBlockType == BLOCK_INDUSTORY ? "2" : "1");
    int index = 0;
    while ((index = result.indexOf(QRegularExpression("BK0[0-9]{3}"), index)) > 0) {
        BlockData data;
        data.code = result.mid(index, 6).replace("BK0", replaceStr);
        qDebug()<<"code:"<<data.code;
        mBlockDataList[data.code] = data;
        index += 6;
    }
    //开始根据板块代码，获取板块内的所有shares
    foreach (QString key, mBlockDataList.keys()) {
        QEastMoneyBlockShareThread *thread = new QEastMoneyBlockShareThread(key, this);
        mWorkThreadList.append(thread);
        connect(thread, SIGNAL(signalUpdateBlockShareCodeList(QString,QStringList)), this, SLOT(slotUpdateBlockShareCodeList(QString,QStringList)));
        connect(thread, SIGNAL(finished()), this, SLOT(slotBlockShareThreadFinished()));
        thread->start();
    }

}



