#include "qshareactivedateupdatethread.h"
#include "utils/qhttpget.h"
#include "dbservices/dbservices.h"
#include "utils/hqinfoparseutil.h"
#include "utils/profiles.h"
#include "annualcloseddaydisclosure.h"
#include <QEventLoop>
#include <QTimer>

QShareActiveDateUpdateThread::QShareActiveDateUpdateThread(QObject *parent) : QThread(parent)
{
}

QList<QDate> QShareActiveDateUpdateThread::getDateListFromNetease()
{
    //首先获取当前的日期列表
    ShareWorkingDate cur = ShareWorkingDate::currentDate();
    ShareWorkingDate start(cur.date().addYears(-1));
    QString wkURL = QString("http://quotes.money.163.com/service/chddata.html?code=0000001&start=%1&end=%2")
            .arg(start.toString("yyyyMMdd")).arg(cur.toString("yyyyMMdd"));
    QByteArray recv = QHttpGet::getContentOfURL(wkURL);
    QTextCodec* gbk = QTextCodec::codecForName("GBK");
    QTextCodec* utf8 = QTextCodec::codecForName("UTF-8");
    QString result = QString::fromUtf8(utf8->fromUnicode(gbk->toUnicode(recv)));

    QStringList lines = result.split("\r\n");
    QList<QDate> list;
    for(int i=1; i<lines.length(); i++)
    {
        QStringList cols = lines[i].split(",");
        if(cols.length() >= 15)
        {
            //if(mCode == "000400")qDebug()<<recv.mid(20, 200);
            ShareWorkingDate curDate = ShareWorkingDate::fromString(cols[0]);
            if(curDate.isWeekend()) continue;
            list.append(curDate.date());
        }
    }

    return list;
}

QList<QDate> QShareActiveDateUpdateThread::getDateListFromHexun()
{
    //首先获取当前的日期列表
    ShareWorkingDate cur = ShareWorkingDate::currentDate();
    ShareWorkingDate start(cur.date().addYears(-1));
    QString wkURL = "http://webstock.quote.hermes.hexun.com/a/kline?code=sse000001&start=20191011000000&number=-252&type=5&callback=callback";
    QByteArray recv = QHttpGet::getContentOfURL(wkURL);
    QTextCodec* gbk = QTextCodec::codecForName("GBK");
    QTextCodec* utf8 = QTextCodec::codecForName("UTF-8");
    QString result = QString::fromUtf8(utf8->fromUnicode(gbk->toUnicode(recv)));

    QStringList lines = result.split("\r\n");
    QList<QDate> list;
    for(int i=1; i<lines.length(); i++)
    {
        QStringList cols = lines[i].split(",");
        if(cols.length() >= 15)
        {
            //if(mCode == "000400")qDebug()<<recv.mid(20, 200);
            ShareWorkingDate curDate = ShareWorkingDate::fromString(cols[0]);
            if(curDate.isWeekend()) continue;
            list.append(curDate.date());
        }
    }

    return list;
}

QList<QDate> QShareActiveDateUpdateThread::getCurrentYearClosedDateList()
{
    QDateList list;
    while (true) {
        AnnualClosedDayDisclosure *closure = new AnnualClosedDayDisclosure(false);
        QEventLoop subloop;
        connect(closure, SIGNAL(finished()), &subloop, SLOT(quit()));
        QTimer timer;
        timer.setSingleShot(true);
        connect(&timer, SIGNAL(timeout()), &subloop, SLOT(quit()));
        timer.start(10*1000);
        closure->start();
        subloop.exec();
        if(timer.isActive()) timer.stop();
        list = closure->getClosedDateList();
        closure->deleteLater();
        if(list.size() > 0)
        {
            break;
        }

        sleep(3);
    }

    return list;
}

void QShareActiveDateUpdateThread::run()
{
    QDate workDate;
    //检查当前时间是不是工作日
    QDateList closedDateList;
    while(true)
    {
        //获取当前年度对应的节假日休市
        if(closedDateList.size() == 0 || closedDateList.first().year() != QDate::currentDate().year())
        {
            closedDateList.clear();
            closedDateList = getCurrentYearClosedDateList();
            if(closedDateList.size() == 0) continue;
            PROFILES_INS->setValue(CLOSE_DATE_SEC, CLOSE_DATE_KEY, HqInfoParseUtil::dateListToStringList(closedDateList));
        }


        QTime t;
        t.start();
        QString wkURL = QString("http://hq.sinajs.cn/list=sh000001");
        QByteArray recv = QHttpGet::getContentOfURL(wkURL);
        QTextCodec* gbk = QTextCodec::codecForName("GBK");
        QTextCodec* utf8 = QTextCodec::codecForName("UTF-8");
        QString result = QString::fromUtf8(utf8->fromUnicode(gbk->toUnicode(recv)));
        QRegularExpression dateExp("[0-9]{4}\\-[0-9]{2}\\-[0-9]{2}");
        int index = result.indexOf(dateExp);
        bool code_change = false;
        if(index >= 0)
        {
            QDateTime nowDateTime = QDateTime::fromString(result.mid(index, 19), "yyyy-MM-dd,hh:mm:ss");
            QDate now = nowDateTime.date();
            if(now!= workDate)
            {
                workDate = now;
                ShareWorkingDate::setCurWorkDate(now);
                //新的日期开始了,开始更新历史日期
                QList<QDate> list = HqInfoParseUtil::getActiveDateListOfLatestYearPeriod();
                if(list.size() > 0)
                {
                    ShareWorkingDate::setHisWorkingDay(list);
                }
                code_change = true;
            }

            //检查当前的时间,如果是9:00以后,需要开始重新获取代码
            QTime   time = QDateTime::currentDateTime().time();
            QDate   date = QDateTime::currentDateTime().date();
            int status = DATA_SERVICE->getSystemStatus();
            if(date <= workDate)
            {
                QString nowStr = time.toString("hhmmss");
                if(nowStr >= "150000")
                {
                    status = HQ_Closed;
                } else if(nowStr >= "091000")
                {
                    if(status != HQ_InCharge)
                    {
                        status = HQ_InCharge;
                        code_change = true;
                    }
                } else
                {
                    status = HQ_NotOpen;
                }

            }
            DATA_SERVICE->setSystemStatus(status);
            if(status == HQ_InCharge)
            {
                nowDateTime = QDateTime::currentDateTime();
            }
            emit signalSystemStatus(nowDateTime.toMSecsSinceEpoch(), status);
            if(code_change)
            {
                emit signalNewWorkDateNow();
            }
        }



        //每分钟运行
        QThread::msleep(500);
    }


}
