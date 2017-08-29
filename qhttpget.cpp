#include "qhttpget.h"
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QNetworkReply>
#include <QDebug>
#include <QThread>
#include <QTime>
#include "webfile/webfile.h"

QHttpGet::QHttpGet(const QString& url, QObject *parent) : QObject(parent)
{
    mUrl = url;
}

QHttpGet::~QHttpGet()
{

}

QByteArray QHttpGet::getContent(const QString &url)
{
    QString wkURL = url.length() != 0 ? url : mUrl;
    if(wkURL.length() == 0) return QByteArray();

    QNetworkAccessManager *mgr = new QNetworkAccessManager;
    QNetworkReply *reply = mgr->get(QNetworkRequest(wkURL));
    if(!reply)
    {
        delete mgr;
        return QByteArray();
    }
    QEventLoop loop; // 使用事件循环使得网络通讯同步进行
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec(); // 进入事件循环， 直到reply的finished()信号发出， 这个语句才能退出
    if(reply->error()) return QByteArray();;

    //开始解析数据
    QByteArray res = reply->readAll();
    reply->deleteLater();
    mgr->deleteLater();
    return res;
}

QByteArray QHttpGet::getContentOfURL(const QString& url)
{
    qDebug()<<__FUNCTION__<<__LINE__;
    if(url.length() == 0) return QByteArray();
    webfile file(url);
    QByteArray res;
    if(file.open())
    {
        res = file.readAll();
        file.close();
    }

    return res;
}


