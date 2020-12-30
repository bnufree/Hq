#include "qsharecodeswork.h"
#include <QDebug>
#include <QDateTime>
#include <QRegularExpression>
#include <QRegExp>
#include <QFile>
#include <QDir>
#include "dbservices/hqdatadefines.h"
#include "dbservices/dbservices.h"
#include <QTextCodec>
#include "utils/qhttpget.h"
#include "utils/hqinfoparseutil.h"
#include "data_structure/hqutils.h"
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "utils/profiles.h"
#include <QtConcurrent>
#include <QMutex>

struct ShareCodeDate{
    QString         code;
    QString         date;
    bool            valid;
};
ShareCodeDate updateShetfDate(const QString &code)
{
    ShareCodeDate res;
    res.valid = false;
    QDate start, end;
    HqInfoParseUtil::getShareDateRange(code, start, end);
    if(start.isValid())
    {
        res.code = code;
        res.valid = true;
        res.date = start.toString("yyyy-MM-dd");
    }

    return res;
}



enum MarketType{
    Market_SH,
    Market_SZ,
    Market_KZZ,
    Market_SZ_ETF,
    Market_SH_ETF,
};

QShareCodesWork::QShareCodesWork(QObject *parent) : QThread(parent)
{
}

QShareCodesWork::~QShareCodesWork()
{
}

void QShareCodesWork::parseKZZ()
{
    qDebug()<<"parse kzz start";
    QString url = "http://dcfm.eastmoney.com/em_mutisvcexpandinterface/api/js/get?type=KZZ_LB&token=70f12f2f4f091e459a279469fe49eca5&st=STARTDATE&sr=-1&p=1&ps=10000&js=";
    QString result = QString::fromUtf8(QHttpGet::getContentOfURL(url));

    int start_index = result.indexOf("[", 0);
    if(start_index < 0) return;
    result = result.mid(start_index);
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(result.toUtf8(), &error);
    if(error.error != QJsonParseError::NoError) return;
    if(!doc.isArray()) return;
    QJsonArray array = doc.array();
    for(int i=0; i<array.size(); i++)
    {
        QJsonObject obj = array.at(i).toObject();
        if(obj.value("LISTDATE").toString() == "-") continue;
        if(obj.value("DELISTDATE").toString() != "-") continue;
        ShareData data;
        data.mName = obj.value("SNAME").toString();
        data.mCode = obj.value("BONDCODE").toString();
        data.mVol = obj.value("AISSUEVOL").toDouble() * 1000000;
        data.mPY = HqUtils::GetFirstLetter(QTextCodec::codecForLocale()->toUnicode(data.mName.toUtf8()));
        data.mShareType = ShareData::shareType(data.mCode);
        data.mListTime = obj.value("LISTDATE").toString().left(10);
        mShareCodeMap[data.mCode] = data;
    }
    qDebug()<<"parse kzz end";
}

void QShareCodesWork::run()
{
    QDate update_date = DATA_SERVICE->getLastUpdateDateOfBasicInfo();
    qDebug()<<"last code date:"<<update_date;
    if(update_date.isNull() || update_date < QDate::currentDate())
    {
//        QString stock_code_url = "http://18.push2.eastmoney.com/api/qt/clist/get?cb=&pn=1&pz=20000&po=0&np=1&ut=bd1d9ddb04089700cf9c27f6f7426281&fltt=2&invt=2&fid=f6&fs=m:0+t:6,m:0+t:13,m:0+t:80,m:1+t:2,m:1+t:23&fields=f12,f14&_=";
//        QString fund_code_url = "http://18.push2.eastmoney.com/api/qt/clist/get?cb=&pn=1&pz=10000&po=0&np=1&ut=bd1d9ddb04089700cf9c27f6f7426281&fltt=2&invt=2&fid=f12&fs=b:MK0021,b:MK0022,b:MK0023,b:MK0024&fields=f12,f14&_=";
//        fund_code_url.append(QString::number(QDateTime::currentMSecsSinceEpoch()));
        //股票数
        parseShCode();
        parseSzCode();
        parseKZZ();
        parseShEtf();
        parseSzEtf();
    }
    foreach(ShareData data, mShareCodeMap)
    {
        qDebug()<<data.mCode<<data.mName<<data.mListTime<<ShareData::shareTypeString(SHARE_DATA_TYPE(data.mShareType))<<data.mShareType;
    }
    DATA_SERVICE->signalUpdateShareBasicInfo(mShareCodeMap.values());
    return;
}



