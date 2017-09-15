#include "qeastmoneyblockthread.h"
#include <QDebug>
#include "profiles.h"
#include <QDateTime>
#include "stkinfofilemanage.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "qexchangedatamanage.h"
#include <QRegularExpression>
#include "qeastmoneyblocksharethread.h"
#include "qhttpget.h"

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
    qRegisterMetaType<QMap<QString, QStringList>>("const QMap<QString, QStringList>&");
    qRegisterMetaType<QMap<QString, BlockData>>("const QMap<QString, BlockData>&");
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

    QString wkURL = url.arg(blockthread[mBlockType]).arg(mBlockType).arg(mSortRule).arg(QDateTime::currentDateTime().toMSecsSinceEpoch());

    //开始解析数据
    BlockDataList list;
    QByteArray bytes = QHttpGet::getContentOfURL(wkURL);
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
        mBlockDataMap[data.code] = data;
    }
    //qDebug()<<__FUNCTION__<<__LINE__<<mBlockDataList.values().length();
    emit sendBlockDataList(mBlockType, list, mBlockDataMap);

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
       emit sendShareBlockDataMap(mShareBlockMap);
        while (true) {
            slotUpdateBlockInfos();
            QThread::sleep(2);
        }
    } else
    {
        qDebug()<<"current block commnad length:"<<mWorkThreadList.length();
    }
}

void QEastMoneyBlockThread::slotUpdateBlockShare()
{
    QString url("http://nufm.dfcfw.com/EM_Finance2014NumericApplication/JS.aspx?type=CT&cmd=C._BK%1&sty=FCCS&st=c&p=1&ps=100&cb=&js=var%20BKtrade%20={Trade:[[(x)]]}&token=d0075ac6916d4aa6ec8495db9efe7ede&bklb=%2&jsName=BKtrade&sr=%3&dt=%4");
    QString wkURL = url.arg(blockthread[mBlockType]).arg(mBlockType).arg(mSortRule).arg(QDateTime::currentDateTime().toMSecsSinceEpoch());

    //开始解析数据,取得所有板块的代码
    QByteArray bytes = QHttpGet::getContentOfURL(wkURL);
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
        connect(thread, SIGNAL(signalUpdateShareBlock(QString,QString)), this, SLOT(slotUpdateShareBlock(QString,QString)));
        thread->start();
    }

}

void QEastMoneyBlockThread::slotUpdateShareBlock(const QString &share, const QString &block)
{
    mShareBlockMap[share].append(block);
}



