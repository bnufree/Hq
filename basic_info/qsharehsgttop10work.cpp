#include "qsharehsgttop10work.h"
#include "utils/qhttpget.h"
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include "utils/hqutils.h"
#include "utils/sharedata.h"

QShareHsgtTop10Work::QShareHsgtTop10Work(const QString& date, QObject *parent) : mDate(date), mParent(parent),QRunnable()
{

}

QShareHsgtTop10Work::~QShareHsgtTop10Work()
{

}



void QShareHsgtTop10Work::run()
{
    QString url = QString("http://dcfm.eastmoney.com//EM_MutiSvcExpandInterface/api/js/get?type=HSGTCJB&token=70f12f2f4f091e459a279469fe49eca5&filter=(DetailDate=^%1^)&js=(x)&sr=1&st=Rank&rt=50014200")
            .arg(mDate);
    qDebug()<<"url:"<<url;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(QHttpGet().getContentOfURL(url), &err);
    if(err.error != QJsonParseError::NoError) return;
    if(!doc.isArray())  return;
    //开始解析
    QJsonArray result = doc.array();
    ShareBaseDataList list;
    for(int i=0; i<result.size(); i++)
    {
        QJsonObject obj = result.at(i).toObject();
        qDebug()<<obj;
        int market_type = obj.value("MarketType").toInt();
        if(market_type == 2 || market_type == 4) continue;
        ShareBaseData data;
        data.setCode(ShareBaseData::fullCode(obj.value("Code").toString()));
        if(data.mCode[2] == '6')
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

        list.append(data);
    }



    if(list.length() > 0 && mParent)
    {
        QMetaObject::invokeMethod(mParent, "slotUpdateShareHsgtTop10List", Qt::DirectConnection, Q_ARG(ShareBaseDataList,list ));
    }
}
