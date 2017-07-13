#include "qsinastkinfothread.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>
#include <QEventLoop>
#include "profiles.h"
#include <QDateTime>
#include "stkmktcap.h"
#include "stkinfofilemanage.h"
#include "qexchangedatamanage.h"


QSinaStkInfoThread::QSinaStkInfoThread(QObject *parent) : QThread(parent)
{
    mOptType =STK_DISPLAY_SORT_TYPE_CHGPER;
    mSortRule = -1;
    mStkList.clear();
    //mActive = true;
}

QSinaStkInfoThread::~QSinaStkInfoThread()
{

}
bool QSinaStkInfoThread::isActive()
{
    //return mActive;
    return true;
}

void QSinaStkInfoThread::setOptType(STK_DISPLAY_TYPE type)
{
    if(mOptType == type)
    {
        reverseSortRule();
    } else
    {
        mOptType = type;
        mSortRule = -1;
    }
}

void QSinaStkInfoThread::setSortRule(STK_DISPLAY_RULE rule)
{
    if(rule == STK_DISPLAY_SORT_RULE_NONE) return;
    mSortRule = (rule == STK_DISPLAY_SORT_RULE_DESCEND? -1 : 1);
}

void QSinaStkInfoThread::reverseSortRule()
{
    mSortRule = (mSortRule == -1 ? 1: -1);
}

void QSinaStkInfoThread::setStkList(const QStringList &list)
{
    mStkList = list;
    //取得配置文件保存的昨天的数据
    mDataMap.clear();
    foreach (QString wkcode, mStkList) {
        StockData data;
        data.code = wkcode.right(6);
        if(data.code.left(1) == "5" || data.code.left(1) == "1")
        {
            data.totalshare = 0;
            data.mutableshare = 0;
            data.last_money = 0;
            data.totalCap = 0;
            data.mutalbleCap = 0;
            data.gxl = 0;
            data.szzbl = 0;
            data.gqdjr = "-";
            data.xjfh = 0;
        } else
        {
            StkInfoFileManage *filemgr = new StkInfoFileManage(wkcode.right(6));
            QStringList secs = filemgr->subkeys("Dates");
            if(secs.length())
            {
                //取得上次更新的时间
                secs.sort();
                QString dateStre = secs.last();
                QDate date = QDate::fromString(dateStre, "yyyy-MM-dd");
                if(date == QExchangeDataManage::instance()->GetLatestActiveDay(QDate::currentDate()))
                {
                    if(secs.length() > 1)
                    {
                        dateStre = secs.at(secs.length() -2);
                    } else
                    {
                        dateStre.clear();
                    }
                }
                QStringList content = filemgr->value("Dates", dateStre).toStringList();
                if(content.length() >2)
                {
                    data.totalshare = 0;
                    data.mutableshare = 0;
                    data.last_money = content[0].toDouble();
                    data.totalCap = content[1].toDouble();
                    data.mutalbleCap = content[2].toDouble();
                    if(data.code.contains("002352"))
                        qDebug()<<"content:"<<content<<" total:"<<data.totalCap<<" mutal:"<<data.mutalbleCap;

                } else
                {
                    data.totalshare = 0;
                    data.mutableshare = 0;
                    data.last_money = 0;
                    data.totalCap = 0;
                    data.mutalbleCap = 0;
                }
            }
            data.last_three_pers = filemgr->value("Chg", "L3").toDouble();
            data.last_five_pers = filemgr->value("Chg", "L5").toDouble();
            data.totalshare = filemgr->value("Chg", "total").toDouble();
            data.mutableshare = filemgr->value("Chg", "mutable").toDouble();
            data.blocklist = filemgr->value("Block", "names").toStringList();
            QStringList fhsp = filemgr->value("FHSP", "Content").toStringList();
            if(fhsp.length() > 4)
            {
                data.xjfh = fhsp.at(1).toDouble() / 10;
                data.gxl = fhsp.at(2).toDouble();
                data.szzbl = fhsp.at(0).toDouble();
                QString datestr = fhsp.at(3);
                data.yaggr = fhsp.at(4);
                if(datestr.length() > 10)
                {
                    data.gqdjr = datestr.left(10);
                } else
                {
                    data.gqdjr = "-";
                }
            } else
            {
                data.gxl = 0;
                data.szzbl = 0;
                data.gqdjr = "-";
                data.xjfh = 0;
            }
        }
        mDataMap[data.code] = data;
    }

   // qDebug()<<"mdataMap:"<<mDataMap.keys();

}

