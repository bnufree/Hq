#include "qsharebasicinfoworker.h"
#include "qsharecodeswork.h"
#include "data_structure/hqutils.h"
#include "dbservices/hqdatadefines.h"
#include <QThreadPool>
#include "qsharecodeswork.h"
#include "qsharefhspwork.h"
#include "qsharehsgttop10work.h"
#include "qsharefinancialinfowork.h"
#include <QFile>
#include "dbservices/dbservices.h"
#include "utils/comdatadefines.h"
#include "dbservices/qactivedate.h"

#define         STOCK_CODE_FILE         QString("%1share.data").arg(HQ_CODE_DIR)
QShareBasicInfoWorker::QShareBasicInfoWorker(QObject *parent) : QObject(parent)
{
    connect(this, SIGNAL(signalGetBasicInfo()), this, SLOT(slotGetBasicInfo()));
    moveToThread(&mWorkThread);
    mWorkThread.start();
}

void QShareBasicInfoWorker::slotGetBasicInfo()
{
<<<<<<< HEAD
    //获取数据库的更新时间.决定从数据库获取还是网络获取
    //首先获取数据库的更新日期
    ShareDate update_date = DATA_SERVICE->getLastUpdateDateOfBasicInfo();
    if(update_date == ShareDate::latestActiveDay())return;
    //开始从网络获取数据
    QThreadPool pool;
    pool.setMaxThreadCount(8);
    pool.setExpiryTimeout(-1);

    //取得代码
    pool.start(new QShareCodesWork(this));
    //取得分红送配
    pool.start(new QShareFHSPWork("2017-12-31", this));
    //取得北向交易TOP10
    pool.start(new QShareHsgtTop10Work(QActiveDateTime::latestActiveDay().toString(DATE_FORMAT), this));
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


//    //从文件获取信息基本信息，包括代码，分红送配，财务信息
//    if(!getInfosFromFile(mShareBaseDataMap))
//    {
//        getInfossFromWeb(mShareBaseDataMap);
//        writeInfos(mShareBaseDataMap.values());
//    }
//    //获取陆股通TOP10数据
//    QThreadPool pool;
//    pool.setExpiryTimeout(-1);
//    QDate queryDate = QActiveDateTime::latestActiveDay();
//    //取得北向交易TOP10
//    pool.start(new QShareHsgtTop10Work(queryDate.toString(DATE_FORMAT), this));
//    pool.waitForDone();
//    emit signalBaseDataListFinished(QStringList(mShareBaseDataMap.keys()), mShareBaseDataMap.values());

}

void QShareBasicInfoWorker::updateShareFavCode(const QString &code)
{
    qDebug()<<__func__<<code;
    ShareBaseData &data = mShareBaseDataMap[ShareBaseData::fullCode(code)];
    data.mIsFav = !(data.mIsFav);
    qDebug()<<__func__<<data.mCode<<data.mIsFav;
    writeInfos(mShareBaseDataMap.values());
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
    qDebug()<<"file path:"<<file.fileName();
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
        QDate lastDate = QDateTime::fromMSecsSinceEpoch(lastupdate).date();
        QDate latestActiveDate = QActiveDateTime::latestActiveDay();
        qDebug()<<"last:"<<lastDate<<"latest:"<<latestActiveDate;
        if(lastDate == latestActiveDate)
        {
            file.read((char*)(&totalNum), sizeof(int));
            int count = 0;
            while (!file.atEnd() ) {
                ShareBaseData data;
                file.read((char*)(&data), sizeof(ShareBaseData));
                map[QString::fromStdString(data.mCode)] = data;
                //qDebug()<<data.mCode<<data.mName;
            }
        }
    }
    file.close();
    qDebug()<<"total num:"<<totalNum<<" read count:"<<map.count();
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
=======
>>>>>>> c3e20bfc50cfd7f11c19b3ff55625200506659c7
    QThreadPool pool;
    pool.setMaxThreadCount(8);
    pool.setExpiryTimeout(-1);
    //取得代码
    pool.start(new QShareCodesWork(this));
    //取得分红送配
    pool.start(new QShareFHSPWork(this));
#if 0
    //取得北向交易TOP10
    pool.start(new QShareHsgtTop10Work(this));
    pool.waitForDone();
    //取得财务信息
    QStringList allCodes = DATA_SERVICE->getAllShareCodes();
    int pos = 0;
    int section = 200;
    while(pos < allCodes.length())
    {
        QStringList sublist = allCodes.mid(pos, section);
        pos += section;
        pool.start(new QShareFinancialInfoWork(sublist, this));
    }
#endif
    pool.waitForDone();
}
