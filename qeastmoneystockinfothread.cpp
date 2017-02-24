#include "qeastmoneystockinfothread.h"
#include <QDateTime>
#include <QNetworkAccessManager>

#include <QNetworkReply>
#include <QEventLoop>
#include <QDebug>
#include "stkmktcap.h"
#include "stockdata.h"
#include "stkinfofilemanage.h"

qeastmoneystockinfothread::qeastmoneystockinfothread(QThread *parent) : QThread(parent)
{

}

qeastmoneystockinfothread::~qeastmoneystockinfothread()
{

}

void qeastmoneystockinfothread::setStockCodeList(const QStringList &codes)
{
    mStockcodeList = codes;
}

void qeastmoneystockinfothread::run()
{
//    MktCapFile::instance()->setValue("Update", "time", QDateTime::currentDateTime().toString("yyyy-MM-dd"));
#if 0
    //取得上次更新的时间
    QString dateStre = MktCapFile::instance()->value("Update", "time").toString();
    qDebug()<<"stock info last:"<<dateStre;
    QDateTime lastDate = QDateTime::fromString(dateStre, "yyyy-MM-dd");
    if(lastDate.date().day() == QDateTime::currentDateTime().date().day())
    {
        return;
    }
    QNetworkAccessManager *mgr = new QNetworkAccessManager;
    int i=0;
    int total = mStockcodeList.size();
    QList<StockData> datalist;
    foreach (QString code, mStockcodeList) {
        emit sendUpdateProgress(++i, total);
        code = code.right(6);
        if(code.left(1) == "6" || code.left(1) == "5")
        {
            code += "1";
        } else
        {
            code += "2";
        }
        QString wkURL = QString("http://nufm.dfcfw.com/EM_Finance2014NumericApplication/JS.aspx?type=CT&cmd=P.[(x)]|%1&sty=MPICT&st=z&sr=&p=&ps=&cb=callback&js=&token=aaf32a88de888ea7b0ea63e017ecb049&_=%2")
                .arg(code).arg(QDateTime::currentMSecsSinceEpoch());

        QNetworkReply *reply  = mgr->get(QNetworkRequest(wkURL));
        if(!reply)
        {
            continue;
        }
        QEventLoop loop; // 使用事件循环使得网络通讯同步进行
        connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec(); // 进入事件循环， 直到reply的finished()信号发出， 这个语句才能退出
        if(reply->error())
        {

            reply->deleteLater();
            continue;
        }
        //开始解析数据
        QByteArray bytes = reply->readAll();
        QString result = QString::fromUtf8(bytes.data());
        //qDebug()<<"result:"<<result;
        int startindex = -1, endindex = -1;
        startindex = result.indexOf("[[") + 2;
        endindex = result.indexOf("]]");
        if(startindex < 0 || endindex < 0) continue;
        QString hqStr = result.mid(startindex, endindex - startindex);
        //qDebug()<<"info:"<<hqStr;
        if( !hqStr.isEmpty() )
        {
            QStringList blockInfoList = hqStr.split(QRegExp(","), QString::SkipEmptyParts);
            //qDebug()<<"sections:"<<blockInfoList[1];
            if(blockInfoList.length() > 24)
            {
                QString code = blockInfoList[1];
                qint64     totalA = blockInfoList[20].toLongLong();
                qint64     ltA = blockInfoList[22].toLongLong();
                StockData data;
                data.code = code;
                data.totalshare = totalA;
                data.ltshare = ltA;
                datalist.append(data);
                //emit sendMktCapInfo(code, totalA, ltA);
                //MktCapFile::instance()->setValue(code, "total", totalA);
                //MktCapFile::instance()->setValue(code, "lt", ltA);
                //qDebug()<<"code:"<<code<<"  total:"<<totalA/100000000.0<<" lta:"<<ltA/100000000.0;
            }
        }

        reply->deleteLater();
    }

    foreach (StockData data, datalist) {
        MktCapFile::instance()->setValue(data.code, "total", data.totalshare);
        MktCapFile::instance()->setValue(data.code, "lt", data.ltshare);
    }

    qDebug()<<"stk info update finished";
    MktCapFile::instance()->setValue("Update", "time", QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    qDebug()<<"stk info update finished";
    mgr->deleteLater();
#else
    //下载前10天的数据，如果数据
    QNetworkAccessManager *mgr = new QNetworkAccessManager;
    foreach (QString code, mStockcodeList) {
        emit sendUpdateProgress(1, 1);
        bool update = true;
        StkInfoFileManage *filemgr = new StkInfoFileManage(code.right(6));
        QStringList secs = filemgr->getAllSections();
        //检查旧的时间格式，然后删除
        foreach (QString sec, secs) {
            QRegExp exp("[0-9]{4}-[0-9]{2}-[0-9]{2}");
            if(exp.exactMatch(sec))
            {
                filemgr->removeGroup(sec);
            }

        }
        //取得上次更新的时间
        secs = filemgr->subkeys("Dates");
        if(secs.length())
        {
            secs.sort();
            //qDebug()<<"Dates:"<<secs;
            QString dateStre = secs.last();
            QDateTime lastDate = QDateTime::fromString(dateStre, "yyyy-MM-dd");
            if(lastDate.date() == /*QDateTime::currentDateTime().addDays(-1).date()*/lastActiveDay())
            {
                update = false;
            }
        }
        if(update == false) continue;
        //开始更新
        filemgr->removeGroup("Dates");
        QDateTime start = QDateTime::currentDateTime().addDays(-11);
        QDateTime end = QDateTime::currentDateTime().addDays(-1);
        code = code.right(6);
        if(code.left(1) == "6" || code.left(1) == "5")
        {
            code = "0" + code;
        } else
        {
            code = "1" + code;
        }
        //http://quotes.money.163.com/service/chddata.html?code=1000002&start=19910129&end=20161006&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;PCHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP
        QString wkURL = QString("http://quotes.money.163.com/service/chddata.html?code=%1&start=%2&end=%3&fields=TCLOSE;LCLOSE;VOTURNOVER;VATURNOVER;TCAP;MCAP;PCHG")
                .arg(code).arg(start.toString("yyyyMMdd")).arg(end.toString("yyyyMMdd"));

        //qDebug()<<"url:"<<wkURL;
        QNetworkReply *reply  = mgr->get(QNetworkRequest(wkURL));
        if(!reply)
        {
            continue;
        }
        QEventLoop loop; // 使用事件循环使得网络通讯同步进行
        connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec(); // 进入事件循环， 直到reply的finished()信号发出， 这个语句才能退出
        if(reply->error())
        {

            reply->deleteLater();
            continue;
        }
        //开始解析数据
        int index = -1;
        QByteArray bytes;
        int k=0;
        double l3 = 0.0, l5 = 0.0;
        while((bytes = reply->readLine()).length())
        {
            index++;
            if(index == 0) continue;
            QString result = QString::fromLocal8Bit(bytes.data());
            if( !result.isEmpty() )
            {
                QStringList blockInfoList = result.split(QRegExp("[,\"'\s\r\t]"), QString::SkipEmptyParts);

                if(blockInfoList.length() > 9)
                {
                    if(k<3) l3 += blockInfoList[9].toDouble();
                    if(k<5) l5 += blockInfoList[9].toDouble();
                    k++;
                    QString wkDate = blockInfoList[0];
                    QStringList wklist = blockInfoList.mid(6, 3);
                    filemgr->setValue("Dates", wkDate, wklist);
                }
            }

        }
        //计算3日涨幅
        filemgr->setValue("Chg", "L3", l3);
        filemgr->setValue("Chg", "L5", l5);
        //qDebug()<<"code:"<<code<<" L3:"<<l3<<" L5:"<<l5;
        filemgr->deleteLater();
        reply->deleteLater();
    }
    qDebug()<<"stk info update finished";
    mgr->deleteLater();
#endif
}

bool qeastmoneystockinfothread::isActiveDay(QDate date)
{
    if(date.dayOfWeek() == 6 || date.dayOfWeek() == 7) return false;
    return true;
}

QDate qeastmoneystockinfothread::lastActiveDay()
{
    QDate date = QDate::currentDate().addDays(-1);
    while(!isActiveDay(date))
    {
        date = date.addDays(-1);
    }
    return date;
}


