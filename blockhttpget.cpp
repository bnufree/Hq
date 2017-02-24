#include "blockhttpget.h"

QString block[3] = {"trade", "notion", "area"};


BlockHttpGet::BlockHttpGet(const QString& pUrl, QObject *parent)
    :BaseHttpGet(pUrl,parent),
     mSortRule(-1),
     mSortType(BLOCK_HY)
{
}

BlockHttpGet::~BlockHttpGet()
{
}



void BlockHttpGet::SetCbkFun(BLOCKGETCBKFUN pFun,void *pUser )
{
    mFunc = pFun;
    mUser = pUser;
}

void BlockHttpGet::slotFinishedReply(QNetworkReply *reply)
{
    //qDebug()<<"child ThreadID: "<<QThread::currentThreadId()<<endl;
    reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if(reply->error() == QNetworkReply::NoError)
    {

        QByteArray bytes = reply->readAll();
        QString result = QString::fromLocal8Bit(bytes.data());
        int startindex = -1, endindex = -1;
        if (mSortRule == -1){
            startindex = result.indexOf("[[") + 2;
            endindex = result.indexOf("],[");
        } else {
            startindex = result.indexOf(",[") + 2;
            endindex = result.indexOf("]]}");
        }
        if(startindex < 0 || endindex < 0) return;
        QString hqStr = result.mid(startindex, endindex - startindex);
        //qDebug()<<"blockstr:"<<hqStr;
        if( !hqStr.isEmpty() )
        {
            BlockDataList wklist;
            QStringList blockInfoList = hqStr.split(QRegExp("\"|\","), QString::SkipEmptyParts);
            //qDebug()<<stockInfoList;
            foreach (QString blockInfo, blockInfoList) {
                QStringList detailList = blockInfo.split(",", QString::SkipEmptyParts);
                if(detailList.length() < 11) continue;
                BlockData data;
                data.code = detailList[10] + detailList[9];
                data.name = detailList[0];
                data.changePer = detailList[1].left(detailList[1].length() -1).toDouble();
                data.mktkap = detailList[2].toDouble();
                //qDebug()<<"mketcap:"<<data.mktkap<<" ori:"<<detailList[2];
                data.hsl = detailList[3].toDouble();
                data.stockUpNum = detailList[4].toInt();
                data.stockDownNum = detailList[5].toInt();
//                data.highStockData.code = detailList[6];
//                data.highStockData.name = detailList[7];
//                data.highStockData.per = detailList[8].toDouble();
//                wklist.append(data);
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

   // reply->deleteLater();
    BaseHttpGet::slotFinishedReply(reply);
}

void BlockHttpGet::ModifyUrl(int type)
{

    QString url = GetUrl();
    //http://quote.eastmoney.com/hq2data/bk/data/
    //area.js?v=1450502044118
    url.replace(QRegExp("/data/[a-z]{1,}."), "/data/" + block[type] + ".");
    qDebug()<<"modify url :" <<url;
    SetUrl(url);

}


void BlockHttpGet::SetSortType(int type)
{
    mSortRule = -1;
    mSortType = type;
    qDebug()<<"cur sort type:"<<mSortType;
    ModifyUrl(mSortType);
}

void BlockHttpGet::SetSortRule(int rule)
{
    mSortRule = mSortRule == -1 ? 1 : -1;
    qDebug()<<"sort rule:"<<mSortRule;
    //ModifyUrl(mSortStyle, mSortType, mSortRule);
}


int BlockHttpGet::GetSortRule()
{
    return mSortRule;
}

int BlockHttpGet::GetSortType()
{
    return mSortType;
}

//http://hqdigi2.eastmoney.com/EM_Quote2010NumericApplication/index.aspx?type=s&sortType=wktype&sortRule=wkrule&pageSize=50&page=1&jsName=quote_123&style=wkstyle
//   QString resUrl = mTemplateUrl;
//   resUrl.replace("wktype", sort[mSortType]);
//   resUrl.replace("wkrule", QString::number(mSortRule));
//   return resUrl.replace("wkstyle", QString::number(market[mSortStyle]));



