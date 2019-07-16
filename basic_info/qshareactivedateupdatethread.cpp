#include "qshareactivedateupdatethread.h"
#include "utils/qhttpget.h"
#include "dbservices/dbservices.h"

QShareActiveDateUpdateThread::QShareActiveDateUpdateThread(QObject *parent) : QThread(parent)
{

}

void QShareActiveDateUpdateThread::run()
{
    //首先获取当前的日期列表
    ShareDate cur = ShareDate::currentDate();
    ShareDate start(cur.date().addYears(-1));
    QString wkURL = QString("http://quotes.money.163.com/service/chddata.html?code=0000001&start=%1&end=%2")
            .arg(start.toString("yyyyMMdd")).arg(cur.toString("yyyyMMdd"));
    QByteArray recv = QHttpGet::getContentOfURL(wkURL);
    QTextCodec* gbk = QTextCodec::codecForName("GBK");
    QTextCodec* utf8 = QTextCodec::codecForName("UTF-8");
    QString result = QString::fromUtf8(utf8->fromUnicode(gbk->toUnicode(recv)));

    QStringList lines = result.split("\r\n");
    ShareDateList list;
    for(int i=1; i<lines.length(); i++)
    {
        QStringList cols = lines[i].split(",");
        if(cols.length() >= 15)
        {
            //if(mCode == "000400")qDebug()<<recv.mid(20, 200);
            ShareDate curDate = ShareDate::fromString(cols[0]);
            if(curDate.isWeekend()) continue;
            list.append(curDate);
        }
    }
    ShareDate::setHisWorkingDay(list);
    QDate cur;
    //检查当前时间是不是工作日
    while(true)
    {
        QString wkURL = QString("http://hq.sinajs.cn/list=sh000001");
        QByteArray recv = QHttpGet::getContentOfURL(wkURL);
        QTextCodec* gbk = QTextCodec::codecForName("GBK");
        QTextCodec* utf8 = QTextCodec::codecForName("UTF-8");
        QString result = QString::fromUtf8(utf8->fromUnicode(gbk->toUnicode(recv)));
        QRegularExpression dateExp("[0-9]{4}\\-[0-9]{2}\\-[0-9]{2}");
        int index = result.indexOf(dateExp);

        if(index >= 0)
        {
            QDate now = QDate::fromString(result.mid(index, 10), "yyyy-MM-dd");
            if(now != cur)
            {
                ShareDate::setCurWorkDate(now);
                emit signalNewWorkDateNow();
                cur = now;
            }
        }
        QThread::sleep(60);
    }


}
