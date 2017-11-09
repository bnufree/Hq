#include "qeastmoneychinashareexchange.h"
#include "qhttpget.h"
#include <QDebug>
#include "dbservices/dbservices.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>

QEastMoneyChinaShareExchange::QEastMoneyChinaShareExchange(const QDate& pDate, QObject *parent) : QThread(parent)
{
    mWorkDate = pDate;
    qRegisterMetaType<QList<ChinaShareExchange>>("const QList<ChinaShareExchange>&");
}

QEastMoneyChinaShareExchange::~QEastMoneyChinaShareExchange()
{

}

void QEastMoneyChinaShareExchange::getHGTTop10Share(QList<ChinaShareExchange>& list, const QDate& date)
{
#if 0
    QString url = tr("http://dcfm.eastmoney.com//EM_MutiSvcExpandInterface/api/js/get?type=HSGTCJB&"
                     "token=70f12f2f4f091e459a279469fe49eca5&filter=(DetailDate=^%1^)&js=(x)&sr=1&st=Rank&rt=50014200")
            .arg(date.toString("yyyy-MM-dd"));
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(QHttpGet().getContent(url), &err);
    if(err.error != QJsonParseError::NoError) return;
    if(!doc.isArray())  return;
    //开始解析
    QJsonArray result = doc.array();
    for(int i=0; i<result.size(); i++)
    {
        QJsonObject obj = result.at(i).toObject();
        int market_type = obj.value("MarketType").toInt();
        if(market_type == 2 || market_type == 4) continue;
        ChinaShareExchange data;
        data.mCode = obj.value("Code").toString();
        data.mName = obj.value("Name").toString();
        data.mCur = obj.value("Close").toDouble();
        data.mChgPercent = obj.value("ChangePercent").toDouble();
        if(data.mCode.left(1) == "6")
        {
            //上海
            data.mTop10Buy = obj.value("HGTMRJE").toDouble();
            data.mTop10Sell = obj.value("HGTMCJE").toDouble();
        } else
        {
            //深圳
            data.mTop10Buy = obj.value("SGTMRJE").toDouble();
            data.mTop10Sell = obj.value("SGTMCJE").toDouble();
        }
        data.mDate = date;

        list.append(data);
    }
#endif

    return;

}

void QEastMoneyChinaShareExchange::run()
{
    QDate lastDate = DATA_SERVICE->getLastUpdateDateOfHSGT();
    QDate wkDate = lastDate;
    //取得历史信息
    while(true)
    {
        qDebug()<<"date:"<<wkDate;
        wkDate = wkDate.addDays(1);
        if(wkDate >= QDate::currentDate()) break;
        QList<ChinaShareExchange> wklist;
        getHGTTop10Share(wklist, wkDate);
        if(wklist.length() > 0)
        {
            emit DATA_SERVICE->signalRecvTop10ChinaStockInfos(wklist);
            lastDate = wkDate;
        }
        qDebug()<<"date:"<<wkDate<<" list len:"<<wklist.length();
    }

    //更新今日的信息
    if(lastDate == QDate::currentDate()) return;
    //更新今天的信息
    while(true)
    {
        QThread::sleep(10);
        QList<ChinaShareExchange> wklist;
        getHGTTop10Share(wklist, QDate::currentDate());
        if(wklist.length() > 0)
        {
            emit DATA_SERVICE->signalRecvTop10ChinaStockInfos(wklist);
            emit signalHSGTofTodayTop10Updated();
            break;
        }
    }

    return;
}

