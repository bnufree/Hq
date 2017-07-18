#include "qeastmoneychinashareexchange.h"
#include "qhttpget.h"
#include <QRegExp>
#include <QDebug>

QEastMoneyChinaShareExchange::QEastMoneyChinaShareExchange(const QDate& pDate, QObject *parent) : QThread(parent)
{
    mWorkDate = pDate;
    qRegisterMetaType<QList<ChinaShareExchange>>("const QList<ChinaShareExchange>&");
}

QEastMoneyChinaShareExchange::~QEastMoneyChinaShareExchange()
{

}

void QEastMoneyChinaShareExchange::run()
{
    QString url = tr("http://www.hkex.com.hk/chi/csm/DailyStat/data_tab_daily_%1c.js?%2")
            .arg(mWorkDate.toString("yyyyMMdd"))
            .arg(QDateTime::currentMSecsSinceEpoch());
    QString result = QString::fromUtf8(QHttpGet().getContent(url));
    //qDebug()<<"result:"<<result;
    //开始解析
    QRegExp reg("\"([0-9]{1,2})\", \"([1-9]{1}[0-9]{0,5})\", \"([\u4E00-\u9FA5]{1,})\", \"(([0-9]{1,},{0,}){1,})\", \"(([0-9]{1,},{0,}){1,})\", \"(([0-9]{1,},{0,}){1,})\"");
    //QRegExp reg("\"([0-9]{1,2})\", \"([1-9]{1}[0-9]{0,5})\", \"([\u4E00-\u9FA5]{1,})\"");

    int index =0;
    QList<ChinaShareExchange> wklist;
    while ((index = reg.indexIn(result, index)) != -1) {
        index += reg.cap().length();
        ChinaShareExchange data;
        data.code = reg.cap(2).length() == 6 ? reg.cap(2) : QString("").sprintf("%06d", reg.cap(2).toInt());
        data.name = reg.cap(3).trimmed();
        data.mTop10Buy = reg.cap(4).replace(",", "") .toDouble();
        data.mTop10Sell = reg.cap(6).replace(",", "").toDouble();
        wklist.append(data);
    }

    emit signalTop10Exchangers(wklist);
}

