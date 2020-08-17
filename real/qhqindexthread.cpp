#include "qhqindexthread.h"
#include "utils/qhttpget.h"
#include "utils/profiles.h"
#include "utils/hqinfoparseutil.h"

QHqIndexThread::QHqIndexThread(QObject *parent) : QThread(parent)
{
    //首先设定默认的值
    QStringList default_index_list;
    default_index_list<<"sh000001"<<"sh000300"<<"sz399001"<<"sz399006"<<"sh000016"<<"sz399673"<<"rt_hkHSI";
    PROFILES_INS->setDefault(GLOBAL_SETTING, INDEX_KEY, default_index_list);

}

ShareDataList QHqIndexThread::getDataList()
{
    QMutexLocker locker(&mMutex);
    return mDataList;
}

void QHqIndexThread::run()
{
    QStringList index_list = PROFILES_INS->value(GLOBAL_SETTING, INDEX_KEY, QStringList()).toStringList();
    if(index_list.size() == 0) return;
    //开始更新
    QString url("http://hq.sinajs.cn/?list=%1");
    QString wkURL = url.arg(index_list.join(","));
    while (1) {
        QString result = QString::fromUtf8(QHttpGet::getContentOfURL(wkURL));
        //先换行
        QStringList resultlist = result.split(QRegExp("[\\n;]"), QString::SkipEmptyParts);
        ShareDataList datalist;
        foreach (QString detail, resultlist)
        {
            //var hq_str_sz399006="创业板指,1647.848,1692.416,1680.387,1718.384,1635.450,0.000,0.000,10414641478,126326323478.810
            detail.replace("var hq_str_", "");
            ShareData data;
            if(detail.startsWith("rt_hk"))
            {
                if(HqInfoParseUtil::parseShareDataFromSinaHK(data, detail)) datalist.append(data);
            } else
            {
                if(HqInfoParseUtil::parseShareDataFromSinaA(data, detail)) datalist.append(data);
            }
        }
        emit signalSendIndexDataList(datalist, QDateTime::currentMSecsSinceEpoch());
        {
            QMutexLocker locker(&mMutex);
            mDataList = datalist;
        }
        QThread::sleep(2);
    }

}
