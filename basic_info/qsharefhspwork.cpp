#include "qsharefhspwork.h"
#include "qhttpget.h"
#include "dbservices/sharedata.h"
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include "utils/hqutils.h"

QShareFHSPWork::QShareFHSPWork(const QString& date, QObject *parent) : mDate(date), mParent(parent),QRunnable()
{

}

QShareFHSPWork::~QShareFHSPWork()
{

}



void QShareFHSPWork::run()
{
    QString url("http://data.eastmoney.com/DataCenter_V3/yjfp/getlist.ashx?js=var vWLdLOFe&pagesize=5000&page=1&sr=-1&sortType=SZZBL&mtk=%C8%AB%B2%BF%B9%C9%C6%B1&filter=(ReportingPeriod=^%1^)&rt=49499306");
    QString wkURL = url.arg(mDate);
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
        qDebug()<<__FUNCTION__<<__LINE__;
        return;
    }

    if(!doc.isArray())
    {
        qDebug()<<"json with wrong format.";
        qDebug()<<__FUNCTION__<<__LINE__;
        return;
    }
    qDebug()<<__FUNCTION__<<__LINE__;
    QJsonArray array = doc.array();
    ShareBaseDataList list;
    for(int i=0; i<array.count(); i++)
    {
        QJsonValue value = array.at(i);
        if(!value.isObject()) continue;
        QJsonObject subobj = value.toObject();
        //开始解析角色信息数据
        ShareBaseData data;
        data.setCode(subobj.value("Code").toString());
        data.mSZZG = subobj.value("SZZBL").toString().toDouble();
        data.mXJFH = subobj.value("XJFH").toString().toDouble()/10;
        data.mGQDJR = QDateTime(HqUtils::dateFromStr(subobj.value("GQDJR").toString().left(10))).toMSecsSinceEpoch();
        data.mYAGGR = QDateTime(HqUtils::dateFromStr(subobj.value("YAGGR").toString().left(10))).toMSecsSinceEpoch();
        list.append(data);
    }

    if(list.length() > 0 && mParent)
        {
            QMetaObject::invokeMethod(mParent, "slotUpdateShareFHSPList", Qt::DirectConnection, Q_ARG(ShareBaseDataList,list ));
        }
}
