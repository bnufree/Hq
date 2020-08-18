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
#include "date/shareworkingdatetime.h"
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
    while (true) {
        QDate last_update_date = DATA_SERVICE->getLastUpdateDateOfHsgtTop10();
        qDebug()<<"last date:"<<ShareTradeDateTime(last_update_date).toString()<<last_update_date.isNull();
        //如果日期就是当前的工作日,证明更新完成,不再需要继续更新,结束线程
        if(last_update_date == TradeDateMgr::instance()->lastTradeDay())
        {

            DATA_SERVICE->signalUpdateHsgtTop10Keys(last_update_date);
            return;
        }
        QDate curDate = QDate::currentDate();
        if(last_update_date.isNull()) last_update_date = curDate.addDays(-30);

        QDate final_date = last_update_date;
        ShareHsgtList list;
        last_update_date = ShareTradeDateTime(last_update_date).nextTradeDay();
        qDebug()<<"start get north top10 from date:"<<ShareTradeDateTime(last_update_date).toString();
        while (last_update_date <= curDate) {
            int old_size = list.size();
            if(TradeDateMgr::instance()->isTradeDay(last_update_date))
            {
                //从网络获取
                if(!getDataFromEastMoney(list, last_update_date))
                {
                    getDataFromHKEX(list, last_update_date);
                }
                int new_size = list.size();
                if(new_size != old_size)
                {
                    final_date = last_update_date;
                }
            }
            last_update_date = last_update_date.addDays(1);
        }
        if(list.size() > 0)
        {
            DATA_SERVICE->signalUpdateShareHsgtTop10Info(list);
        }

        DATA_SERVICE->signalUpdateHsgtTop10Keys(final_date);

        sleep(600);
    }
}

bool QShareHsgtTop10Work::getDataFromEastMoney(ShareHsgtList &list, const QDate &date)
{
    //从网络获取.
    QString url = QString("http://dcfm.eastmoney.com//EM_MutiSvcExpandInterface/api/js/get?type=HSGTCJB&token=70f12f2f4f091e459a279469fe49eca5&filter=(DetailDate=^%1^)&js=(x)&sr=1&st=Rank&rt=50014200")
            .arg(date.toString());
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(QHttpGet().getContentOfURL(url), &err);
    if(err.error != QJsonParseError::NoError) return false;
    if(!doc.isArray())  return false;
    //开始解析.
    ShareHsgtList resList;
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
        data.mDate = date;
        resList.append(data);
    }
    if(resList.size() > 0) list.append(resList);

    return resList.size() > 0;
}

bool QShareHsgtTop10Work::getDataFromHKEX(ShareHsgtList &list, const QDate &date)
{
    return true;
}
