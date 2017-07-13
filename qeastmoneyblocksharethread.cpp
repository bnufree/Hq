#include "qeastmoneyblocksharethread.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QRegularExpression>

QEastMoneyBlockShareThread::QEastMoneyBlockShareThread(const QString& pBlockCode, QObject *parent) : QThread(parent)
{
    mBlockCode = pBlockCode;
}

QEastMoneyBlockShareThread::~QEastMoneyBlockShareThread()
{

}

void QEastMoneyBlockShareThread::run()
{
    QStringList     sharecodeslist;
    QNetworkAccessManager *mgr = new QNetworkAccessManager;
    QString wkURL = QString("http://nufm.dfcfw.com/EM_Finance2014NumericApplication/JS.aspx?type=CT&cmd=C.BK0wkcode1&sty=FCOIATA&sortType=C&sortRule=-1&page=1&pageSize=1000&js=var%20quote_123%3d{rank:[(x)],pages:(pc)}&token=7bc05d0d4c3c22ef9fca8c2a912d779c&jsName=quote_123&_g=0.5186116359042079");
    wkURL.replace("wkcode", mBlockCode.right(3));

    QNetworkReply *reply  = mgr->get(QNetworkRequest(wkURL));
    if(!reply) return;
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
    QString result = QString::fromUtf8(bytes.data());

    int index = 0;
    while((index = result.indexOf(QRegularExpression(tr("[12]{1},(60[013][0-9]{3}|300[0-9]{3}|00[012][0-9]{3})")), index)) >= 0)
    {
        QString code = result.mid(index, 8);
        code.left(1) == "1"? code.replace(0, 2, "sh"):code.replace(0, 2, "sz");
        index = index+8;
        sharecodeslist.append(code);
    }

    reply->deleteLater();
    mgr->deleteLater();

    emit signalUpdateBlockShareCodeList(mBlockCode, sharecodeslist);
}

