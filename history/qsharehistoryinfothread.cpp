#include "qsharehistoryinfothread.h"
#include <QDateTime>
#include <QDebug>
#include "utils/qhttpget.h"
#include "data_structure/hqutils.h"
#include "dbservices/dbservices.h"
#include "dbservices/qactivedate.h"
#include "utils/comdatadefines.h"
#include "utils/hqinfoparseutil.h"
#include "utils/profiles.h"

#define     UPDATE_SEC              "CodeInfo"

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

bool QShareHistoryInfoThread::readFile(ShareHistoryFileDataList& list, bool& adjust)
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
        //检查当前的外资持股数据是否需要更新,这里主要是防止获取外资持股数据出现了遗漏
        qint64 vol = 0;
        double percent = 0;
        getForeignVolData(vol, percent, QDateTime::fromTime_t(data.mDate).date());
        if(vol > 0 && data.mForeignVol != vol)
        {
            data.mForeignVol = vol;
            data.mForeignMututablePercent = percent;
            adjust = true;
        }
//        if(mCode == "002057")
//        {
//            qDebug()<<QDateTime::fromTime_t(data.mDate).toString("yyyyMMdd")<<data.mClose<<data.mLastClose<<data.mCloseAdjust<<data.mForeignVolOri<<data.mForeignVolAdjust;
//        }

        list.append(data);
    }
    file.close();

    return true;
}

void QShareHistoryInfoThread::getForeignVolData(qint64 &vol, double &percent, const QDate &date)
{
    if(!mExistForeignMap) return;
    if(!mExistForeignMap->contains(date)) return;
    //获取当日对应的陆股通数据
    ShareForignVolFileDataList wklist = mExistForeignMap->value(date);
    int index = wklist.indexOf(ShareForignVolFileData(mCode.toUInt()));
    if(index < 0) return;
    ShareForignVolFileData temp = wklist[index];
    vol = temp.mForeignVol;
    percent = temp.mMutuablePercent;
}

void QShareHistoryInfoThread::run()
{
    QTime t;
    t.start();
    QDate start, end;
    //default is
    ShareWorkingDate start_update_date;
    bool adjust = false;
    ShareHistoryFileDataList list;
    readFile(list, adjust);
    if(list.size() > 0)
    {
        start_update_date.setDate(QDateTime::fromTime_t(list.last().mDate).date().addDays(1));
    } else
    {
        if(!HqInfoParseUtil::getShareDateRange(mCode.right(6), start, end)) return;
        start_update_date.setDate(start);
    }
    //开始更新日线数据到今天
    int new_size = 0;
    ShareHistoryFileDataList new_list;
    bool need_update = true;
    if(ShareWorkingDate::getCurWorkDay().date() == QDate::currentDate())
    {
        if(start_update_date == QDate::currentDate()) need_update = false;
    } else
    {
        if(start_update_date.date().addDays(-1) == ShareWorkingDate::getCurWorkDay().date()) need_update = false;
    }
//    qDebug()<<mCode<<" update:"<<need_update;
    if(need_update)
    {
//        new_list = HqInfoParseUtil::getShareHistoryDataFrom163(last_update_date.date(), mCode);
        if(new_list.size() == 0) new_list = HqInfoParseUtil::getShareHistoryData(start_update_date.date(), mCode);
    }
#if 0
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
                //获取当日对应的陆股通数据
                getForeignVolData(data.mForeignVolOri, data.mForeignMututablePercent, curDate);
                data.mForeignVolAdjust = data.mForeignVolOri;
                //检查是否有除权处理,如果有就更新前面所有的除权价格
                if(list.size() > 0 && list.last().mClose != data.mLastClose)
                {
                    //前一笔的最后价格和当前获取的前一次的最后价格不相同,那么今天就是进行了除权处理,对前面的所有价格都进行除权
#if 0
                    double adjust_price = data.mLastClose - list.last().mClose;
#else
                    double adjust_price = data.mLastClose / list.last().mClose;
#endif
                    if(adjust == false) adjust = true;
                    //检查股本是否发生了变化
                    double share_ratio = 1;
                    if(list.last().mTotalShareCount > 0 ) share_ratio = data.mTotalShareCount * 1.0 / list.last().mTotalShareCount;
                    adjustDataList(list, adjust_price, share_ratio);
                }
                if(list.size() > 0 && data.mForeignVolOri == 0 && list.last().mForeignVolOri != 0)
                {
                    data.mForeignVolOri = list.last().mForeignVolOri;
                    data.mForeignMututablePercent = list.last().mForeignMututablePercent;
                }
                list.append(data);
                new_size++;
            }
        }
    }
#else
    for(int i=0; i<new_list.size(); i++)
    {
        ShareHistoryFileData data = new_list[i];
        data.mCloseAdjust = data.mClose;
        //获取当日对应的陆股通数据
        getForeignVolData(data.mForeignVol, data.mForeignMututablePercent, QDateTime::fromTime_t(data.mDate).date());
        //检查是否有除权处理,如果有就更新前面所有的除权价格
        if(list.size() > 0 && list.last().mClose != data.mLastClose)
        {
            //前一笔的最后价格和当前获取的前一次的最后价格不相同,那么今天就是进行了除权处理,对前面的所有价格都进行除权
            double adjust_price = data.mLastClose / list.last().mClose;
            if(adjust == false) adjust = true;
            //检查股本是否发生了变化
            double share_ratio = 1;
            adjustDataList(list, adjust_price, share_ratio);
        }
//        if(list.size() > 0 && data.mForeignVolOri == 0 && list.last().mForeignVolOri != 0)
//        {
//            data.mForeignVolOri = list.last().mForeignVolOri;
//            data.mForeignMututablePercent = list.last().mForeignMututablePercent;
//        }
        list.append(data);
        new_size++;
    }

#endif
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
//    qDebug()<<mCode<<" history size:"<<list.size();
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
//    qDebug()<<"update history "<<mCode<< t.elapsed();
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
#if 0
        data.mCloseAdjust += price;
        data.mLastClose += price;
#else
        data.mCloseAdjust *= price;
        data.mLastClose *= price;
#endif
//        data.mForeignVolAdjust = qint64(floor(data.mForeignVolAdjust * ratio));
    }
}



