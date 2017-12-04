#include "qeastmonystockcodesthread.h"
#include <QDebug>
#include <QDateTime>
#include <QRegularExpression>
#include <QFile>
#include <QDir>

#define STOCK_CODE_FILE  "stock.data"
QEastMonyStockCodesThread::QEastMonyStockCodesThread(QObject *parent) : QObject(parent)
{
    mHttp = 0;
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

bool QEastMonyStockCodesThread::writeCodes(const QStringList &codes)
{
    FILE *fp = fopen(STOCK_CODE_FILE, "wb+");
    if(fp)
    {
        qint64 cur = QDateTime::currentMSecsSinceEpoch();
        fwrite(&cur, sizeof(cur), 1, fp);
        int *stks = new int[codes.length()+1];
        *stks = codes.length();
        int i=1;
        foreach (QString code, codes) {
            *(stks+i) = code.right(6).toInt();
            i++;
        }
        fwrite(stks, sizeof(int), codes.length() + 1, fp);

        fclose(fp);

    }

    return true;
}

bool QEastMonyStockCodesThread::getCodesFromFile(QStringList& codes)
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
            while (!file.atEnd() ) {
                int code = 0;
                file.read((char*)(&code), sizeof(int));
                if(code > 500000)
                {
                    codes.append(QString("").sprintf("s_sh%06d", code));
                } else
                {
                    codes.append(QString("").sprintf("s_sz%06d", code));
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

    QStringList list;
    if(getCodesFromFile(list))
    {
        emit signalSendCodesList(list);
        return;
    }

    mHttp = new QHttpGet(QString("http://quote.eastmoney.com/stocklist.html"));
    connect(mHttp, SIGNAL(signalSendHttpConent(QByteArray)), this, SLOT(slotRecvHttpContent(QByteArray)));
    mHttp->start();
}

void QEastMonyStockCodesThread::slotRecvHttpContent(const QByteArray &bytes)
{
    QStringList list;
    QString result = QString::fromUtf8(bytes.data());
    int index = 0;
    while((index = result.indexOf(QRegularExpression(tr("s[hz](60[013][0-9]{3}|300[0-9]{3}|00[012][0-9]{3}|510[0-9]{3}|15[0-9]{4})")), index)) >= 0)
    {
        QString code = result.mid(index, 8);
        if(!list.contains(code)) list.append("s_"+code);
        index = index+8;
    }

    writeCodes(list);
    emit signalSendCodesList(list);
}

