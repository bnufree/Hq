#include "qeastmonystockcodesthread.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QDebug>
#include <QDateTime>
#include <QRegularExpression>

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
    QEventLoop loop;
    QNetworkAccessManager *mgr = new QNetworkAccessManager;
    QNetworkReply *reply  = mgr->get(QNetworkRequest(QString("http://quote.eastmoney.com/stocklist.html")));
    if(!reply) goto FUNC_END;

    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec(); // 进入事件循环， 直到reply的finished()信号发出， 这个语句才能退出
    if(reply->error()) goto FUNC_END;

    //开始解析数据
    {
        QByteArray bytes = reply->readAll();
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
        emit signalSendCodesList(list);
    }
FUNC_END:
    qDebug()<<"total time cost:"<<t.elapsed();
    if(reply) reply->deleteLater();
    mgr->deleteLater();
}

