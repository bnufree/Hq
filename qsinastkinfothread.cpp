#include "qsinastkinfothread.h"
#include <QDebug>
#include "profiles.h"
#include <QDateTime>
#include "stkmktcap.h"
#include "stkinfofilemanage.h"
#include "qexchangedatamanage.h"
#include "dbservices.h"
#include "qhttpget.h"


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
            data.xjfh = 0;
        } else
        {
            StockData basic_data = DATA_SERVICE->getBasicStkData(data.code);
            data.totalshare = basic_data.totalshare;
            data.mutableshare = basic_data.mutableshare;
            data.last_money = basic_data.last_money;
            data.last_3day_pers = basic_data.last_3day_pers;
            data.last_5day_pers = basic_data.last_5day_pers;
            data.last_10day_pers = basic_data.last_10day_pers;
            data.last_month_pers = basic_data.last_month_pers;
            data.gxl = 0;
            data.szzbl = 0;
            data.xjfh = 0;
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
    QString url("http://hq.sinajs.cn/?list=%1");
    while(true)
    {
        if(mStkList.length() == 0) continue;

        QString wkURL = url.arg(mStkList.join(","));
        //开始解析数据
        QByteArray bytes = QHttpGet::getContentOfURL(wkURL);
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
            if(mDataMap[code].cur == 0)
            {
                mDataMap[code].cur = mDataMap[code].last_close;
            }
            if(mDataMap[code].cur != 0)
            {
                mDataMap[code].gxl = mDataMap[code].xjfh / mDataMap[code].cur;
            }
            //if(data.cur == 0 ) continue;
            mDataMap[code].chg = mDataMap[code].cur - mDataMap[code].last_close;
            mDataMap[code].per = mDataMap[code].chg *100 / mDataMap[code].last_close;
            mDataMap[code].totalCap = mDataMap[code].cur * mDataMap[code].totalshare;
            mDataMap[code].mutalbleCap = mDataMap[code].cur * mDataMap[code].mutableshare;
            if(mDataMap[code].profit == 0)
            {
                mDataMap[code].profit = DATA_SERVICE->getProfit(code);
            }
        }
        emit sendStkDataList(mDataMap.values());
        QThread::sleep(1);
    }

}

void QSinaStkInfoThread::run()
{
    RealtimeInfo();
}

int QSinaStkInfoThread::getStkCount()
{
    return mStkList.count();
}


