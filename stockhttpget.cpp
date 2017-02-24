#include "stockhttpget.h"
#include<QDir>
#include<QFile>
#include<QJsonParseError>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>
#include<QDebug>
#include<QDateTime>

static const char * MESSAGETYPESTR = "messagetype";
static const char * MESSAGEVALUESTR = "messagevalue";
static const char * MESSAGECHANSTR = "messagechan";
static const char * MESSAGEURLSTR = "messageurl";

static const char * MESSAGE_WARNLINE = "warnline";
static const char * MESSAGE_WARNZONE = "warnzone";
static const char * MESSAGE_WARNRESULT = "warnresult";


#define MAX_HTTP_POST_STR_LEN  2048
QString sort[6] = {"B","C","E"};
QString market[5] = {"33", "10", "20", "26", "27"};


StockHttpGet::StockHttpGet(const QString& pUrl, QObject *parent)
    :BaseHttpGet(pUrl,parent),
     mSortStyle("33"),
     mSortRule(-1),
     mSortType(SORT_ZHANGDIEFU),
     mHqMode(HQ_STOCKCENTER)
{
    qDebug("sort type = %d", mSortType);
}

StockHttpGet::~StockHttpGet()
{
}



void StockHttpGet::SetCbkFun(HTTPGETCBKFUN pFun,void *pUser )
{
    mFunc = pFun;
    mUser = pUser;
}


void StockHttpGet::slotFinishedReply(QNetworkReply *reply)
{
    //qDebug()<<"url:"<<reply->url();
    //qDebug()<<"receive time:"<<QDateTime::currentDateTime();
    //qDebug()<<"child ThreadID: "<<QThread::currentThreadId()<<endl;
    if(!reply) return;
    //mRepluMutex.lock();
    reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        QString result = QString::fromUtf8(bytes.data());
        int startindex = result.indexOf("[") + 1;
        int endindex = result.indexOf("]");
        if(startindex < 0 || endindex < 0) return;
        QString hqStr = result.mid(startindex, endindex - startindex);
//        qDebug()<<"hqstr:"<<hqStr;
        if( !hqStr.isEmpty() )
        {
            StockDataList wklist;
            QStringList stockInfoList = hqStr.split(QRegExp("\"|\","), QString::SkipEmptyParts);
            //qDebug()<<stockInfoList;
            foreach (QString stockInfo, stockInfoList) {
                QStringList detailList = stockInfo.split(",", QString::SkipEmptyParts);
                if(detailList.length() < 20) continue;
                StockData data;
                data.code = detailList[1];
                data.name = detailList[2];
                data.last_close = detailList[3].toDouble();
                data.open = detailList[4].toDouble();
                data.cur = detailList[5].toDouble();
                data.high = detailList[6].toDouble();
                data.low = detailList[7].toDouble();
                data.money = detailList[8].toInt();
                data.vol = detailList[9].toInt();
                data.chg = detailList[10].toDouble();
                data.per = detailList[11].left(detailList[11].length()-1).toDouble();
                data.hsl = detailList[23].left(detailList[23].length()-1).toDouble();
//                data.code = detailList[1];
//                data.name = detailList[2];
//                data.cur = detailList[3].toDouble();
//                data.per = detailList[4].left(detailList[4].length()-1).toDouble();
                wklist.append(data);
            }
            //json解析
            if( mFunc )
            {
                mFunc(wklist, mUser);
            }

        }

    }
    else
    {

    }

//    reply->deleteLater();
//    reply = 0;
//    //mRepluMutex.unlock();
    BaseHttpGet::slotFinishedReply(reply);
}

void StockHttpGet::ModifyUrl(int type, int rule)
{
    qDebug()<<"rule:"<<rule<<" type:"<<type;
    if(mHqMode == HQ_STOCKCENTER){
        QString url = GetUrl();
        url.replace(QRegExp("sortType=[0-9a-zA-Z]{1,}"), "sortType=" + sort[type]);
        url.replace(QRegExp("sortRule=[-1]{1,}"), "sortRule=" + QString::number(rule));
        url.replace(QRegExp("style=[0-9a-zA-Z]{1,}"), "style=" + mSortStyle);
        qDebug()<<"modify url :" <<url;
        SetUrl(url);
    }
    //http://hqdigi2.eastmoney.com/EM_Quote2010NumericApplication/index.aspx?
    //type=s&sortType=wktype&sortRule=wkrule&pageSize=50&page=1&jsName=quote_123&style=wkstyle

}

void StockHttpGet::SetSortStyle(QString style)
{
    mSortStyle = style;
    ModifyUrl(mSortType, mSortRule);
}

void StockHttpGet::SetSortType(int type)
{
    if(type == mSortType)
    {
       mSortRule =  mSortRule == -1? 1: -1;
    } else {
        mSortRule = -1;
    }
    mSortType = type;
    qDebug()<<"cur sort type:"<<mSortType;
    ModifyUrl(mSortType, mSortRule);
}

void StockHttpGet::SetSortRule(int rule)
{
    mSortRule = rule;
    ModifyUrl(mSortType, mSortRule);
}

void StockHttpGet::SetHqMode(int mode)
{
    mHqMode = mode;
}

int StockHttpGet::GetSortRule()
{
    return mSortRule;
}

int StockHttpGet::GetSortType()
{
    return mSortType;
}

QString StockHttpGet::GetSortStyle()
{
    return mSortStyle;
}

int StockHttpGet::GetHqMode()
{
    return mHqMode;
}

//http://hqdigi2.eastmoney.com/EM_Quote2010NumericApplication/index.aspx?type=s&sortType=wktype&sortRule=wkrule&pageSize=50&page=1&jsName=quote_123&style=wkstyle
//   QString resUrl = mTemplateUrl;
//   resUrl.replace("wktype", sort[mSortType]);
//   resUrl.replace("wkrule", QString::number(mSortRule));
//   return resUrl.replace("wkstyle", QString::number(market[mSortStyle]));


