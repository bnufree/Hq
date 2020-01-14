#include "hqkuaixun.h"
#include <QDateTime>
#include "utils/qhttpget.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

HqKuaixun::HqKuaixun(QObject *parent) : QThread(parent)
{
    qRegisterMetaType<KuaiXunList>("const KuaiXunList&");

}

void HqKuaixun::run()
{

    while (true) {
        msleep(1000);
        KuaiXunList list;
        parseEastMoney(list);
        parseThs(list);
        if(list.size() > 0)
        {
            std::stable_sort(list.begin(), list.end(), std::greater<KuaixunData>());
            emit signalSendKuaiXun(list);
        }
    }
}

void HqKuaixun::parseEastMoney(KuaiXunList &resList)
{
    static quint64 last_id = -1;
    QString url = QString("http://newsapi.eastmoney.com/kuaixun/v1/getlist_102_ajaxResult_50_1_.html?r=0.4051267500266684&_=%1").arg(QDateTime::currentMSecsSinceEpoch());
    QString result = QString::fromUtf8(QHttpGet::getContentOfURL(url));
    int start_index = result.indexOf("{", 0);
    if(start_index < 0) return;
    result = result.mid(start_index);
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(result.toUtf8(), &error);
    if(error.error != QJsonParseError::NoError) return;
    if(!doc.isObject()) return;
    QJsonValue list = doc.object().value("LivesList");
    if(!list.isArray()) return;
    QJsonArray array = list.toArray();
    qint64 first_id = -1;
    for(int i=0; i<array.size(); i++)
    {
        QJsonObject obj = array.at(i).toObject();
        qint64 new_id = obj.value("id").toString().toLongLong();
        if(new_id == last_id ) break;
        KuaixunData data;
        data.title = obj.value("title").toString();
        data.digest = obj.value("digest").toString();
        data.url = obj.value("url_m").toString();
        data.time = obj.value("showtime").toString();
        data.strid = QString("%1_%2").arg(KuaixunData::Source_EastMoney).arg(new_id);
        data.source = KuaixunData::Source_EastMoney;
        resList.append(data);
        if(first_id == -1) first_id = new_id;
    }
    if(first_id > 0) last_id = first_id;
}

void HqKuaixun::parseThs(KuaiXunList &resList)
{
    static quint64 last_ths_id = -1;
    QString url = QString("http://news.10jqka.com.cn/tapp/news/push/stock/?page=1&tag=&track=website");
    QString result = QString::fromUtf8(QHttpGet::getContentOfURL(url));
    int start_index = result.indexOf("{", 0);
    if(start_index < 0) return;
    result = result.mid(start_index);
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(result.toUtf8(), &error);
    if(error.error != QJsonParseError::NoError) return;
    if(!doc.isObject()) return;
    QJsonValue list = doc.object().value("data");
    if(!list.isObject()) return;
    list = list.toObject().value("list");
    if(!list.isArray()) return;
    QJsonArray array = list.toArray();
    qint64 first_id = -1;
    for(int i=0; i<array.size(); i++)
    {
        QJsonObject obj = array.at(i).toObject();
        qint64 new_id = obj.value("seq").toString().toLongLong();
        if(new_id == last_ths_id ) break;
        KuaixunData data;
        data.title = obj.value("title").toString();
        data.digest = obj.value("digest").toString();
        data.url = obj.value("appUrl").toString();
        data.time = QDateTime::fromTime_t(obj.value("ctime").toString().toUInt()).toString("yyyy-MM-dd hh:mm:ss");
        data.strid = QString("%1_%2").arg(KuaixunData::Source_Ths).arg(new_id);
        data.source = KuaixunData::Source_Ths;
        resList.append(data);
        if(first_id == -1) first_id = new_id;
    }
    if(first_id > 0) last_ths_id = first_id;
}
