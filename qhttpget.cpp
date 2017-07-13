#include "qhttpget.h"
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QNetworkReply>
#include <QScopedPointer>

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

    QScopedPointer<QNetworkAccessManager> mgr(new QNetworkAccessManager);
    QScopedPointer<QNetworkReply> reply(mgr.data()->get(QNetworkRequest(wkURL)));
    if(!reply) return QByteArray();
    QEventLoop loop; // 使用事件循环使得网络通讯同步进行
    connect(reply.data(), SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec(); // 进入事件循环， 直到reply的finished()信号发出， 这个语句才能退出
    if(reply.data()->error()) return QByteArray();;

    //开始解析数据
    return reply.data()->readAll();
}

