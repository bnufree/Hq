#include "qsharehistoryinfothread.h"
#include <QDateTime>
#include <QDebug>
#include "utils/qhttpget.h"
#include "data_structure/hqutils.h"
#include "dbservices/dbservices.h"
#include "dbservices/qactivedate.h"
#include "utils/comdatadefines.h"

QShareHistoryInfoThread::QShareHistoryInfoThread(const QString& code, const ShareWorkingDate& start, const ShareWorkingDate& end, QObject* parent) :
    mCode(code),
    mStart(start),
    mEnd(end),
    mParent(parent),
    mExistForeignMap(0),
    QRunnable()
{
    mCode = code;
    if(mCode.length() > 6)
    {
        mCode = mCode.right(6);
    }
}

QShareHistoryInfoThread::QShareHistoryInfoThread(const QString &code, QMap<QDate, ShareForignVolFileDataList>* map, QObject *parent)
    : mCode(code)
    , mParent(parent)
    , mStart(ShareWorkingDate())
    , mEnd(ShareWorkingDate())
    , mExistForeignMap(map)
    , QRunnable()
{

}

QShareHistoryInfoThread::~QShareHistoryInfoThread()
{

}

QString QShareHistoryInfoThread::getFileName()
{
    //设定保存的文件名
    return QString("%1/%2").arg(HQ_DAY_HISTORY_DIR).arg(mCode);
}

bool QShareHistoryInfoThread::readFile(ShareHistoryFileDataList& list)
{
    QString fileName = getFileName();
    if(!QFile::exists(fileName))
    {
        qDebug()<<__FUNCTION__<<__LINE__<<fileName<<" not exist";
        return false;
    }
    //读取文件
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<__FUNCTION__<<__LINE__<<fileName<<" open error";
        return false;
    }
    while (!file.atEnd() ) {
        ShareHistoryFileData data;
        file.read((char*)(&data), sizeof(ShareHistoryFileData));
        list.append(data);
    }
    file.close();

    return true;
}

