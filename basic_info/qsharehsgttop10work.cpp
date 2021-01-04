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
        qDebug()<<"hsgt top10 last date:"<<ShareTradeDateTime(last_update_date).toString()<<last_update_date.isNull();
        if(last_update_date.isNull())
        {
            last_update_date = QDate::currentDate().addYears(-1);
        } else
        {
            last_update_date = last_update_date.addDays(1);
        }

        QDate final_date = last_update_date;
        ShareHsgtList list;
        qDebug()<<"start get north top10 from date:"<<last_update_date.toString("yyyy-MM-dd");
        while (last_update_date <= QDate::currentDate()) {
            int old_size = list.size();
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
            qDebug()<<"hstop10:"<<last_update_date<<(new_size - old_size)<<"final date:"<<final_date;
            last_update_date = last_update_date.addDays(1);
        }

        if(list.size() > 0)
        {
            DATA_SERVICE->signalUpdateShareHsgtTop10Info(list);
            DATA_SERVICE->signalUpdateHsgtTop10Keys(final_date);
        }

        sleep(60);
    }
}

bool QShareHsgtTop10Work::getDataFromEastMoney(ShareHsgtList &list, const QDate &date)
{
    qDebug()<<"start get from eastmoney"<<date;
    //从网络获取.
    QString url = QString("http://dcfm.eastmoney.com//EM_MutiSvcExpandInterface/api/js/get?type=HSGTCJB&token=70f12f2f4f091e459a279469fe49eca5&filter=(DetailDate=^%1^)&js=(x)&sr=1&st=Rank&rt=50014200")
            .arg(date.toString("yyyy-MM-dd"));
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(QHttpGet::getContentOfURL(url), &err);
    if(err.error != QJsonParseError::NoError)
    {
        qDebug()<<"east money json error:"<<date<<err.errorString();
        return false;
    }
    if(!doc.isArray())
    {
        qDebug()<<"east money content error:"<<date;
        return false;
    }
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
        qDebug()<<"EAST"<<data.mCode<<data.mName<<data.mPure;
        resList.append(data);
    }
    if(resList.size() > 0) list.append(resList);

    return resList.size() > 0;
}

bool QShareHsgtTop10Work::getDataFromHKEX(ShareHsgtList &list, const QDate &date)
{
    QByteArray recv = QHttpGet::getContentOfURL(QString("http://sc.hkex.com.hk/TuniS/www.hkex.com.hk/chi/csm/DailyStat/data_tab_daily_%1c.js?_=%2").arg(date.toString("yyyyMMdd"))
                                                .arg(QDateTime::currentDateTime().toMSecsSinceEpoch()));
    ShareHsgtList resList;
    int index = recv.indexOf("[");
    if(index >= 0) recv = recv.mid(index);
    QString result = QString::fromUtf8(recv).remove(QRegExp("[\\r\\n\\t]"));
    QRegularExpression start_reg("\\[\\[\"[0-9]{1,2}\"");
    int start_index = 0;
    while ((start_index = result.indexOf(start_reg, start_index)) >= 0) {
        int end_index = result.indexOf("]]", start_index);
        if(end_index == -1) break;
        QString line = result.mid(start_index, end_index - start_index+2);
        QStringList lineList = line.split("\", \"", QString::SkipEmptyParts);
        start_index = end_index;
        if(lineList.size() == 6)
        {
            ShareHsgt data;
            data.mCode = lineList[1];
            if(data.mCode.size() == 5) continue;
            int code = data.mCode.toInt();
            data.mCode = QString("").sprintf("%06d", code);
            data.mName = lineList[2].trimmed();
            data.mBuy = lineList[3].remove(",").toDouble();
            data.mSell = lineList[4].remove(",").toDouble();
            data.mTotal = data.mBuy + data.mSell;
            data.mPure = data.mBuy - data.mSell;
            data.mIsTop10 = true;
            data.mDate = date;
            qDebug()<<"HKEX"<<data.mCode<<data.mName<<data.mPure;
            resList.append(data);
        }
    }
    if(resList.size()) list.append(resList);
    return resList.size() > 0;
}
