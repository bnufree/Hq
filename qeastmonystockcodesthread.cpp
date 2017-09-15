#include "qeastmonystockcodesthread.h"
#include <QDebug>
#include <QDateTime>
#include <QRegularExpression>
#include "qhttpget.h"

QEastMonyStockCodesThread::QEastMonyStockCodesThread(QObject *parent) : QThread(parent)
{

}

QEastMonyStockCodesThread::~QEastMonyStockCodesThread()
{

}

void QEastMonyStockCodesThread::run()
{
    QTime t = QDateTime::currentDateTime().time();
    t.start();

    //开始解析数据
    QByteArray bytes = QHttpGet::getContentOfURL(QString("http://quote.eastmoney.com/stocklist.html"));
    qDebug()<<"total time cost:"<<t.elapsed();
    t.start();
    QString result = QString::fromUtf8(bytes.data());
    int index = 0;
    QStringList list;
    while((index = result.indexOf(QRegularExpression(tr("s[hz](60[013][0-9]{3}|300[0-9]{3}|00[012][0-9]{3})")), index)) >= 0)
    {
        QString code = result.mid(index, 8);
        //qDebug()<<code;
        if(!list.contains(code)) list.append(code);
        index = index+8;
    }
    qDebug()<<"stock codes list:"<<list.length();
    emit signalSendCodesList(list);
    qDebug()<<"total time cost:"<<t.elapsed();
}

