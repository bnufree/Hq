#include "qsharehsgthistorychangethread.h"
#include <QDate>
#include "utils/qhttpget.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include "dbservices/dbservices.h"

QShareHSGTHistoryChangeThread::QShareHSGTHistoryChangeThread(HSGT_CHG_MODE mode, QObject *parent) :
    QObject(parent),
    mMode(mode)
{

}

void QShareHSGTHistoryChangeThread::run()
{
    QDate date = QDate::currentDate();
    QMap<QString, ShareHsgt> dataMap;
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
                    ShareHsgt &data = dataMap[code];
                    data.mChange = obj.value("Zdf").toDouble();
                    data.mCode = code;
                    data.mDate = QDate::fromString(obj.value("HdDate").toString(),"yyyy-MM-dd");
                    data.mName = obj.value("SName").toString();
                    data.mVolTotal = obj.value("ShareHold").toDouble();
                    data.mVolTotalChangePercent = obj.value("ZZB").toDouble();
                    data.mVolMutablePercent = obj.value("LTZB").toDouble();
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
    if(dataMap.size() > 0) emit DATA_SERVICE->signalUpdateShareHsgtCounter(dataMap.values());
}
