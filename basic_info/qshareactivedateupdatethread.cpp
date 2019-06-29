#include "qshareactivedateupdatethread.h"
#include "utils/qhttpget.h"
#include "dbservices/dbservices.h"

QShareActiveDateUpdateThread::QShareActiveDateUpdateThread(QObject *parent) : QThread(parent)
{

}

void QShareActiveDateUpdateThread::run()
{
    QList<QDate> daylist = ShareDate::getUnWorkingDay();
    ShareDate cur = ShareDate::currentDate();
    ShareDate start(cur.date().addYears(-1));
    if(list.size() > 0)
    {
        start.setDate(daylist.first().addDays(1));
        if (start.isWeekend()) start = start.nextWorkDay();
    }
    // fetch main shanghai index day data from server
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
    for(int i=0; i<list.size(); i++)
    {
        if
    }
}