void QShareHistoryInfoThread::run()
{
    //基金不更新
    if(mCode.left(1) == "5" || mCode.left(1) == "1") return;
    //默认上一次更新日期是一年前的第一天
    ShareWorkingDate last_update_date(ShareWorkingDate::getHisWorkingDay().last());
    //获取当前文件已经更新的日期
    ShareHistoryFileDataList list;
    readFile(list);
    if(list.size() > 0)
    {
        last_update_date.setDate(QDateTime::fromTime_t(list.last().mDate).date().addDays(1));
    }
    //开始更新日线数据到今天
    int new_size = 0;
    bool adjust = false;
    if(last_update_date < ShareWorkingDate::currentDate())
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
                .arg(wkCode).arg(last_update_date.date().toString("yyyyMMdd")).arg(ShareWorkingDate::currentDate().date().toString("yyyyMMdd"));
        QByteArray recv = QHttpGet::getContentOfURL(wkURL);
        QTextCodec* gbk = QTextCodec::codecForName("GBK");
        QTextCodec* utf8 = QTextCodec::codecForName("UTF-8");
        QString result = QString::fromUtf8(utf8->fromUnicode(gbk->toUnicode(recv)));

        QStringList lines = result.split("\r\n");
        int size = lines.length();
        for(int i=size-1; i>0; i--)
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
                data.mLastClose = cols[7].toDouble();
                data.mCloseAdjust = data.mClose;
                data.mMoney = cols[12].toDouble();
                data.mTotalShareCount = qint64(floor(cols[13].toDouble() / data.mClose));

//                if(curDate == QDate(2019,7,29))
//                {
//                    qDebug()<<__LINE__<<mCode<<QDateTime::fromTime_t(data.mDate).toString("yyyyMMdd")<<data.mForeignVol<<data.mForeignMututablePercent;
//                }
                //获取当日对应的陆股通数据
                if(mExistForeignMap && mExistForeignMap->contains(curDate))
                {
                    ShareForignVolFileDataList wklist = mExistForeignMap->value(curDate);
                    int index = wklist.indexOf(ShareForignVolFileData(mCode.toUInt()));
                    if(index >= 0)
                    {
                        ShareForignVolFileData temp = wklist[index];
                        data.mForeignVol = temp.mForeignVol;
                        data.mForeignMututablePercent = temp.mMutuablePercent;
//                        if(curDate == QDate(2019,7,29))
//                        {
//                            qDebug()<<__LINE__<<mCode<<"index:"<<index<<temp;
//                        }

                    }

                }
//                if(curDate == QDate(2019,7,29))
//                {
//                    qDebug()<<__LINE__<<mCode<<QDateTime::fromTime_t(data.mDate).toString("yyyyMMdd")<<data.mForeignVol<<data.mForeignMututablePercent;
//                }

                //检查是否有除权处理,如果有就更新前面所有的除权价格
                if(list.size() > 0 && list.last().mClose != data.mLastClose)
                {
                    //前一笔的最后价格和当前获取的前一次的最后价格不相同,那么今天就是进行了除权处理,对前面的所有价格都进行除权
                    double adjust_price = data.mLastClose - list.last().mClose;
                    if(adjust == false) adjust = true;
                    //检查股本是否发生了变化
                    double share_ratio = 1;
                    if(list.last().mTotalShareCount > 0 ) share_ratio = data.mTotalShareCount * 1.0 / list.last().mTotalShareCount;
                    adjustDataList(list, adjust_price, share_ratio);
                }
                if(list.size() > 0 && data.mForeignVol == 0 && list.last().mForeignVol != 0)
                {
                    data.mForeignVol = list.last().mForeignVol;
                    data.mForeignMututablePercent = list.last().mForeignMututablePercent;
                }
//                if(curDate == QDate(2019,7,29))
//                {
//                    qDebug()<<__LINE__<<mCode<<QDateTime::fromTime_t(data.mDate).toString("yyyyMMdd")<<data.mForeignVol<<data.mForeignMututablePercent;
//                }
                list.append(data);
                new_size++;
            }
        }
    }
    //对新的数据更新陆股痛数据处理
    if(adjust)
    {
        //删除文件,重新写入
        writeFile(list, "wb+");
    } else
    {
        //没有除权,只添加本次新获取的部分
        if(new_size > 0)
        {
            writeFile(list.mid(list.size() - new_size , new_size), "ab+");
        }
    }
    //最后更新到管理类,便于开始统计
//    if(list.size() > 0)
//    {
//        qDebug()<<__LINE__<<mCode<<QDateTime::fromTime_t(list.last().mDate).toString("yyyyMMdd")<<list.last().mForeignVol<<list.last().mForeignMututablePercent;
//    }
FUNC_END:
    if(mParent)
    {
        QMetaObject::invokeMethod(mParent,\
                                  "slotUpdateShareHistoryProcess",\
                                  Qt::DirectConnection,
                                  Q_ARG(ShareHistoryFileDataList, list),
                                  Q_ARG(QString, mCode)
                                  );
    }
    return;
}

QString QShareHistoryInfoThread::getCode()
{
    return mCode;
}



void QShareHistoryInfoThread::writeFile(const ShareHistoryFileDataList& list, const char* mode)
{
    if(list.size() == 0) return;
    FILE *fp = fopen(getFileName().toStdString().data(), /*"ab+"*/mode);
    if(fp)
    {
        foreach (ShareHistoryFileData data, list) {
            fwrite(&data, sizeof(ShareHistoryFileData), 1, fp);
        }
        fclose(fp);
    }
}

void QShareHistoryInfoThread::adjustDataList(ShareHistoryFileDataList &list, double price, double ratio)
{
    for(int i=0; i<list.size(); i++)
    {
        ShareHistoryFileData &data = list[i];
        data.mCloseAdjust += price;
        data.mLastClose += price;
        data.mForeignVol = qint64(floor(data.mForeignVol * ratio));
    }
}



