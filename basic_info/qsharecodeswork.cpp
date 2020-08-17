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
#include "data_structure/hqutils.h"
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

QShareCodesWork::QShareCodesWork(QObject *parent) : QThread(parent)
{
}

QShareCodesWork::~QShareCodesWork()
{
}

void QShareCodesWork::parseKZZ(ShareDataList &list)
{
    QString url = "http://dcfm.eastmoney.com/em_mutisvcexpandinterface/api/js/get?type=KZZ_LB&token=70f12f2f4f091e459a279469fe49eca5&st=STARTDATE&sr=-1&p=1&ps=10&js=";
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
        data.mPY = HqUtils::GetFirstLetter(QTextCodec::codecForLocale()->toUnicode(data.mName.toUtf8()));
        data.mShareType = ShareData::shareType(data.mCode);
        list.append(data);
    }
}

void QShareCodesWork::run()
{
    //首先获取数据库的更新日期
    ShareDataList list;
    //股票数据
    parseShCode(list);
    parseSzCode(list);
    //ETF数据
    //可转债数据
//    {
//        //
//        QEtfScaleThread *etf = new QEtfScaleThread(this);
//        connect(etf, SIGNAL(finished()), etf, SLOT(deleteLater()));
//        etf->start();
//    }
    ShareWorkingDate update_date = DATA_SERVICE->getLastUpdateDateOfBasicInfo();
//    if(update_date.isNull() || update_date < ShareWorkingDate::getCurWorkDay())
    {
        QString stock_code_url = "http://18.push2.eastmoney.com/api/qt/clist/get?cb=&pn=1&pz=20000&po=0&np=1&ut=bd1d9ddb04089700cf9c27f6f7426281&fltt=2&invt=2&fid=f6&fs=m:0+t:6,m:0+t:13,m:0+t:80,m:1+t:2,m:1+t:23&fields=f12,f14&_=";
        QString fund_code_url = "http://18.push2.eastmoney.com/api/qt/clist/get?cb=&pn=1&pz=10000&po=0&np=1&ut=bd1d9ddb04089700cf9c27f6f7426281&fltt=2&invt=2&fid=f12&fs=b:MK0021,b:MK0022,b:MK0023,b:MK0024&fields=f12,f14&_=";
        fund_code_url.append(QString::number(QDateTime::currentMSecsSinceEpoch()));
        QTime t;
        t.start();
        parseHttp(list, stock_code_url, 1);
        qDebug()<<"code:"<<t.elapsed();
        t.start();
        parseHttp(list, fund_code_url, 2);
        qDebug()<<"fund:"<<t.elapsed();
    }
  //  DATA_SERVICE->signalUpdateShareBasicInfo(list);
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
        qDebug()<<data.mCode<<data.mName<<data.mPY;
        list.append(data);
    }
}

void QShareCodesWork::parseShCode(ShareDataList &result_list)
{
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
            result_list.append(data);

        }
    }
}


void QShareCodesWork::parseSzCode(ShareDataList &result_list)
{
    int page_num = 1;
    int page_size = 10000;
    int page_count = 1;
    int total_record_count = 0;
    while (page_num <= page_count) {
        QString url = QString("http://www.szse.cn/api/report/ShowReport/data?SHOWTYPE=JSON&CATALOGID=1110&TABKEY=tab1&tab1PAGENO=%1&tab1PAGESIZE=%2&random=0.9825325365046125")
                .arg(page_num)
                .arg(page_size);
        QByteArray recv = QHttpGet::getContentOfURL(url);
        qDebug()<<url<<recv.size();
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
            data.mPY = HqUtils::GetFirstLetter(QTextCodec::codecForLocale()->toUnicode(data.mName.toUtf8()));
            QRegExp py("[A-Z]{1,}");
            int index2 = py.indexIn(data.mPY);
            int length = py.matchedLength();
            data.mPY = data.mPY.mid(index2, length);
            result_list.append(data);
        }
        page_num++;
    }

}

