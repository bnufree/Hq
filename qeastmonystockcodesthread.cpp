#include "qeastmonystockcodesthread.h"
#include <QDebug>
#include <QDateTime>
#include <QRegularExpression>
#include <QRegExp>
#include <QFile>
#include <QDir>
#include "dbservices/dbservices.h"
#include "utils/hqutils.h"
#include <QTextCodec>

#define STOCK_CODE_FILE  "share.dat"
QEastMonyStockCodesThread::QEastMonyStockCodesThread(QObject *parent) : QObject(parent)
{
    mHttp = new QHttpGet(QString("http://quote.eastmoney.com/stocklist.html"));
    connect(DATA_SERVICE, SIGNAL(signalDbInitFinished()), this, SLOT(slotDBInitFinished()));
    connect(mHttp, SIGNAL(signalSendHttpConent(QByteArray)), this, SLOT(slotRecvHttpContent(QByteArray)));
    connect(this, SIGNAL(start()), this, SLOT(run()));
    this->moveToThread(&mThread);
    mThread.start();
}

QEastMonyStockCodesThread::~QEastMonyStockCodesThread()
{
    if(mHttp) mHttp->deleteLater();
    mThread.quit();
    mThread.deleteLater();
}

bool QEastMonyStockCodesThread::writeCodes(const StockDataList &list)
{
    FILE *fp = fopen(STOCK_CODE_FILE, "wb+");
    if(fp)
    {
        qint64 cur = QDateTime::currentDateTime().addDays(-1).toMSecsSinceEpoch();
        fwrite(&cur, sizeof(cur), 1, fp);
        int size = list.size();
        fwrite(&size, sizeof(size), 1, fp);
        for(int i=0; i<list.size(); i++){
            HqUtils::writeString2File(list[i].mCode, fp);
            HqUtils::writeString2File(list[i].mName, fp);
            HqUtils::writeString2File(list[i].mPY, fp);
        }

        //更新时间到最新，移动到开头写入时间，保证是最新的
        fseek(fp, 0, SEEK_SET);
        cur = QDateTime::currentMSecsSinceEpoch();
        fwrite(&cur, sizeof(cur), 1, fp);
        fclose(fp);
    }

    return true;
}

bool QEastMonyStockCodesThread::getCodesFromFile(StockDataList& codes)
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
        //qDebug()<<"read date:"<<QDateTime::fromMSecsSinceEpoch(lastupdate).date();
        if(QDateTime::fromMSecsSinceEpoch(lastupdate).date() == QDate::currentDate())
        {
            file.read((char*)(&totalNum), sizeof(int));
            //qDebug()<<"total num:"<<totalNum;
            int count = 0;
            QStringList list;
            while (!file.atEnd() ) {
                int size = 0;
                file.read((char*)(&size), sizeof(int));
                if(size > 0)
                {
                    list.append(QString::fromUtf8(file.read(size)));
                }
                count++;
                if(count == 3)
                {
                    if(list.length() == 3)
                    {
                        StockData data;
                        data.mCode = list[0];
                        data.mName = list[1];
                        data.mPY = list[2];
                        codes.append(data);
                    }
                    list.clear();
                }
            }
        }
    }
    file.close();
    //qDebug()<<"cods:"<<codes;
    if(totalNum != 0 && codes.length() == totalNum)
    {
        return true;
    }


    return false;
}

void QEastMonyStockCodesThread::run()
{
    QTime t = QDateTime::currentDateTime().time();
    t.start();

    StockDataList list;
    if(getCodesFromFile(list))
    {
        slotRecvAllCodes(list);
        return;
    }
    if(mHttp)
    mHttp->start();
}

void QEastMonyStockCodesThread::slotRecvHttpContent(const QByteArray &bytes)
{
    StockDataList list;
    QTextCodec *gbkCodec = QTextCodec::codecForName("UTF8");
    QString result = QString::fromLocal8Bit(bytes.data());
    QRegExp reg(">([\u4e00-\u9fa5A-Z0-9]{1,})\\(([0-9]{6})\\)<");
    QRegExp reg_code("60[013][0-9]{3}|300[0-9]{3}|00[012][0-9]{3}|510[0-9]{3}|1599[0-9]{2}");
    int index = 0;
    while((index = reg.indexIn(result, index)) >= 0)
    {
        QString name = reg.cap(1);
        QString code = reg.cap(2);
        if(reg_code.exactMatch(code))
        {
            StockData data;
            data.mCode = code;
            data.mName = name;
            data.mPY = HqUtils::GetFirstLetter(gbkCodec->toUnicode( data.mName.toStdString().data()));
            list.append(data);
        }
        index += reg.matchedLength();
    }
    writeCodes(list);
    slotRecvAllCodes(list);
}

void QEastMonyStockCodesThread::slotRecvAllCodes(const StockDataList &list)
{
    mCodesList.clear();
    foreach (StockData data, list) {
        int code = data.mCode.toInt();
        if(code > 500000)
        {
            mCodesList.append(QString("").sprintf("s_sh%06d", code));
        } else
        {
            mCodesList.append(QString("").sprintf("s_sz%06d", code));
        }
    }
    emit DATA_SERVICE->signalUpdateStockCodesList(mCodesList);
}

void QEastMonyStockCodesThread::slotDBInitFinished()
{
    emit signalSendCodesList(mCodesList);
}

