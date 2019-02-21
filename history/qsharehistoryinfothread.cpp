#include "qsharehistoryinfothread.h"
#include <QDateTime>
#include <QDebug>
#include "utils/qhttpget.h"
#include "data_structure/hqutils.h"
#include "dbservices/dbservices.h"
#include "dbservices/qactivedate.h"
#include "utils/comdatadefines.h"

QShareHistoryInfoThread::QShareHistoryInfoThread(const QString& code, const QMap<qint64, qint64>& foreign_map, QObject* parent) :
    mCode(code),
    mForeignMap(foreign_map),
    mParent(parent),
    QRunnable()
{
    mCode = code;
    if(mCode.length() > 6)
    {
        mCode = mCode.right(6);
    }
    mFileName = QString("%1%2").arg(HQ_SHARE_HISTORY_DIR).arg(mCode);
}

QShareHistoryInfoThread::~QShareHistoryInfoThread()
{

}

QDate QShareHistoryInfoThread::lastUpdateDate()
{
    QDate date = QDate::currentDate().addYears(-1);
    if(!QFile::exists(mFileName))
    {
        qDebug()<<__func__<<__LINE__<<mCode<<"file not exist!";
        return date;
    }
    //读取文件
    QFile file(mFileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<__func__<<__LINE__<<mCode<<"file not exist!";
        return date;
    }
    int size = file.size();
    int totalNum = 0;
    if(size >= sizeof(ShareData))
    {
        if(file.seek(size - sizeof(ShareData)))
        {
            //读取最后一个的日期
            ShareData info;
            file.read((char*)(&info), sizeof(ShareData));
            //date = QDate::fromJulianDay(info.date);
        } else
        {
        }
    }
    file.close();
    return date;
}

bool QShareHistoryInfoThread::write(const ShareDataList &list)
{
    FILE *fp = fopen(mFileName.toStdString().data(), "ab+");
    if(fp)
    {
        for(int i=list.size()-1; i>=0; i--)
        {
            fwrite(&(list[i]), sizeof(ShareData), 1, fp);
        }
        fclose(fp);
    }
}


void QShareHistoryInfoThread::run()
{
    //基金不更新
    if(mCode.left(1) == "5" || mCode.left(1) == "1") return;
    QDate start = QActiveDateTime(lastUpdateDate()).nextActiveDay();
    QDate end = QActiveDateTime::latestActiveDay();
    ShareDataList list;
    if(start <= end)
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
                .arg(wkCode).arg(start.toString("yyyyMMdd")).arg(end.toString("yyyyMMdd"));
        QByteArray recv = QHttpGet::getContentOfURL(wkURL);
        QString result = QString::fromLocal8Bit(recv);

        QStringList lines = result.split("\r\n");
        int index = 0;
        for(int i=1; i<lines.length(); i++)
        {
            QStringList cols = lines[i].split(",");
            if(cols.length() >= 15)
            {
                QDate curDate = QDate::fromString(cols[0], "yyyy-MM-dd");
                if(!QActiveDateTime(curDate).isDayActive()) continue;
                if(cols[3].toDouble() < 0.001) continue;
                ShareData data;
//                data.date = curDate.toJulianDay();
//                data.close = cols[3].toDouble();
//                data.change = cols[9].toDouble();
//                data.total_share = round(cols[13].toDouble() / data.close);
//                data.mutal_share = round(cols[14].toDouble() / data.close);
//                data.foreign_vol = mForeignMap[data.date];
//                data.money = cols[12].toDouble();
//                data.vol = cols[11].toDouble();
                list.append(data);
            }
        }
    }
    if(list.size() > 0)
    {
        //写入文件
        write(list);
    }

FUNC_END:
    if(mParent)
    {
        QMetaObject::invokeMethod(mParent,\
                                  "slotUpdateShareHistoryProcess",\
                                  Qt::DirectConnection, Q_ARG(ShareDataList,list ));
    }
    return;
}

QString QShareHistoryInfoThread::getCode()
{
    return mCode;
}