void QSinaStkInfoThread::setActive(bool act)
{
    //mActive = act;
}

void QSinaStkInfoThread::RealtimeInfo()
{
    QNetworkAccessManager *mgr = new QNetworkAccessManager;
    QString url("http://hq.sinajs.cn/?list=%1");
    while(true)
    {
        if(mStkList.length() == 0) continue;

        QString wkURL = url.arg(mStkList.join(","));
        QNetworkReply *reply  = mgr->get(QNetworkRequest(wkURL));
        if(!reply) continue;
        QEventLoop loop; // 使用事件循环使得网络通讯同步进行
        connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec(); // 进入事件循环， 直到reply的finished()信号发出， 这个语句才能退出
        if(reply->error())
        {
            qDebug()<<"err occured:"<<reply->errorString();
            reply->deleteLater();
            continue;
        }
        //开始解析数据
        QByteArray bytes = reply->readAll();
        QString result = QString::fromLocal8Bit(bytes.data());
        //先换行
        QStringList resultlist = result.split(QRegExp("[\\n|;]"), QString::SkipEmptyParts);
        //再分割具体的字段
        foreach (QString detail, resultlist) {
            detail.replace(QRegExp("([a-z]{1,} )"), "");
            QStringList detailList = detail.split(QRegExp("[a-z|\"|\"|,|=|_]"), QString::SkipEmptyParts);
            if(detailList.length() < 20) continue;
            QString code = detailList[0];
            mDataMap[code].name = detailList[1];
            mDataMap[code].open = detailList[2].toDouble();
            mDataMap[code].last_close = detailList[3].toDouble();
            mDataMap[code].cur = detailList[4].toDouble();

            mDataMap[code].cur = detailList[4].toDouble();
            mDataMap[code].high = detailList[5].toDouble();
            mDataMap[code].low = detailList[6].toDouble();
            mDataMap[code].buy = detailList[7].toDouble();
            mDataMap[code].sell = detailList[8].toDouble();
            mDataMap[code].vol = detailList[9].toInt();
            mDataMap[code].time = detailList[detailList.length()-2];
            qint64 total = mDataMap[code].mutableshare;
            if(mDataMap[code].cur != 0)
            {
                mDataMap[code].gxl = mDataMap[code].xjfh / mDataMap[code].cur;
            }
            if(total > 0){
                mDataMap[code].hsl = mDataMap[code].vol / (double)total * 100;
            } else {
                mDataMap[code].hsl = 0.00;
            }
            double lastmonry = mDataMap[code].last_money;
            if(lastmonry > 0){
                mDataMap[code].money_ratio = detailList[10].toDouble() / lastmonry;
            } else {
                mDataMap[code].money_ratio = 0.00;
            }
            //MktCapFile::instance()->setValue(sec, key, detailList[10].toDouble());
            mDataMap[code].money = detailList[10].toDouble() / 10000;
            int hour = QDateTime::currentDateTime().time().hour();
            int min = QDateTime::currentDateTime().time().minute();
            if( hour == 9){
                if(min>=15 && min<=25){
                    mDataMap[code].cur = detailList[12].toDouble();
                }
            }
            //if(data.cur == 0 ) continue;
            mDataMap[code].chg = mDataMap[code].cur - mDataMap[code].last_close;
            mDataMap[code].per = mDataMap[code].chg *100 / mDataMap[code].last_close;
            mDataMap[code].totalCap = mDataMap[code].cur * mDataMap[code].totalshare;
            mDataMap[code].mutalbleCap = mDataMap[code].cur * mDataMap[code].mutableshare;
        }
        emit sendStkDataList(mDataMap.values());
        reply->deleteLater();
        QThread::sleep(1);
    }

    mgr->deleteLater();
}

void QSinaStkInfoThread::run()
{
    RealtimeInfo();
}

int QSinaStkInfoThread::getStkCount()
{
    return mStkList.count();
}


