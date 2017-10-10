#include "qeastmonystockcodesthread.h"
#include <QDebug>
#include <QDateTime>
#include <QRegularExpression>
#include "qhttpget.h"
#include <QFile>

QEastMonyStockCodesThread::QEastMonyStockCodesThread(QObject *parent) : QThread(parent)
{

}

QEastMonyStockCodesThread::~QEastMonyStockCodesThread()
{

}

bool QEastMonyStockCodesThread::writeCodes(const QStringList &codes)
{
    FILE *fp = fopen("stock.data", "wb+");
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

    }
    fclose(fp);

    return true;
}

bool QEastMonyStockCodesThread::getCodesFromFile(QStringList& codes)
{
    codes.clear();
    if(!QFile::exists("stock.data")) return false;
    //读取文件
    QFile file("stock.data");
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
                    codes.append(QString("").sprintf("sh%06d", code));
                } else
                {
                    codes.append(QString("").sprintf("sz%06d", code));
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
    if(!getCodesFromFile(list))
    {
        list.clear();
        //开始解析数据
        QByteArray bytes = QHttpGet::getContentOfURL(QString("http://quote.eastmoney.com/stocklist.html"));
        qDebug()<<"total time cost:"<<t.elapsed();
        t.start();
        QString result = QString::fromUtf8(bytes.data());
        int index = 0;
        while((index = result.indexOf(QRegularExpression(tr("s[hz](60[013][0-9]{3}|300[0-9]{3}|00[012][0-9]{3})")), index)) >= 0)
        {
            QString code = result.mid(index, 8);
            //qDebug()<<code;
            if(!list.contains(code)) list.append(code);
            index = index+8;
        }

        writeCodes(list);
    }
    //qDebug()<<"stock codes list:"<<list.length()<<" "<<list.mid(0, 100);
    emit signalSendCodesList(list);
    qDebug()<<"total time cost:"<<t.elapsed();
}

