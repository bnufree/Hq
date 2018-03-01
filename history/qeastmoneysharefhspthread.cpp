#include "qeastmoneysharefhspthread.h"
#include "qhttpget.h"
#include "stockdata.h"
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include "utils/hqutils.h"

QEastMoneyShareFHSPThread::QEastMoneyShareFHSPThread(QObject *parent) : QObject(parent)
{

}

QEastMoneyShareFHSPThread::~QEastMoneyShareFHSPThread()
{

}

void QEastMoneyShareFHSPThread::updateFHSPFiles()
{
//    //更新所有股票的板块信息
//    slotUpdateFHSPInfo();
//    int index = 0;
//    int total = mShareBlockList.keys().length();
//    foreach (QString code, mShareBlockList.keys()) {
//        emit signalUpdateMsg(QString("%1:%2/%3").arg(tr("正在更新板块信息")).arg(++index).arg(total));
//        StkInfoFileManage *filemgr = new StkInfoFileManage(code.right(6));
//        filemgr->setValue("Block", "names", mShareBlockList[code]);
//        filemgr->setValue("FHSP", "Content", mShareFHSPList[code]);
//        filemgr->deleteLater();
//    }
}

void QEastMoneyShareFHSPThread::updateFHSPInfoWithDate(StockDataList& list, const QString &date)
{
    //wkdate = 2016-12-31
    QString url("http://data.eastmoney.com/DataCenter_V3/yjfp/getlist.ashx?js=var vWLdLOFe&pagesize=5000&page=1&sr=-1&sortType=SZZBL&mtk=%C8%AB%B2%BF%B9%C9%C6%B1&filter=(ReportingPeriod=^%1^)&rt=49499306");
    QString wkURL = url.arg(date);
    //结果分析
    QString result = QString::fromUtf8(QHttpGet::getContentOfURL(wkURL));
    int startindex = -1, endindex = -1;
    startindex = result.indexOf("[{");
    endindex = result.indexOf("}]");
    if(startindex < 0 || endindex < 0)
    {
        return;
    }
    endindex += 2;
    QString hqStr = result.mid(startindex, endindex - startindex);

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(hqStr.toUtf8().data(), &err);
    if(err.error != QJsonParseError::NoError)
    {
        return;
    }

    if(!doc.isArray())
    {
        qDebug()<<"json with wrong format.";
        return;
    }
    QJsonArray array = doc.array();
    for(int i=0; i<array.count(); i++)
    {
        QJsonValue value = array.at(i);
        if(!value.isObject()) continue;
        QJsonObject subobj = value.toObject();
        //开始解析角色信息数据
        StockData data;
        data.mCode = subobj.value("Code").toString();
        data.mSZZG = subobj.value("SZZBL").toDouble();
        data.mXJFH = subobj.value("XJFH").toDouble();
        data.mGQDJR = HqUtils::dateFromStr(subobj.value("GQDJR").toString().left(10));
        data.mYAGGR = HqUtils::dateFromStr(subobj.value("YAGGR").toString().left(10));
        list.append(data);
    }

    return;
}

