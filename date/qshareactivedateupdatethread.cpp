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
    QDate cur = QDate::currentDate();
    QDate start(cur.addYears(-1));
    QString wkURL = QString("http://quotes.money.163.com/service/chddata.html?code=0000001&start=%1&end=%2")
            .arg(start.toString("yyyyMMdd")).arg(cur.toString("yyyyMMdd"));
    QByteArray recv = QHttpGet::getContentOfURL(wkURL);
    QString result = QString::fromUtf8(recv);

    QStringList lines = result.split("\r\n");
    QList<QDate> list;
    for(int i=1; i<lines.length(); i++)
    {
        QStringList cols = lines[i].split(",");
        if(cols.length() >= 15)
        {
            //if(mCode == "000400")qDebug()<<recv.mid(20, 200);
            QDate curDate = QDate::fromString(cols[0]);
            list.append(curDate);
        }
    }

    return list;
}

QList<QDate> QShareActiveDateUpdateThread::getDateListFromHexun()
{
    //首先获取当前的日期列表
    QDate cur = QDate::currentDate();
    QDate start(cur.addYears(-1));
    QString wkURL = "http://webstock.quote.hermes.hexun.com/a/kline?code=sse000001&start=20191011000000&number=-252&type=5&callback=callback";
    QByteArray recv = QHttpGet::getContentOfURL(wkURL);
    QString result = QString::fromUtf8(recv);

    QStringList lines = result.split("\r\n");
    QList<QDate> list;
    for(int i=1; i<lines.length(); i++)
    {
        QStringList cols = lines[i].split(",");
        if(cols.length() >= 15)
        {
            //if(mCode == "000400")qDebug()<<recv.mid(20, 200);
            QDate curDate = QDate::fromString(cols[0]);
            list.append(curDate);
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
    while(true)
    {
        //获取当前行情系统的时间
        QByteArray recv = QHttpGet::getContentOfURL("http://hq.sinajs.cn/list=sh000001");
        QString result = QString::fromUtf8(recv);
        QRegularExpression dateExp("[0-9]{4}\\-[0-9]{2}\\-[0-9]{2}");
        int index = result.indexOf(dateExp);
        bool code_change = false;
        bool update_hs_top10 = false;
        if(index >= 0)
        {
            QDateTime hqDateTime = QDateTime::fromString(result.mid(index, 19), "yyyy-MM-dd,hh:mm:ss");
            QDate now = hqDateTime.date();
            if(now != workDate)
            {
                //行情系统时间进行切换到最新的时间
                workDate = now;
                //新的日期开始了,开始更新历史日期
                QList<QDate> list;
                while (1) {
                    list = HqInfoParseUtil::getActiveDateListOfLatestYearPeriod();
                    if(list.size() > 0)
                    {
//                        if(list.contains(workDate)) list.removeOne(workDate);
                        TradeDateMgr::instance()->setHistoryTradeDays(list);
                        QDate previous_day = ShareTradeDateTime(now).previousTradeDay();
                        TradeDateMgr::instance()->setCurrentTradeDay(now);
                        TradeDateMgr::instance()->setLastTradeDay(previous_day);
                        code_change = true;
                        update_hs_top10 = false;
                        break;
                    }
                }
            }
            //更新系统的交易状态
            QTime   time = QDateTime::currentDateTime().time();
            QDate   date = QDateTime::currentDateTime().date();
            int status = DATA_SERVICE->getSystemStatus();
            if(date > TradeDateMgr::instance()->currentTradeDay())
            {
                status = HQ_NotOpen;
            } else
            {
                QString nowStr = time.toString("hhmmss");
                if(nowStr >= "150000")
                {
                    if(status != HQ_Closed)
                    {
                        status = HQ_Closed;
                        emit signalSaveTodayShareHistory();
                    }
                    if(nowStr >= "173000" && !update_hs_top10)
                    {
                        emit signalUpdateHsgtTop10();
                        update_hs_top10 = true;
                    }

                } else if(nowStr >= "091000")
                {
                    status = HQ_InCharge;
                } else
                {
                    status = HQ_NotOpen;
                }

            }
            DATA_SERVICE->setSystemStatus(status);
            emit signalSystemStatus(QDateTime::currentDateTime().toMSecsSinceEpoch(), status);
            if(code_change)
            {
                emit signalNewWorkDateNow();
            }
        }
        //每分钟运行
        QThread::sleep(3);
    }


}
