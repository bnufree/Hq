#include "qsharefhspwork.h"
#include "utils/qhttpget.h"
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include "data_structure/hqutils.h"
#include "data_structure/sharedata.h"
#include "dbservices/dbservices.h"
#include "dbservices/qactivedate.h"

QShareFHSPWork::QShareFHSPWork(QObject *parent) : QThread(parent)
{

}

QShareFHSPWork::~QShareFHSPWork()
{

}



void QShareFHSPWork::run()
{
    //首先获取数据库的更新日期,获取最近10年的分红送配记录,主要是半年报或者年报
    ShareBonusList list;
    QDate last_update_date = DATA_SERVICE->getLastUpdateDateOfBonusInfo();
    QDate curDate = QDate::currentDate();
    QDate start_report_date(QDate(curDate.addYears(-3)));
    if(last_update_date.isNull())
    {
        start_report_date = QDate::currentDate().addYears(-3);
    } else
    {
        start_report_date = last_update_date.addYears(-1);
    }

    int start_year = start_report_date.year();
    int end_year =  curDate.year();
    //计算到现在需要更新的日期,最近两个日期强制更新
    QList<QDate> datelist;
    for(; start_year <= end_year; start_year++)
    {
        QDate half(QDate(start_year, 6, 30));
        if(half >= start_report_date)
        {
            datelist.append(half);
        }
        QDate end(QDate(start_year, 12, 31));
        if(end >= start_report_date)
        {
            datelist.append(end);
        }
    }
    foreach (QDate date, datelist)
    {
        QElapsedTimer t;
        t.start();
        QString url("http://data.eastmoney.com/DataCenter_V3/yjfp/getlist.ashx?js=var vWLdLOFe&pagesize=50000&page=1&sr=-1&sortType=SZZBL&mtk=%C8%AB%B2%BF%B9%C9%C6%B1&filter=(ReportingPeriod=^%1^)&rt=49499306");
        QString wkURL = url.arg(date.toString());
        //结果分析
        QString result = QString::fromUtf8(QHttpGet::getContentOfURL(wkURL));
        int startindex = -1, endindex = -1;
        startindex = result.indexOf("[{");
        endindex = result.indexOf("}]");
        if(startindex < 0 || endindex < 0)
        {
            qDebug()<<"error content found now:"<<startindex<<endindex;
            continue;
        }
        endindex += 2;
        QString hqStr = result.mid(startindex, endindex - startindex);

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(hqStr.toUtf8().data(), &err);
        if(err.error != QJsonParseError::NoError) continue;
        if(!doc.isArray()) continue;
        QJsonArray array = doc.array();
        for(int i=0; i<array.count(); i++)
        {
            QJsonValue value = array.at(i);
            if(!value.isObject()) continue;
            QJsonObject subobj = value.toObject();
            //开始解析角色信息数据
            ShareBonus data;
            data.mCode = subobj.value("Code").toString();
            data.mSZZG = subobj.value("SZZBL").toString().toDouble();
            data.mXJFH = subobj.value("XJFH").toString().toDouble()/10;
            data.mGQDJR = QDate::fromString(subobj.value("GQDJR").toString().left(10));
            data.mYAGGR = QDate::fromString(subobj.value("YAGGR").toString().left(10));
            data.mDate = QDate::fromString(subobj.value("ReportingPeriod").toString().left(10));
            list.append(data);
            //                qDebug()<<data.mCode<<data.mSZZG<<data.mXJFH<<data.mDate.toString();
        }
    }
    DATA_SERVICE->signalUpdateShareBonusInfo(list);
}
