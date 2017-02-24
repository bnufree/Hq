#include "qsinasearchthread.h"
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QDebug>

QSinaSearchThread::QSinaSearchThread(QObject *parent) : QThread(parent)
{
    mSearchText = "";
}

QSinaSearchThread::~QSinaSearchThread()
{

}

void QSinaSearchThread::run()
{
    QNetworkAccessManager *mgr = new QNetworkAccessManager;
    while (true) {
        if(mSearchText.length() == 0) continue;
        QString wkURL = QString("http://suggest3.sinajs.cn/suggest/type=11&key=%1&name=suggestdata_%2")
                .arg(mSearchText)
                .arg(QDateTime::currentDateTime().toMSecsSinceEpoch());

        QNetworkReply *reply  = mgr->get(QNetworkRequest(wkURL));
        if(!reply)
        {
            mSearchText.clear();
            continue;
        }

        QEventLoop loop; // 使用事件循环使得网络通讯同步进行
        connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec(); // 进入事件循环， 直到reply的finished()信号发出， 这个语句才能退出
        if(reply->error())
        {
            qDebug()<<"err occured:"<<reply->errorString();
            reply->deleteLater();
            mgr->deleteLater();
            return;
        }
        //开始解析数据
        QByteArray bytes = reply->readAll();
        QString result = QString::fromLocal8Bit(bytes.data());
        int start = 0;
        QStringList resultlist;
        while((start = result.indexOf(QRegExp("[shz]{2}[0-9]{6}"), start))  != -1)
        {
            resultlist.append(result.mid(start, 8));
            start += 1;
        }
        //qDebug()<<"search result:"<<resultlist;
        emit sendSearchResult(resultlist);
        mSearchText.clear();
    }

}

void QSinaSearchThread::setSearchString(const QString &text)
{
    mSearchText = text;
}
