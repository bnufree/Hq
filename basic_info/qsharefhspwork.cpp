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

QShareFHSPWork::QShareFHSPWork(QObject *parent) : mParent(parent),QRunnable()
{

}

QShareFHSPWork::~QShareFHSPWork()
{

}



void QShareFHSPWork::run()
{
    //首先获取数据库的更新日期,获取最近10年的分红送配记录,主要是半年报或者年报
    ShareDate last_update_date = DATA_SERVICE->getLastUpdateDateOfBonusInfo();
    if(last_update_date == ShareDate::currentDate()) return;
    ShareDate curDate = ShareDate::currentDate();
    ShareDate start_report_date(QDate(curDate.date().addYears(-1)));
    if(last_update_date.isNull())
    {
        start_report_date.setDate(QDate(2006,06,30));
    }

    int start_year = start_report_date.date().year();
    int end_year =  curDate.date().year();
    //计算到现在需要更新的日期,最近两个日期强制更新
    ShareBonusList list;
    QList<ShareDate> datelist;
    for(; start_year < end_year; start_year++)
    {
        datelist.append(ShareDate(QDate(start_year, 6, 30)));
        datelist.append(ShareDate(QDate(start_year, 12, 31)));
    }

    datelist.append(ShareDate(QDate(end_year, 6, 30)));
    datelist.append(ShareDate(QDate(end_year, 12, 31)));
    foreach (ShareDate date, datelist)
    {
        qDebug()<<__FUNCTION__<<date.toString();
        QElapsedTimer t;
        t.start();
        QString url("http://data.eastmoney.com/DataCenter_V3/yjfp/getlist.ashx?js=var vWLdLOFe&pagesize=5000&page=1&sr=-1&sortType=SZZBL&mtk=%C8%AB%B2%BF%B9%C9%C6%B1&filter=(ReportingPeriod=^%1^)&rt=49499306");
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
            data.mGQDJR = ShareDate::fromString(subobj.value("GQDJR").toString().left(10));
            data.mYAGGR = ShareDate::fromString(subobj.value("YAGGR").toString().left(10));
            data.mDate = ShareDate::fromString(subobj.value("ReportingPeriod").toString().left(10));
            list.append(data);
        }
        qDebug()<<__FUNCTION__<<__LINE__<<t.elapsed() / 1000<<list.size();
    }
    if(list.size() > 0)
    {
        DATA_SERVICE->signalUpdateShareBonusInfo(list);
    }
}
