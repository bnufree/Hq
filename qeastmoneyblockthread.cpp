#include "qeastmoneyblockthread.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>
#include <QEventLoop>
#include "profiles.h"
#include <QDateTime>
#include "qeastmoneystockinfothread.h"
#include "stkinfofilemanage.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "qexchangedatamanage.h"

#define     BLOCK_NMAE          "name"
#define     BLOCK_CODE          "codes"
#define     BLOCK_UPDATE        "Update"
#define     BLOCK_TIME          "time"
#define     REPORT              "Report"

QString blockthread[4] = {"NONE", "DY", "HY", "GN"};

QEastMoneyBlockThread::QEastMoneyBlockThread(QObject *parent) : QThread(parent)
{
    mOptType = BLOCK_INDUSTORY;
    mSortRule = -1;
    mUpdateBlockCodes = true;
    mBlockDataList.clear();
    qRegisterMetaType<BlockDataList>("const BlockDataList&");
    //设定分红送配的日期
    Profiles::instance()->setDefault(REPORT, BLOCK_TIME, "2016-12-31");
}

QEastMoneyBlockThread::~QEastMoneyBlockThread()
{

}

void QEastMoneyBlockThread::setOptType(BLOCK_OPT_TYPE type)
{
    mOptType = type;
    mSortRule = -1;
}

void QEastMoneyBlockThread::setSortRule(BLOCK_DISPLAY_RULE rule)
{
    if(rule == BLOCK_DISPLAY_SORT_NONE) return;
    mSortRule = (rule == BLOCK_DISPLAY_SORT_DESCEND? -1 : 1);
}

void QEastMoneyBlockThread::reverseSortRule()
{
    mSortRule = (mSortRule == -1 ? 1: -1);
}

void QEastMoneyBlockThread::RealtimeBlockInfo()
{
    QNetworkAccessManager *mgr = new QNetworkAccessManager;
    //QString url("http://quote.eastmoney.com/hq2data/bk/data/%1.js?v=%2");
    QString url("http://nufm.dfcfw.com/EM_Finance2014NumericApplication/JS.aspx?type=CT&cmd=C._BK%1&sty=FCCS&st=c&p=1&ps=100&cb=&js=var%20BKtrade%20={Trade:[[(x)]]}&token=d0075ac6916d4aa6ec8495db9efe7ede&bklb=%2&jsName=BKtrade&sr=%3&dt=%4");
    while(true)
    {
        BlockDataList wklist;
        for(int i = BLOCK_DISTRICT; i<= BLOCK_CONCEPT; i++)
        {
            mOptType = i;
            QString wkURL = url.arg(blockthread[mOptType]).arg(mOptType).arg(mSortRule).arg(QDateTime::currentDateTime().toMSecsSinceEpoch());
            //qDebug()<<"wkURL:"<<wkURL;
            QNetworkReply *reply  = mgr->get(QNetworkRequest(wkURL));
            if(!reply) continue;
            QEventLoop loop; // 使用事件循环使得网络通讯同步进行
            connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
            loop.exec(); // 进入事件循环， 直到reply的finished()信号发出， 这个语句才能退出
            if(reply->error())
            {
                qDebug()<<"err occured:"<<reply->errorString();
                reply->deleteLater();
                continue;
            }
            //开始解析数据
            QByteArray bytes = reply->readAll();
            QString result = QString::fromUtf8(bytes.data());
            //qDebug()<<"result:"<<result;
            int startindex = -1, endindex = -1;
            startindex = result.indexOf("[[") + 2;
            endindex = result.indexOf("]]}");
            if(startindex < 0 || endindex < 0) continue;
            QString hqStr = result.mid(startindex, endindex - startindex);
            //qDebug()<<"blockstr:"<<hqStr;
            if( !hqStr.isEmpty() )
            {

                QStringList blockInfoList = hqStr.split(QRegExp("\"|\","), QString::SkipEmptyParts);
                //qDebug()<<stockInfoList;
                foreach (QString blockInfo, blockInfoList) {
                    QStringList detailList = blockInfo.split(",", QString::SkipEmptyParts);
                    //qDebug()<<"line:"<<detailList;
                    if(detailList.length() < 11) continue;
                    BlockData data;
                    QString replaceStr = mOptType == BLOCK_CONCEPT ? "3" : mOptType == BLOCK_INDUSTORY ? "2" : "1";
                    data.code = detailList[1].replace("BK0", replaceStr);
                    data.name = detailList[2];
                    data.changePer = detailList[3].left(detailList[3].length() -1).toDouble();
                    data.hsl = detailList[4].left(detailList[4].length() -1).toDouble();
                    QStringList updownlist = detailList[5].split("|");
                    if(updownlist.length() > 3)
                    {
                        data.stockUpNum = detailList[5].split("|").at(0).toInt();
                        data.stockDownNum = detailList[5].split("|").at(2).toInt();
                    }
                    data.stklist = mBlockDataList[data.code].stklist;
                    wklist.append(data);
                }
            }
            reply->deleteLater();
        }
        emit sendBlockDataList(wklist);


        QThread::sleep(3);
    }

    mgr->deleteLater();
}

