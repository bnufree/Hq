#include "qeastmoneychinashareexchange.h"
#include "qhttpget.h"
#include "QRegExp"

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
    //开始解析
    QRegExp reg("\"([0-9]{1,2})\", \"([1-9]{1}[0-9]{0,5})\", \"([^\x00-\xff]{1,})\", \"(([0-9]{1,},{0,}){1,})\", \"(([0-9]{1,},{0,}){1,})\", \"(([0-9]{1,},{0,}){1,})\"");


}

