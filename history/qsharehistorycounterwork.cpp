#include "qsharehistorycounterwork.h"
#include "dbservices/dbservices.h"
#include "dbservices/qactivedate.h"

QShareHistoryCounterWork::QShareHistoryCounterWork(const QString& code,const ShareHistoryList& list, QObject* parent)
    :mCode(code),mList(list),mParent(parent),QRunnable()
{

}

QShareHistoryCounterWork::~QShareHistoryCounterWork()
{

}

void QShareHistoryCounterWork::run()
{
    if(mList.size() == 0)
    {
        //从文件获取
        if(mCode.length() > 6)
        {
            mCode = mCode.right(6);
        }
        QString fileName = QString("%1%2").arg(HQ_SHARE_HISTORY_DIR).arg(mCode);
        //读取文件信息
        if(!QFile::exists(fileName)) return ;
        //读取文件
        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly)) return ;
        int size = file.size();
        int totalNum = 0;
        if(size >= sizeof(SHARE_HISTORY_INFO))
        {
            while(!file.atEnd())
            {
                SHARE_HISTORY_INFO info;
                file.read((char*)(&info), sizeof(SHARE_HISTORY_INFO));
                mList.append(info);
            }
        }
        file.close();

    }
 //   qSort(mList.begin(), mList.end(), ShareData::sortByDateAsc);    
    int size = mList.size();
    double lastColse = 0;
    double lastMoney = 0.0;
    double last3Change = 0.0;
    double last5Change = 0.0;
    double last10Change = 0.0;
    double lastMonthChange = 0.0;
    double last1HYearChange = 0.0;
    qint64 vol_change = 0;
    qint64 vol = 0;

    if(size > 0)
    {
        lastMoney = mList[size-1].money;
        lastColse = mList[size-1].close;
        if(QDate::fromJulianDay(mList[size-1].date) == QActiveDateTime::latestActiveDay())
        {
            if(size - 2 > 0)
            {
                lastMoney = mList[size-2].money;
                lastColse = mList[size-2].close;
            }
        }
        int day[] = {DAYS_3, DAYS_5, DAYS_10, DAYS_MONTH, DAYS_HALF_YEAR};
        double *change[] = {&last3Change, &last5Change, &last10Change, &lastMonthChange, &last1HYearChange};
        int k = 0;
        double changper = 1.0;
        for(int i=0; i<5; i++)
        {
            for(; k<size; k++)
            {

                if(k < day[i])
                {
                    SHARE_HISTORY_INFO curData = mList[size-1-k];
                    changper *= (1 + curData.change /100);
                    for(int j=i; j<5; j++)
                    {
                        *(change[j]) = (changper -1) * 100;
                    }
                } else
                {
                    break;
                }

            }
        }

        vol = mList[size-1].foreign_vol;
        vol_change = vol;
        if(size >=2)
        {
            vol_change -= mList[size-2].foreign_vol;
        }
    }
    DATA_SERVICE->signalUpdateShareinfoWithHistory(mCode, lastMoney, last3Change, last5Change, last10Change, lastMonthChange, last1HYearChange, vol, vol_change, mList);

//    qDebug()<<mCode<<lastMoney<<last3Change<<last5Change<<last10Change<<lastMonthChange<<last1HYearChange<<vol<<vol_change;

    if(mParent)
    {
        QMetaObject::invokeMethod(mParent,\
                                  "slotUpdateShareHistoryInfoFinished",\
                                  Q_ARG(QString, mCode));
    }
}
