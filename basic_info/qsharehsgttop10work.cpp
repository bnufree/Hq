#include "qsharehsgttop10work.h"
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
#include <QFile>

QShareHsgtTop10Work::QShareHsgtTop10Work(QObject *parent) : QThread(parent)
{
}

QShareHsgtTop10Work::~QShareHsgtTop10Work()
{

}

void QShareHsgtTop10Work::run()
{
    ShareWorkingDate last_update_date = DATA_SERVICE->getLastUpdateDateOfHsgtTop10();
    qDebug()<<"last date:"<<last_update_date.toString()<<last_update_date.isNull();
    if(last_update_date == ShareWorkingDate::currentDate()) return;
    ShareWorkingDate curDate = ShareWorkingDate::currentDate();
    if(last_update_date.isNull()) last_update_date.setDate(curDate.date().addDays(-30));

    ShareHsgtList list;
    last_update_date.next();
    while (last_update_date <= curDate) {
        if(last_update_date.isWeekend())
        {
            last_update_date.next();
            continue;
        }
        //从网络获取
        if(!getDataFromEastMoney(list, last_update_date))
        {
            getDataFromHKEX(list, last_update_date);
        }
        last_update_date.next();
    }
    if(list.size() > 0)
    {
        DATA_SERVICE->signalUpdateShareHsgtTop10Info(list);
    }
}

bool QShareHsgtTop10Work::getDataFromEastMoney(ShareDataList &list, const ShareWorkingDate &date)
{
    //从网络获取
    QString url = QString("http://dcfm.eastmoney.com//EM_MutiSvcExpandInterface/api/js/get?type=HSGTCJB&token=70f12f2f4f091e459a279469fe49eca5&filter=(DetailDate=^%1^)&js=(x)&sr=1&st=Rank&rt=50014200")
            .arg(date.toString());
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(QHttpGet().getContentOfURL(url), &err);
    if(err.error != QJsonParseError::NoError) return false;
    if(!doc.isArray())  return false;
    //开始解析
    ShareDataList resList;
    QJsonArray result = doc.array();
    for(int i=0; i<result.size(); i++)
    {
        QJsonObject obj = result.at(i).toObject();
        int market_type = obj.value("MarketType").toInt();
        if(market_type == 2 || market_type == 4) continue;
        ShareHsgt data;
        data.mCode = obj.value("Code").toString();
        data.mName = obj.value("Name").toString();
        if(data.mCode.left(1) == "6")
        {
            //上海
            data.mBuy = obj.value("HGTMRJE").toDouble();
            data.mSell = obj.value("HGTMCJE").toDouble();
        } else
        {
            //深圳
            data.mBuy = obj.value("SGTMRJE").toDouble();
            data.mSell = obj.value("SGTMCJE").toDouble();
        }
        data.mIsTop10 = true;
        data.mPure = data.mBuy - data.mSell;
        data.mDate = ShareWorkingDateTime(last_update_date.date());
        resList.append(data);
    }
    if(resList.size() > 0) list.append(resList);

    return resList.size() > 0;
}

bool QShareHsgtTop10Work::getDataFromHKEX(ShareDataList &list, const QDate &date)
{

}