void QShareCodesWork::parseHttp(ShareDataList& list, const QString& url, int mode)
{
#if 0
    QTime t;
    t.start();
    QByteArray http = QHttpGet::getContentOfURL(url);
    QTextCodec *UTF8 = QTextCodec::codecForName("UTF-8");
    QString result = QString::fromUtf8(http);
    qDebug()<<result.left(4000);
    QRegExp reg("([0-9]{6}),([0-9]{0,}[\u4e00-\u9fffA-Z\*]{1,}[0-9]{0,}[\u4e00-\u9fffA-Z\*]{1,}[0-9]{0,})");  //([\u4e00-\u9fffA-Z0-9\*]{1,})
//    QString utf8_result = QString::fromUtf8(UTF8->fromUnicode(result));
    QRegExp reg_code("6[0-9]{5}|300[0-9]{3}|00[012][0-9]{3}|51[0-9]{4}|159[0-9]{3}");
    if(mode == 2)
    {
        reg_code.setPattern(QString("51[0-9]{4}|159[0-9]{3}"));
        reg.setPattern(QString("\"f12\":\"([0-9]{6})\",\"f14\":\"([\u4e00-\u9fffA-Z\*]{1,})\""));
    }
    int index = 0;
    while((index = reg.indexIn(result, index)) >= 0)
    {
        QString name = reg.cap(2);
        QString code = reg.cap(1);
        if(reg_code.exactMatch(code))
        {
            ShareData data;
            data.mCode = code;
            data.mName = name;
            data.mShareType = ShareData::shareType(code);
//            qDebug()<<data.mCode<<data.mName<<name.toUtf8().toHex()<<name.toUtf8().size();
            data.mPY = HqUtils::GetFirstLetter(UTF8->toUnicode(name.toUtf8()));
            qDebug()<<data.mCode<<data.mName<<data.mPY;
            list.append(data);
        }
        index += reg.matchedLength();
    }
#endif
    QJsonDocument doc = QJsonDocument::fromJson(QHttpGet::getContentOfURL(url));
    if(!doc.isObject()) return;
    QTextCodec *UTF8 = QTextCodec::codecForName("UTF-8");
    QJsonArray array = doc.object().value("data").toObject().value("diff").toArray();
    foreach (QJsonValue val, array) {
        ShareData data;
        data.mCode = val.toObject().value("f12").toString();
        data.mName = val.toObject().value("f14").toString();;
        data.mShareType = ShareData::shareType(data.mCode);
//            qDebug()<<data.mCode<<data.mName<<name.toUtf8().toHex()<<name.toUtf8().size();
        data.mPY = HqUtils::GetFirstLetter(UTF8->toUnicode(data.mName.toUtf8()));
        list.append(data);
    }
}

void QShareCodesWork::parseShCode()
{
    qDebug()<<"parse sh start";
    QList<QNetworkCookie> list;
    list.append(QNetworkCookie("Referer", "http://www.sse.com.cn/market/funddata/volumn/etfvolumn/"));
    list.append(QNetworkCookie("Cookie", "yfx_c_g_u_id_10000042=_ck20072115125217721926968350309; yfx_f_l_v_t_10000042=f_t_1595315572764__r_t_1595579117057__v_t_1595579117057__r_c_1; VISITED_MENU=%5B%228864%22%2C%228528%22%2C%2210025%22%2C%228547%22%2C%228491%22%5D"));

    QList<int> typeList;
    typeList<<1<<8;
    foreach (int type, typeList) {
        QByteArray recv = QHttpGet::getContentOfURL(QString("http://query.sse.com.cn/security/stock/getStockListData.do?&jsonCallBack=&isPagination=true&stockCode=&csrcCode=&areaName=&stockType=%1&pageHelp.cacheSize=1&pageHelp.beginPage=1&pageHelp.pageSize=250000&pageHelp.pageNo=1&_=%2").arg(type).arg(QDateTime::currentMSecsSinceEpoch()), list);
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(recv, &error);
        if(!doc.isObject()) return;
        QJsonArray dataArray = doc.object().value("pageHelp").toObject().value("data").toArray();
        foreach (QJsonValue val, dataArray) {
            QJsonObject obj = val.toObject();
            ShareData data;
            data.mCode = obj.value("COMPANY_CODE").toString();
            data.mName = obj.value("COMPANY_ABBR").toString();
            data.mShareType = ShareData::shareType(data.mCode);
            data.mPY = HqUtils::GetFirstLetter(QTextCodec::codecForLocale()->toUnicode(data.mName.toUtf8()));
            data.mListTime = obj.value("LISTING_DATE").toString().left(10);
            mShareCodeMap[data.mCode] = data;

        }
    }
    qDebug()<<"parse sh end";
}


