#include "qsharehistoryinfothread.h"
#include <QDateTime>
#include <QDebug>
#include "utils/qhttpget.h"
#include "data_structure/hqutils.h"
#include "dbservices/dbservices.h"
#include "dbservices/qactivedate.h"
#include "utils/comdatadefines.h"

QShareHistoryInfoThread::QShareHistoryInfoThread(const QString& code, const ShareDate& start, const ShareDate& end, QObject* parent) :
    mCode(code),
    mStart(start),
    mEnd(end),
    mParent(parent),
    QRunnable()
{
    mCode = code;
    if(mCode.length() > 6)
    {
        mCode = mCode.right(6);
    }
}

QShareHistoryInfoThread::~QShareHistoryInfoThread()
{

}

void QShareHistoryInfoThread::run()
{
    //基金不更新
    if(mCode.left(1) == "5" || mCode.left(1) == "1") return;
    ShareForignVolFileDataList list;
    if(mStart <= mEnd)
    {
        QString wkCode;
        if(mCode.left(1) == "6" || mCode.left(1) == "5")
        {
            wkCode = "0" + mCode;
        } else
        {
            wkCode = "1" + mCode;
        }
        //取得日线数据
        QString wkURL = QString("http://quotes.money.163.com/service/chddata.html?code=%1&start=%2&end=%3")
                .arg(wkCode).arg(mStart.date().toString("yyyyMMdd")).arg(mEnd.date().toString("yyyyMMdd"));
        QByteArray recv = QHttpGet::getContentOfURL(wkURL);
        QTextCodec* gbk = QTextCodec::codecForName("GBK");
        QTextCodec* utf8 = QTextCodec::codecForName("UTF-8");
        QString result = QString::fromUtf8(utf8->fromUnicode(gbk->toUnicode(recv)));

        QStringList lines = result.split("\r\n");
        int index = 0;
        for(int i=1; i<lines.length(); i++)
        {
            QStringList cols = lines[i].split(",");
            if(cols.length() >= 15)
            {
                QDate curDate = QDate::fromString(cols[0], "yyyy-MM-dd");
                if(curDate.dayOfWeek() == 6 || curDate.dayOfWeek() == 7) continue;
                if(cols[3].toDouble() < 0.001) continue;
                ShareHistoryFileData data;
                data.mDate = QDateTime(curDate).toTime_t();
                data.mClose = cols[3].toDouble();
                data.mCloseAdjust = data.mClose;
                data.mMoney = cols[12].toDouble();
                list.append(data);
            }
        }
    }
FUNC_END:
    if(mParent)
    {
        QMetaObject::invokeMethod(mParent,\
                                  "slotUpdateShareHistoryProcess",\
                                  Qt::DirectConnection, Q_ARG(ShareForignVolFileDataList, list ));
    }
    return;
}

QString QShareHistoryInfoThread::getCode()
{
    return mCode;
}