void QEastMoneyBlockThread::GetShares(const QString &pBlockCode, QList<QString>& codelist)
{
    QNetworkAccessManager *mgr = new QNetworkAccessManager;
    QString blockCode = "2800" + pBlockCode;
    //QString wkURL = QString("http://hqdigi2.eastmoney.com/EM_Quote2010NumericApplication/index.aspx?type=s&sortType=D&sortRule=-1&pageSize=500&page=1&jsName=quote_123&style=%1").arg(blockCode);
    QString wkURL = QString("http://nufm.dfcfw.com/EM_Finance2014NumericApplication/JS.aspx?type=CT&cmd=C.BK0wkcode1&sty=FCOIATA&sortType=C&sortRule=-1&page=1&pageSize=1000&js=var%20quote_123%3d{rank:[(x)],pages:(pc)}&token=7bc05d0d4c3c22ef9fca8c2a912d779c&jsName=quote_123&_g=0.5186116359042079");
    wkURL.replace("wkcode", pBlockCode.right(3));

    QNetworkReply *reply  = mgr->get(QNetworkRequest(wkURL));
    if(!reply) return;
    QEventLoop loop; // 使用事件循环使得网络通讯同步进行
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec(); // 进入事件循环， 直到reply的finished()信号发出， 这个语句才能退出
    if(reply->error())
    {
        qDebug()<<"err occured:"<<reply->errorString();
        reply->deleteLater();
        return;
    }
    //开始解析数据
    QByteArray bytes = reply->readAll();
    QString result = QString::fromUtf8(bytes.data());
    qDebug()<<"result:"<<result;
    int startindex = result.indexOf("[") + 1;
    int endindex = result.indexOf("]");
    if(startindex < 0 || endindex < 0) return;
    QString hqStr = result.mid(startindex, endindex - startindex);
    if( !hqStr.isEmpty() )
    {
        QStringList stockInfoList = hqStr.split(QRegExp("\"|\","), QString::SkipEmptyParts);
        //qDebug()<<stockInfoList;
        foreach (QString stockInfo, stockInfoList) {
            QStringList detailList = stockInfo.split(",", QString::SkipEmptyParts);
            if(detailList.length() < 20) continue;
            QString code = detailList[1];
            mShareBlockList[code].append(pBlockCode);
            if(code.left(1) == "6" || code.left(1) == "5")
            {
                code  = "sh"+code;
            } else
            {
                code = "sz" + code;
            }
            codelist.append(code);
        }
    }
    reply->deleteLater();

    mgr->deleteLater();
}

