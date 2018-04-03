#include "qsharebasicinfoworker.h"
#include "qsharecodeswork.h"
#include "utils/hqutils.h"
#include "dbservices/hqdatadefines.h"
#include <QThreadPool>
#include "qsharecodeswork.h"
#include "qsharefhspwork.h"
#include "qsharehsgttop10work.h"
#include "qsharefinancialinfowork.h"
#include <QFile>

#define STOCK_CODE_FILE  "share.dat"
QShareBasicInfoWorker::QShareBasicInfoWorker(QObject *parent) : QObject(parent)
{
    mShareBaseDataMap.clear();
    connect(this, SIGNAL(signalGetBasicInfo()), this, SLOT(slotGetBasicInfo()));
    moveToThread(&mWorkThread);
    mWorkThread.start();
}

void QShareBasicInfoWorker::slotGetBasicInfo()
{
    //从文件获取信息
    if(!getInfosFromFile(mShareBaseDataMap))
    {
        getInfossFromWeb(mShareBaseDataMap);
        writeInfos(mShareBaseDataMap.values());
    }

    emit signalBaseDataListFinished(QStringList(mShareBaseDataMap.keys()), mShareBaseDataMap.values());

}

bool QShareBasicInfoWorker::getInfosFromFile(QMap<QString, ShareBaseData>& map)
{
    qDebug()<<__FUNCTION__<<__LINE__;
    map.clear();
    if(!QFile::exists(STOCK_CODE_FILE))
    {
        qDebug()<<__FUNCTION__<<__LINE__;
        qDebug()<<STOCK_CODE_FILE<<" does not exist!!";
        return false;
    }
    //读取文件
    QFile file(STOCK_CODE_FILE);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<__FUNCTION__<<__LINE__;
        return false;
    }
    int size = file.size();
    int totalNum = 0;
    if(size > sizeof(qint64) + sizeof(int))
    {
        qint64 lastupdate = 0;
        file.read((char*)(&lastupdate), sizeof(qint64));
        if(QDateTime::fromMSecsSinceEpoch(lastupdate).date() == HqUtils::latestActiveDay())
        {
            file.read((char*)(&totalNum), sizeof(int));
            int count = 0;
            while (!file.atEnd() ) {
                ShareBaseData data;
                file.read((char*)(&data), sizeof(ShareBaseData));
                map[QString::fromStdString(data.mCode)] = data;
                //qDebug()<<data.mCode<<data.mName<<data.mPY<<data.mTotalShare;
            }
        }
    }
    file.close();
    if(totalNum != 0 && map.count() == totalNum)
    {
        qDebug()<<__FUNCTION__<<__LINE__;
        return true;
    }
    qDebug()<<"read from file failed!!!!!!!!!!!!!!!!!!!!!!";

    return false;
}

bool QShareBasicInfoWorker::getInfossFromWeb(QMap<QString, ShareBaseData>& map)
{
    QThreadPool pool;
    pool.setMaxThreadCount(8);
    pool.setExpiryTimeout(-1);

    //取得代码
    pool.start(new QShareCodesWork(this));
    //取得分红送配
    pool.start(new QShareFHSPWork("2017-12-31", this));
    //取得北向交易TOP10
    pool.start(new QShareHsgtTop10Work(HqUtils::lastActiveDay().toString("yyyy-MM-dd"), this));
    pool.waitForDone();
    //取得财务信息
    QStringList allCodes(mShareBaseDataMap.keys());
    int pos = 0;
    int section = 200;
    while(pos < allCodes.length())
    {
        QStringList sublist = allCodes.mid(pos, section);
        pos += section;
        pool.start(new QShareFinancialInfoWork(sublist, this));
    }
    pool.waitForDone();
    return true;
}

bool QShareBasicInfoWorker::writeInfos(const ShareBaseDataList &list)
{
    FILE *fp = fopen(STOCK_CODE_FILE, "wb+");
    if(fp)
    {
        qDebug()<<__FUNCTION__<<__LINE__;
        qint64 cur = QDateTime(HqUtils::latestActiveDay().addDays(-1)).toMSecsSinceEpoch();
        fwrite(&cur, sizeof(cur), 1, fp);
        int size = list.size();
        fwrite(&size, sizeof(size), 1, fp);
        for(int i=0; i<size; i++)
        {
            fwrite(&list[i], sizeof(ShareBaseData), 1, fp);
        }

        //更新时间到最新，移动到开头写入时间，保证是最新的
        fseek(fp, 0, SEEK_SET);
        cur = QDateTime(HqUtils::latestActiveDay()).toMSecsSinceEpoch();
        fwrite(&cur, sizeof(cur), 1, fp);
        fclose(fp);
    } else
    {
        qDebug()<<__FUNCTION__<<__LINE__;
    }

    return true;
}

void QShareBasicInfoWorker::slotUpdateShareCodesList(const ShareBaseDataList &list)
{
    QMutexLocker locker(&mUpdateMutex);
    foreach (ShareBaseData data, list) {
        QString code = QString::fromStdString(data.mCode);
        ShareBaseData &wkVal = mShareBaseDataMap[code];
        wkVal.setCode(code);
        wkVal.setName(QString::fromStdString(data.mName));
        wkVal.setPY(QString::fromStdString(data.mPY));
        wkVal.mShareType = data.mShareType;
        //qDebug()<<wkVal.mCode<<wkVal.mName<<wkVal.mPY;
    }

}

void QShareBasicInfoWorker::slotUpdateShareFHSPList(const ShareBaseDataList &list)
{
    QMutexLocker locker(&mUpdateMutex);
    foreach (ShareBaseData data, list) {
        QString code = QString::fromStdString(data.mCode);
        ShareBaseData &wkVal = mShareBaseDataMap[code];
        wkVal.mSZZG = data.mSZZG;
        wkVal.mXJFH = data.mXJFH;
        wkVal.mGQDJR = data.mGQDJR;
        wkVal.mYAGGR = data.mYAGGR;
        //qDebug()<<wkVal.mCode<<wkVal.mSZZG<<wkVal.mXJFH;
    }
}

void QShareBasicInfoWorker::slotUpdateShareHsgtTop10List(const ShareBaseDataList &list)
{
    QMutexLocker locker(&mUpdateMutex);
    foreach (ShareBaseData data, list) {
        QString code = QString::fromStdString(data.mCode);
        ShareBaseData &wkVal = mShareBaseDataMap[code];
        wkVal.mIsTop10 = true;
        wkVal.mTop10Buy = data.mTop10Buy;
        wkVal.mTop10Sell = data.mTop10Sell;
//        qDebug()<<wkVal.mCode<<wkVal.mIsTop10<<wkVal.mTop10Buy;
    }
}

void QShareBasicInfoWorker::slotUpdateShareFinancialList(const ShareBaseDataList &list)
{
    QMutexLocker locker(&mUpdateMutex);
    foreach (ShareBaseData data, list) {
        QString code = QString::fromStdString(data.mCode);
        ShareBaseData &wkVal = mShareBaseDataMap[code];
        wkVal.mMGSY = data.mMGSY;
        wkVal.mMGJZC = data.mMGJZC;
        wkVal.mTotalShare = data.mTotalShare;
        wkVal.mMutalShare = data.mMutalShare;
        wkVal.mJZCSYL = data.mJZCSYL;
        wkVal.setPY(QString::fromStdString(data.mPY));
    }
}
