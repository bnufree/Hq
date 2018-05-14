#include "qsharehsgttop10work.h"
#include "utils/qhttpget.h"
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include "utils/hqutils.h"
#include "utils/comdatadefines.h"
#include <QFile>

QShareHsgtTop10Work::QShareHsgtTop10Work(const QString& date, QObject *parent) : mDate(date), mParent(parent),QRunnable()
{
    mFileName = QString("%1%2.data").arg(HQ_HSTOP10_DIR).arg(mDate);
}

QShareHsgtTop10Work::~QShareHsgtTop10Work()
{

}

void QShareHsgtTop10Work::run()
{    
    ShareBaseDataList list;
    //首先从文件获取
    if(QFile::exists(mFileName))
    {
        //读取文件
        QFile file(mFileName);
        if(file.open(QIODevice::ReadOnly))
        {
            while (!file.atEnd() ) {
                ShareBaseData data;
                file.read((char*)(&data), sizeof(ShareBaseData));
                list.append(data);
            }

        }
    }
    if(list.length() == 0)
    {
        //从网络获取
        QString url = QString("http://dcfm.eastmoney.com//EM_MutiSvcExpandInterface/api/js/get?type=HSGTCJB&token=70f12f2f4f091e459a279469fe49eca5&filter=(DetailDate=^%1^)&js=(x)&sr=1&st=Rank&rt=50014200")
                .arg(mDate);
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(QHttpGet().getContentOfURL(url), &err);
        if(err.error != QJsonParseError::NoError) return;
        if(!doc.isArray())  return;
        //开始解析
        QJsonArray result = doc.array();
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
            data.mHKExInfo.mDate = QDate::fromString(mDate, "yyyy-MM-dd");

            list.append(data);
        }
        if(list.size() > 0)
        {
            qSort(list.begin(), list.end(), qGreater<ShareBaseData>());
            //写入文件
            HqUtils::makePath(HQ_HSTOP10_DIR);
            FILE *fp = fopen(mFileName.toStdString().data(), "wb+");
            if(fp)
            {
                for(int i=0; i<list.size(); i++)
                {
                    fwrite(&(list[i]), sizeof(ShareBaseData), 1, fp);
                }
                fclose(fp);
            }
        }
    }
    if(mParent)
    {
        QMetaObject::invokeMethod(mParent, "slotUpdateShareHsgtTop10List", Qt::DirectConnection, Q_ARG(ShareBaseDataList,list ));
    } else
    {
        emit signalChinaAShareTop10Updated(list, mDate);
    }
}