void QShareCodesWork::parseSzCode()
{
    qDebug()<<"parse sz start";
    int page_num = 1;
    int page_size = 10000;
    int page_count = 1;
    int total_record_count = 0;
    while (page_num <= page_count) {
        QString url = QString("http://www.szse.cn/api/report/ShowReport/data?SHOWTYPE=JSON&CATALOGID=1110&TABKEY=tab1&tab1PAGENO=%1&tab1PAGESIZE=%2&random=0.9825325365046125")
                .arg(page_num)
                .arg(page_size);
        QByteArray recv = QHttpGet::getContentOfURL(url);
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(recv, &error);
        if(!doc.isArray()) return;
        QJsonArray array = doc.array();
        QJsonObject metaObj = array[0].toObject().value("metadata").toObject();
        if(metaObj.isEmpty()) break;
        QDate curDate = QDate::fromString(metaObj.value("subname").toString().trimmed(), "yyyy-MM-dd");
        if(!curDate.isValid()) break;
        page_count = metaObj.value("pagecount").toInt();
        qDebug()<<curDate<<page_count;
        if(page_count <= 0) break;
        total_record_count = metaObj.value("recordcount").toInt();

        QJsonArray dataArray = array[0].toObject().value("data").toArray();
        QRegExp nameEXp("<u>(.{1,})</u>");
        foreach (QJsonValue val, dataArray) {
            QJsonObject obj = val.toObject();
            QString code = obj.value("agdm").toString();
            QString name = obj.value("agjc").toString().remove(" ");
            int index = nameEXp.indexIn(name);
            if(index > 0)
            {
                name = nameEXp.cap(1);
            }
            ShareData data;
            data.mCode = code;
            data.mName = name;
            data.mShareType = ShareData::shareType(data.mCode);
            data.mListTime = obj.value("agssrq").toString();
            data.mPY = HqUtils::GetFirstLetter(QTextCodec::codecForLocale()->toUnicode(data.mName.toUtf8()));
            QRegExp py("[A-Z]{1,}");
            int index2 = py.indexIn(data.mPY);
            int length = py.matchedLength();
            data.mPY = data.mPY.mid(index2, length);
            mShareCodeMap[data.mCode] = data;
        }
        page_num++;
    }
    qDebug()<<"parse sz end";

}

