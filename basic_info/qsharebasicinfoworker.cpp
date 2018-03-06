#include "qsharebasicinfoworker.h"
#include "qsharecodeswork.h"
#include "utils/hqutils.h"
#include "dbservices/hqdatadefines.h"

#define STOCK_CODE_FILE  "share.dat"

QShareBasicInfoWorker::QShareBasicInfoWorker(QObject *parent) : QObject(parent)
{
    connect(this, SIGNAL(signalGetBasicInfo()), this, SLOT(slotGetBasicInfo()));
    moveToThread(&mWorkThread);
    mWorkThread.start();
}

void QShareBasicInfoWorker::slotGetBasicInfo()
{
    //从文件获取信息
}

bool QShareBasicInfoWorker::getInfosFromFile(BaseDataList &list)
{

}

bool QShareBasicInfoWorker::getInfossFromWeb(BaseDataList &list)
{

}

bool QShareBasicInfoWorker::writeInfos(const BaseDataList &list)
{
    FILE *fp = fopen(STOCK_CODE_FILE, "wb+");
    if(fp)
    {
        qint64 cur = QDateTime(HqUtils::latestActiveDay().addDays(-1)).toMSecsSinceEpoch();
        fwrite(&cur, sizeof(cur), 1, fp);
        int size = list.size();
        fwrite(&size, sizeof(size), 1, fp);
        for(int i=0; i<list.size(); i++){
            ShareData data = ShareDataList[i];
            int val = list[i].toInt();
            fwrite(&val, sizeof(int), 1, fp);
            //HqUtils::writeInt2File(list[i].toInt(), fp);
        }

        //更新时间到最新，移动到开头写入时间，保证是最新的
        fseek(fp, 0, SEEK_SET);
        cur = QDateTime(HqUtils::latestActiveDay()).toMSecsSinceEpoch();
        fwrite(&cur, sizeof(cur), 1, fp);
        fclose(fp);
    }

    return true;
}

void QShareBasicInfoWorker::slotUpdateShareCodesList(const BaseDataList &list)
{

}


#if 0
#define STOCK_CODE_FILE  "share.dat"
QShareCodesThread::QShareCodesThread(QObject *parent) : QObject(parent)
{
    connect(DATA_SERVICE, SIGNAL(signalDbInitFinished()), this, SLOT(slotDBInitFinished()));
    connect(this, SIGNAL(signalGetCode()), this, SLOT(slotGetCode());
    this->moveToThread(&mThread);
    mThread.start();
}

QShareCodesThread::~QShareCodesThread()
{
    mThread.quit();
}

bool QShareCodesThread::writeCodes(const QStringList &list)
{
    FILE *fp = fopen(STOCK_CODE_FILE, "wb+");
    if(fp)
    {
        qint64 cur = QDateTime::currentDateTime().addDays(-1).toMSecsSinceEpoch();
        fwrite(&cur, sizeof(cur), 1, fp);
        int size = list.size();
        fwrite(&size, sizeof(size), 1, fp);
        for(int i=0; i<list.size(); i++){
            int val = list[i].toInt();
            fwrite(&val, sizeof(int), 1, fp);
            //HqUtils::writeInt2File(list[i].toInt(), fp);
        }

        //更新时间到最新，移动到开头写入时间，保证是最新的
        fseek(fp, 0, SEEK_SET);
        cur = QDateTime::currentMSecsSinceEpoch();
        fwrite(&cur, sizeof(cur), 1, fp);
        fclose(fp);
    }

    return true;
}

bool QShareCodesThread::getCodesFromFile(QStringList& codes)
{
    codes.clear();
    if(!QFile::exists(STOCK_CODE_FILE)) return false;
    //读取文件
    QFile file(STOCK_CODE_FILE);
    if(!file.open(QIODevice::ReadOnly)) return false;
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
                int code = 0;
                file.read((char*)(&code), sizeof(int));
                codes.append(QString("").sprintf("%06d", code));
            }
        }
    }
    file.close();
    if(totalNum != 0 && codes.length() == totalNum)
    {
        return true;
    }

    return false;
}

void QShareCodesThread::slotGetCode()
{
    QTime t = QDateTime::currentDateTime().time();
    t.start();

    QStringList list;
    //先从文件获取，获取不成功，从网络获取
    if(!getCodesFromFile(list))
    {
        getCodesFromURL(list,"http://quote.eastmoney.com/stocklist.html");
        if(list.length() > 0)
        {
            //写入本地代码文件
             writeCodes(list);
        }
    }
    //更新到后台
    slotRecvAllCodes(list);
}


void QShareCodesThread::slotRecvAllCodes(const QStringList &list)
{
    mCodesList.clear();
    Profiles::instance()->setDefault("UPDATE", "MODE", 1);
    int mode = Profiles::instance()->value("UPDATE", "MODE", 1).toInt();
    foreach (QString code, list) {
        if(mode == 1)
        {
            if(code.toInt() > 500000)
            {
                mCodesList.append(QString("").sprintf("s_sh%06d", code.toInt()));
            } else
            {
                mCodesList.append(QString("").sprintf("s_sz%06d", code.toInt()));
            }
        } else
        {
            mCodesList.append(HqUtils::prefixCode(code) + code );
        }

    }
    emit DATA_SERVICE->signalUpdateStockCodesList(mCodesList);
}

void QShareCodesThread::slotDBInitFinished()
{
    emit signalSendCodesList(mCodesList);
}

void QShareCodesThread::getCodesFromURL(QStringList& list, const QString& URL)
{
    QTextCodec *gbkCodec = QTextCodec::codecForName("UTF8");
    QString result = QString::fromLocal8Bit(QHttpGet::getContentOfURL(URL));
    QRegExp reg(">([\u4e00-\u9fa5A-Z0-9]{1,})\\(([0-9]{6})\\)<");
    QRegExp reg_code("60[013][0-9]{3}|300[0-9]{3}|00[012][0-9]{3}|510[0-9]{3}|1599[0-9]{2}");
    int index = 0;
    while((index = reg.indexIn(result, index)) >= 0)
    {
        QString name = reg.cap(1);
        QString code = reg.cap(2);
        if(reg_code.exactMatch(code))
        {
//            StockData data;
//            data.mCode = code;
//            data.mName = name;
//            data.mPY = HqUtils::GetFirstLetter(gbkCodec->toUnicode( data.mName.toStdString().data()));
            list.append(code);
        }
        index += reg.matchedLength();
    }
    return;
}
#endif