void QEastMoneyBlockThread::GetBlockShares()
{
    mShareBlockList.clear();
    //取得上次更新的时间
    int lastUpdate = Profiles::instance()->value(BLOCK_UPDATE, BLOCK_TIME).toInt();
    qDebug()<<"last:"<<lastUpdate;
    QDateTime lastDate = QDateTime::fromTime_t(lastUpdate);
    bool needUpdate = true;
    if(lastDate.date() == QExchangeDataManage::instance()->GetLatestActiveDay(QDate::currentDate()))
    {
        //读取配置
        QStringList secs = Profiles::instance()->getAllSections();
        while (secs.length() == 0) {
            secs = Profiles::instance()->getAllSections();
        }
        //qDebug()<<"groups:"<<secs;
        foreach (QString group, secs) {
            //qDebug()<<"group:"<<group;
            if(group.toInt() < 1000)continue;
            BlockData data;
            data.code = group;
            data.name = Profiles::instance()->value(data.code, BLOCK_NMAE).toString();
            data.stklist = Profiles::instance()->value(data.code, BLOCK_CODE).toStringList();
//            qDebug()<<"stklist:"<<data.stklist;
            mBlockDataList[data.code] = data;

        }
    } else
    {        
        QNetworkAccessManager *mgr = new QNetworkAccessManager;
        QString url("http://nufm.dfcfw.com/EM_Finance2014NumericApplication/JS.aspx?type=CT&cmd=C._BK%1&sty=FCCS&st=c&p=1&ps=100&cb=&js=var%20BKtrade%20={Trade:[[(x)]]}&token=d0075ac6916d4aa6ec8495db9efe7ede&bklb=%2&jsName=BKtrade&sr=%3&dt=%4");
        for(int i = BLOCK_DISTRICT; i<=BLOCK_CONCEPT; i++ )
        {
            mOptType = i;
            QString wkURL = url.arg(blockthread[mOptType]).arg(mOptType).arg(mSortRule).arg(QDateTime::currentDateTime().toMSecsSinceEpoch());
            //qDebug()<<"wkURL:"<<wkURL;
            QNetworkReply *reply  = mgr->get(QNetworkRequest(wkURL));
            if(!reply) continue;
            QEventLoop loop; // 使用事件循环使得网络通讯同步进行
            connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
            loop.exec(); // 进入事件循环， 直到reply的finished()信号发出， 这个语句才能退出
            if(reply->error())
            {
                qDebug()<<"err occured:"<<reply->errorString();
                reply->deleteLater();
                continue;
            }
            //开始解析数据
            QByteArray bytes = reply->readAll();
            QString result = QString::fromUtf8(bytes.data());
            //qDebug()<<"result:"<<result;
            int startindex = -1, endindex = -1;
            startindex = result.indexOf("[[") + 2;
            endindex = result.indexOf("]]}");
            if(startindex < 0 || endindex < 0) continue;
            QString hqStr = result.mid(startindex, endindex - startindex);
            //qDebug()<<"blockstr:"<<hqStr;
            if( !hqStr.isEmpty() )
            {
                BlockDataList wklist;
                QStringList blockInfoList = hqStr.split(QRegExp("\"|\","), QString::SkipEmptyParts);
                //qDebug()<<stockInfoList;
                int total = blockInfoList.length();
                int index = 0;
                QString content = (i==BLOCK_DISTRICT? tr("地区") : i== BLOCK_CONCEPT? tr("概念"): tr("行业"));
                foreach (QString blockInfo, blockInfoList) {
                    emit signalUpdateMsg(QString("%1%2:%3/%4").arg(tr("正在更新")).arg(content).arg(++index).arg(total));
                    QStringList detailList = blockInfo.split(",", QString::SkipEmptyParts);
                    //qDebug()<<"line:"<<detailList;
                    if(detailList.length() < 11) continue;
                    BlockData data;
                    QString replaceStr = mOptType == BLOCK_CONCEPT ? "3" : mOptType == BLOCK_INDUSTORY ? "2" : "1";
                    data.code = detailList[1].replace("BK0", replaceStr);
                    data.name = detailList[2];
                    wklist.append(data);
                    //取得对应的所有code
                    GetShares(data.code, data.stklist);
                    mBlockDataList[data.code] = data;

                    Profiles::instance()->setValue(data.code, BLOCK_NMAE, data.name);
                    Profiles::instance()->setValue(data.code, BLOCK_CODE, data.stklist);
                }
            }

            reply->deleteLater();
        }


        Profiles::instance()->setValue(BLOCK_UPDATE, BLOCK_TIME, QDateTime::currentDateTime().toTime_t());
        mgr->deleteLater();

        //更新所有股票的板块信息
        slotUpdateFHSPInfo();
        int index = 0;
        int total = mShareBlockList.keys().length();
        foreach (QString code, mShareBlockList.keys()) {            
            emit signalUpdateMsg(QString("%1:%2/%3").arg(tr("正在更新板块信息")).arg(++index).arg(total));
            StkInfoFileManage *filemgr = new StkInfoFileManage(code.right(6));
            filemgr->setValue("Block", "names", mShareBlockList[code]);
            filemgr->setValue("FHSP", "Content", mShareFHSPList[code]);
            filemgr->deleteLater();
        }

    }

 FUNC_END:


    //开始取得所有股票代码
    QStringList mStkCodesList;
    foreach (QString group, Profiles::instance()->getAllSections()) {
        if(group.left(1) != "2")continue;
        mStkCodesList.append(Profiles::instance()->value(group, "codes").toStringList());
    }
    mTotalStkCount = mStkCodesList.length();

    //开启线程将股本数据写入文件
    //开始启动线程然后获取信息,每个线程获取100个股票代码信息
    int nthread = (mStkCodesList.length() + 99 ) / 100;
    qDebug()<<"thread count:"<<nthread;
    for(int i=0; i<nthread; i++)
    {
        QStringList wklist = mStkCodesList.mid(i*100, 100);
        qeastmoneystockinfothread *infothr = new qeastmoneystockinfothread;
        infothr->setStockCodeList(wklist);
        connect(infothr, SIGNAL(finished()), this, SLOT(slotWorkThreadFinished()));
        connect(infothr, SIGNAL(sendUpdateProgress(int,int)), this, SLOT(slotStkCodeUpdateFinish(int,int)));
        mWorkThreadList.append(infothr);
        infothr->start();
    }



//    emit updateBlockCodesFinished();
    return;
}