void QShareCodesWork::parseSzEtf()
{
    qDebug()<<"start sz etf";
    int page_num = 1;
    int page_size = 10000;
    int page_count = 1;
    int total_record_count = 0;
    while (page_num <= page_count) {
        QString url = QString("http://www.szse.cn/api/report/ShowReport/data?SHOWTYPE=JSON&CATALOGID=1105&TABKEY=tab1&selectJjlb=ETF&selectTzlb=股票基金&tab1PAGENO=%1&tab1PAGESIZE=%2&random=0.48925979447422674")
                .arg(page_num)
                .arg(page_size);
        QByteArray recv = QHttpGet::getContentOfURL(url);
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(recv, &error);
        if(!doc.isArray()) return;
        QJsonArray array = doc.array();
        QJsonObject metaObj = array[0].toObject().value("metadata").toObject();
        if(metaObj.isEmpty()) break;
        QString date_str = metaObj.value("subname").toString().trimmed();
        QDate curDate = QDate::fromString(date_str, "yyyy-MM-dd");
        if(!curDate.isValid()) break;
        page_count = metaObj.value("pagecount").toInt();
        if(page_count <= 0) break;
        total_record_count = metaObj.value("recordcount").toInt();

        QJsonArray dataArray = array[0].toObject().value("data").toArray();
        QRegExp codeExp("<u>(\\d{6})</u>");
        QRegExp nameEXp("<u>(.{1,})</u>");
        foreach (QJsonValue val, dataArray) {
            QJsonObject obj = val.toObject();
            QString code = obj.value("sys_key").toString();
            int index = codeExp.indexIn(code);
            if(index > 0)
            {
                code = codeExp.cap(1);
            }
            QString name = obj.value("jjjcurl").toString();
            index = nameEXp.indexIn(name);
            if(index > 0)
            {
                name = nameEXp.cap(1);
            }
            double vol = obj.value("dqgm").toString().remove(",").toDouble();

            ShareData data;
            data.mCode = code;
            data.mName = name;
            data.mShareType = ShareData::shareType(data.mCode);
            data.mPY = HqUtils::GetFirstLetter(QTextCodec::codecForLocale()->toUnicode(data.mName.toUtf8()));
            data.mFinanceData.mTotalShare = vol;
            data.mListTime = obj.value("ssrq").toString();
            mShareCodeMap[data.mCode] = data;
        }
        page_num++;
    }
    qDebug()<<"parse szetf end";
}

void QShareCodesWork::parseShEtf()
{
    qDebug()<<"parse shetf start";
    QList<QNetworkCookie> list;
    list.append(QNetworkCookie("Referer", "http://www.sse.com.cn/market/funddata/volumn/etfvolumn/"));
    list.append(QNetworkCookie("Cookie", "yfx_c_g_u_id_10000042=_ck20072115125217721926968350309; yfx_f_l_v_t_10000042=f_t_1595315572764__r_t_1595579117057__v_t_1595579117057__r_c_1; VISITED_MENU=%5B%228864%22%2C%228528%22%2C%2210025%22%2C%228547%22%2C%228491%22%5D"));

    QByteArray recv = QHttpGet::getContentOfURL(QString("http://query.sse.com.cn/commonQuery.do?&jsonCallBack=&isPagination=true&sqlId=COMMON_SSE_ZQPZ_ETFZL_XXPL_ETFGM_SEARCH_L&pageHelp.pageSize=10000&STAT_DATE=%1&_=%2")
                                                .arg(TradeDateMgr::instance()->lastTradeDay().toString("yyyy-MM-dd")).arg(QDateTime::currentDateTime().toMSecsSinceEpoch()), list);
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(recv, &error);
    if(!doc.isObject()) return;
    QJsonArray dataArray = doc.object().value("pageHelp").toObject().value("data").toArray();
    QList<QString> need_update_list;
    foreach (QJsonValue val, dataArray) {
        QJsonObject obj = val.toObject();
        QString code = obj.value("SEC_CODE").toString();
        QString name = obj.value("SEC_NAME").toString();
        double vol = obj.value("TOT_VOL").toString().toDouble();
        ShareData data;
        data.mCode = code;
        data.mName = name;
        data.mShareType = ShareData::shareType(data.mCode);
        data.mPY = HqUtils::GetFirstLetter(QTextCodec::codecForLocale()->toUnicode(data.mName.toUtf8()));
        data.mFinanceData.mTotalShare = vol;
        QString list_time = DATA_SERVICE->getShareData(data.mCode).mListTime;
        if(list_time.isEmpty())
        {
            need_update_list.append(code);
        } else
        {
            data.mListTime = list_time;
        }
        mShareCodeMap[data.mCode] = data;
    }

    //重新获取上海ETF的上市时间
    if(need_update_list.size() > 0)
    {
        QList<ShareCodeDate> res = QtConcurrent::blockingMapped(need_update_list, updateShetfDate);
        foreach (ShareCodeDate now, res) {
            mShareCodeMap[now.code].mListTime = now.date;
        }
    }
    qDebug()<<"parse shetf end";
}

