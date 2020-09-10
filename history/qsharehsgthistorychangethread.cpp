#include "qsharehsgthistorychangethread.h"
#include <QDate>
#include "utils/qhttpget.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

QShareHSGTHistoryChangeThread::QShareHSGTHistoryChangeThread(QObject *parent) : QObject(parent)
{

}

void QShareHSGTHistoryChangeThread::run()
{
    QDate date = QDate::currentDate();
    QMap<QString, ShareHSGTHistoryData> dataMap;
    for(int i=HSGT_CHG_1DAY; i<=HSGT_CHG_YEAR; i++)
    {
        QString type = "";
        switch (i) {
        case HSGT_CHG_1DAY:
            type = "1";
            break;
        case HSGT_CHG_3DAY:
            type = "3";
            break;
        case HSGT_CHG_5DAY:
            type = "5";
            break;
        case HSGT_CHG_10DAY:
            type = "10";
            break;
        case HSGT_CHG_MONTH:
            type = "m";
            break;
        case HSGT_CHG_SEASON:
            type = "jd";
            break;
        case HSGT_CHG_YEAR:
            type = "y";
            break;
        default:
            break;
        }
        if(type.isEmpty()) break;
        int cur_page = 1;
        int total_page = 0;
        while (1) {
            QString url = QString("http://dcfm.eastmoney.com/EM_MutiSvcExpandInterface/api/js/get?type=HSGT20_GGTJ_SUM&token=894050c76af8597a853f5b408b759f5d&st=ShareSZ_Chg_One&sr=1&p=%1&ps=5000&js={pages:(tp),data:(x)}&filter=(DateType='%2' and HdDate='%3')&rt=53323242")
                    .arg(cur_page).arg(type).arg(date.toString("yyyy-MM-dd"));
            QJsonDocument doc = QJsonDocument::fromJson(QHttpGet::getContentOfURL(url));
            if(doc.isObject())
            {
                QJsonObject obj = doc.object();
                total_page = obj.value("pages").toInt();
                if(total_page == 0) date = date.addDays(-1);
                continue;
                //parse
                QJsonArray array = obj.value("data").toArray();
                for(int i=0; i<array.size();i++)
                {
                    obj = array[i].toObject();
                    QString code = obj.value("SCode").toString();
                    ShareHSGTHistoryData &data = dataMap[code];
                    data.mChgPercent = obj.value("Zdf").toDouble();
                    data.mCode = code;
                    data.mCurPrice = obj.value("NewPrice").toDouble();
                    data.mDate = obj.value("HdDate").toString();
                    data.mHYCode = obj.value("ORIGINALCODE").toString();
                    data.mJGNum = int(obj.value("JG_SUM").toDouble());
                    data.mJGShareRate = obj.value("SharesRate").toDouble();
                    data.mName = obj.value("SName").toString();
                    data.mTotalShareMktCap = obj.value("ZSZ").toDouble();
                    data.mMutalShareMktCap = obj.value("LTSZ").toDouble();
                    data.mNowHSGTTotalShare = obj.value("ShareHold").toDouble();
                    data.mNowHSGTTotalSharePercent = obj.value("ZZB").toDouble();
                    data.mNowHSGTMutalSharePercent = obj.value("LTZB").toDouble();
                    ShareHSGTChgData chg;
                    chg.mVolChg = obj.value("ShareHold_Chg_One").toDouble();
                    chg.mVolMutalChangePercent = obj.value("LTZB_One").toDouble();
                    chg.mVolToalChgPercent = obj.value("ZZB_One").toDouble();
                    data.mCounterMap[obj.value("DateType").toString()] = chg;
                }
                if(cur_page == total_page) break;
                cur_page++;

            }
        }

    }
}
