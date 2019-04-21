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

QShareCodesWork::QShareCodesWork(QObject *parent) : QThread(parent)
{
}

QShareCodesWork::~QShareCodesWork()
{
}

void QShareCodesWork::run()
{
    //首先获取数据库的更新日期
    //http://query.sse.com.cn/security/stock/downloadStockListFile.do?csrcCode=&stockCode=&areaName=&stockType=1

    ShareDataList list;
    ShareDate update_date = DATA_SERVICE->getLastUpdateDateOfBasicInfo();
    qDebug()<<update_date.date()<<ShareDate::latestActiveDay().date()<<(update_date != ShareDate::latestActiveDay());
    if(update_date < ShareDate::latestActiveDay())
    {
        QString stock_code_url = tr("http://nufm.dfcfw.com/EM_Finance2014NumericApplication/JS.aspx?cb=&type=CT&token=4f1862fc3b5e77c150a2b985b12db0fd&sty=FCOIATC&js=(%7Bdata%3A%5B(x)%5D%2CrecordsFiltered%3A(tot)%7D)&cmd=C._A&st=(ChangePercent)&sr=-1&p=1&ps=20000&_=%1").arg(QDateTime::currentMSecsSinceEpoch());
        QString fund_code_url = "http://nufm.dfcfw.com/EM_Finance2014NumericApplication/JS.aspx?cb=&type=CT&token=4f1862fc3b5e77c150a2b985b12db0fd&sty=FCOIATC&js=(%7Bdata%3A%5B(x)%5D%2CrecordsFiltered%3A(tot)%7D)&cmd=C.__2850013&st=(Amount)&sr=-1&p=1&ps=2000&_=1555572794676";
        parseHttp(list, stock_code_url, 1);
        parseHttp(list, fund_code_url, 2);
    }
    DATA_SERVICE->signalUpdateShareBasicInfo(list);
    return;
}

void QShareCodesWork::parseHttp(ShareDataList& list, const QString& url, int mode)
{
    QTime t;
    t.start();
    QByteArray http = QHttpGet::getContentOfURL(url);
    QTextCodec *UTF8 = QTextCodec::codecForName("UTF-8");
    QString result = QString::fromUtf8(http);
    QRegExp reg("([0-9]{6}),([\u4e00-\u9fffA-Z\*]{1,})");  //([\u4e00-\u9fffA-Z0-9\*]{1,})
    QString utf8_result = QString::fromUtf8(UTF8->fromUnicode(result));
    QRegExp reg_code("60[013][0-9]{3}|300[0-9]{3}|00[012][0-9]{3}|51[0-9]{4}|159[0-9]{3}");
    if(mode == 2)
    {
        reg_code.setPattern(QString("51[0-9]{4}|159[0-9]{3}"));
    }
    int index = 0;
    while((index = reg.indexIn(utf8_result, index)) >= 0)
    {
        QString name = reg.cap(2);
        QString code = reg.cap(1);
        if(reg_code.exactMatch(code))
        {
            ShareData data;
            data.mCode = code;
            data.mName = name;
            data.mShareType = ShareData::shareType(code);
            //qDebug()<<data.mCode<<data.mName<<name.toUtf8().toHex()<<name.toUtf8().size();
            data.mPY = HqUtils::GetFirstLetter(UTF8->toUnicode(name.toUtf8()));
//            qDebug()<<data.mCode<<data.mName<<data.mPY;
            list.append(data);
        }
        index += reg.matchedLength();
    }
}

//


