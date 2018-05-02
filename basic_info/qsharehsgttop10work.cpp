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
        int market_type = obj.value("MarketType").toInt();
        if(market_type == 2 || market_type == 4) continue;
        ShareBaseData data;
        data.setCode(ShareBaseData::fullCode(obj.value("Code").toString()));
        data.setName(obj.value("Name").toString());
        if(data.mCode[2] == '6')
        {
            //上海
            data.mHKExInfo.mBuyMoney = obj.value("HGTMRJE").toDouble();
            data.mHKExInfo.mSellMoney = obj.value("HGTMCJE").toDouble();
        } else
        {
            //深圳
            data.mHKExInfo.mBuyMoney = obj.value("SGTMRJE").toDouble();
            data.mHKExInfo.mSellMoney = obj.value("SGTMCJE").toDouble();
        }
        data.mHKExInfo.mIsTop10 = true;
        data.mHKExInfo.mPureMoney = data.mHKExInfo.mBuyMoney - data.mHKExInfo.mSellMoney;

        list.append(data);
    }
    if(list.size() > 0)
    {
        qSort(list.begin(), list.end(), qGreater<ShareBaseData>());
    }



    if(mParent)
    {
        QMetaObject::invokeMethod(mParent, "slotUpdateShareHsgtTop10List", Qt::DirectConnection, Q_ARG(ShareBaseDataList,list ));
    }
}
