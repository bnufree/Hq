#include "qsharehistorycounterwork.h"
#include "dbservices/dbservices.h"

QShareHistoryCounterWork::QShareHistoryCounterWork(const QString& code,const ShareDataList& list, QObject* parent)
    :mCode(code),mList(list),mParent(parent),QRunnable()
{

}

QShareHistoryCounterWork::~QShareHistoryCounterWork()
{

}

void QShareHistoryCounterWork::run()
{
    qSort(mList.begin(), mList.end(), ShareData::sortByDateDesc);
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
        lastMoney = mList[0].mMoney;
        lastColse = mList[0].mClose;
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
                    ShareData curData = mList[k];
                    changper *= (1 + curData.mChgPercent /100);
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

        vol = mList[0].mForeignVol;
        vol_change = vol;
        if(size >=2)
        {
            vol_change -= mList[1].mForeignVol;
        }
    }
    DATA_SERVICE->signalUpdateShareinfoWithHistory(mCode, lastMoney, last3Change, last5Change, last10Change, lastMonthChange, last1HYearChange, vol, vol_change);

//    qDebug()<<mCode<<lastMoney<<last3Change<<last5Change<<last10Change<<lastMonthChange<<last1HYearChange<<vol<<vol_change;

    if(mParent)
    {
        QMetaObject::invokeMethod(mParent,\
                                  "slotUpdateShareHistoryInfoFinished",\
                                  Q_ARG(QString, mCode));
    }
}