void QEastMoneyBlockThread::run()
{
    if(!mUpdateBlockCodes)
        RealtimeBlockInfo();
    else
    {
        GetBlockShares();
    }
}

void QEastMoneyBlockThread::SetUpdateBlockCodes(bool pUpdate)
{
    mUpdateBlockCodes = pUpdate;
}

void QEastMoneyBlockThread::slotWorkThreadFinished()
{
    QThread* wkthread = (QThread*) sender();
    qDebug()<<"remove work thread:"<<wkthread;
    if(wkthread)
    {
        mWorkThreadList.removeAll(wkthread);
        wkthread->deleteLater();
        //emit sendStkinfoUpdateProgress();
    }
    if(mWorkThreadList.length() == 0)
    {
        emit updateBlockCodesFinished();
    }
}

void QEastMoneyBlockThread::slotStkCodeUpdateFinish(int cur, int total)
{
    static int wkindex = 1;
    emit signalUpdateMsg(QString("%1:%2/%3").arg(tr("正在更新股票信息")).arg(wkindex).arg(mTotalStkCount));
    wkindex++;
}

void QEastMoneyBlockThread::slotUpdateFHSPInfo()
{
    //wkdate = 2016-12-31
    QNetworkAccessManager *mgr = new QNetworkAccessManager;
    QString date = Profiles::instance()->value(REPORT, BLOCK_TIME).toString();
    QString url("http://data.eastmoney.com/DataCenter_V3/yjfp/getlist.ashx?js=var vWLdLOFe&pagesize=5000&page=1&sr=-1&sortType=SZZBL&mtk=%C8%AB%B2%BF%B9%C9%C6%B1&filter=(ReportingPeriod=^%1^)&rt=49499306");
    QString wkURL = url.arg(date);
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


    //结果分析
    QByteArray bytes = reply->readAll();
    QString result = QString::fromUtf8(bytes.data());
    int startindex = -1, endindex = -1;
    startindex = result.indexOf("[{");
    endindex = result.indexOf("}]");
    if(startindex < 0 || endindex < 0)
    {
        reply->deleteLater();
        mgr->deleteLater();
        return;
    }
    endindex += 2;
    QString hqStr = result.mid(startindex, endindex - startindex);

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(hqStr.toUtf8().data(), &err);
    if(err.error != QJsonParseError::NoError)
    {
        qDebug()<<"parse json error occured.";
        reply->deleteLater();
        mgr->deleteLater();
        return;
    }

    if(!doc.isArray())
    {
        qDebug()<<"json with wrong format.";
        reply->deleteLater();
        mgr->deleteLater();
        return;
    }
    QJsonArray array = doc.array();
    for(int i=0; i<array.count(); i++)
    {
        QJsonValue value = array.at(i);
        if(!value.isObject()) continue;
        QJsonObject subobj = value.toObject();
        //开始解析角色信息数据
        QString code = subobj.value("Code").toString();
        QStringList list;
        list.append(subobj.value("SZZBL").toString());
        list.append(subobj.value("XJFH").toString());
        list.append(subobj.value("GXL").toString());
        list.append(subobj.value("GQDJR").toString());
        list.append(subobj.value("YAGGR").toString().left(10));
        mShareFHSPList[code] = list;
    }

    reply->deleteLater();
    mgr->deleteLater();
}

