#include "qeastmoneyhsgtshareamount.h"
#include "qhttpget.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDate>
#include "dbservices.h"
#include <QDebug>


QEastMoneyHSGTShareAmount::QEastMoneyHSGTShareAmount(QObject *parent) : QThread(parent)
{

}

void QEastMoneyHSGTShareAmount::run()
{
    QDate date = DATA_SERVICE->getLastUpdateDateOfHSGTVol();
    qDebug()<<"last date:"<<date;
    date = date.addDays(1);
    StockDataList list;
    while (date <= QDate::currentDate().addDays(-1)) {
        emit signalAmountFinshedAtDate(tr("外资持股情况更新:%1").arg(date.toString("yyyy-MM-dd")));
        QString url = tr("http://dcfm.eastmoney.com//em_mutisvcexpandinterface/api/js/get?"
                         "type=HSGTHDSTA&token=70f12f2f4f091e459a279469fe49eca5&st=SHAREHOLDSUM&sr=-1&p=1&ps=5000&={pages:(tp),data:(x)}"
                         "&filter=(MARKET%20in%20(%27001%27,%27003%27))(HDDATE=^%1^)")
                .arg(date.toString("yyyy-MM-dd"));
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(QHttpGet().getContent(url), &err);
        if(err.error == QJsonParseError::NoError)
        {
            if(doc.isArray())
            {
                //开始解析
                QJsonArray result = doc.array();
                for(int i=0; i<result.size(); i++)
                {
                    QJsonObject obj = result.at(i).toObject();
                    StockData data;
                    data.mCode = obj.value("SCODE").toString();
                    data.mForeignVol = (qint64)(obj.value("SHAREHOLDSUM").toDouble());
                    data.mDate = date;
                    list.append(data);
                }
            }
        }


        date = date.addDays(1);

    }
    qDebug()<<"list length:"<<list.length();
    if(list.length() > 0)
    {
        DATA_SERVICE->slotAddShareAmoutByForeigner(list);

    //emit DATA_SERVICE->signalAddShareAmoutByForeigner(list);
    }

    return;
}
