#include "qhttpget.h"
#include <QDebug>
#include <QThread>
#include <QEventLoop>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include "data_structure/hqutils.h"
#include "dbservices/dbservices.h"
#include "dbservices/qactivedate.h"

QHttpGet::QHttpGet(const QString& url, bool sequential, QObject *parent) :
    QThread(parent),mMgr(0), mUrl(url), mReply(0), mIsSequential(sequential), mUpdateTimer(0)
{
    mMgr = new QNetworkAccessManager;
    mInertVal = 3;
}

void QHttpGet::setSequential(bool sts)
{
    mIsSequential = sts;
}

QHttpGet::~QHttpGet()
{
    if(mUpdateTimer)
    {
        mUpdateTimer->stop();
        mUpdateTimer->deleteLater();
    }
    if(mMgr) delete mMgr;

}
#if 0
void QHttpGet::startGet()
{
    if(mIsSequential)
    {
        mUpdateTimer = new QTimer;
        mUpdateTimer->setInterval(mInertVal * 1000);
        connect(mUpdateTimer, SIGNAL(timeout()), this, SLOT(slotUpdateHttp()));
        mUpdateTimer->start();
    } else
    {
        slotUpdateHttp();
    }
}
#endif

void QHttpGet::setUpdateInterval(int secs)
{
    mInertVal = secs;
    if(mUpdateTimer)
    {
        mUpdateTimer->setInterval(mInertVal * 1000);
    }
}

//void QHttpGet::slotUpdateHttp()
//{
//    if(mUrl.length() == 0) return;
//    mReply = mMgr->get(QNetworkRequest(mUrl));
//    QEventLoop mainLoop;
//    connect(mReply, SIGNAL(finished())
////    qDebug()<<__FUNCTION__<<QThread::currentThread();
////    connect(mReply, SIGNAL(finished()), this, SLOT(slotReadHttpContent()));
//////    connect(mReply, SIGNAL(readyRead()), this, SLOT(slotStartReadHttpContent()));
////    connect(mReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SIGNAL(signalErrorOccured(QNetworkReply::NetworkError)));
//}

void QHttpGet::run()
{
    if(mUrl.length() == 0) return;
    QNetworkAccessManager mgr;
    bool active = true;
    QByteArray recv;
    do{
        if(mReply)
        {
            mReply->abort();
            mReply->close();
            delete mReply;
        }
        if(active)
        {
            mReply = mgr.get(QNetworkRequest(mUrl));
            if(mReply)
            {
                QEventLoop subloop;
                connect(mReply, SIGNAL(finished()), &subloop, SLOT(quit()));
                connect(mReply, SIGNAL(error(QNetworkReply::NetworkError)), &subloop, SLOT(quit()));
                subloop.exec();
                if(mReply->error() == QNetworkReply::NoError)
                {
                    recv = mReply->readAll();
                } else
                {
                    qDebug()<<"networkreply error:"<<mReply->error();
                }
                mReply->abort();
                mReply->close();
                delete mReply;
                mReply = 0;
                //QThread::run();
            }
        }
        QThread::sleep(mInertVal);
        active = QActiveDateTime::isCurDateTimeActive();
        if(recv.size() >=0)
        {
            emit signalSendHttpConent(recv);
        }

    } while(mIsSequential);
    //QThread::run();
}

void QHttpGet::setUrl(const QString &url)
{
    mUrl = url;
}

void QHttpGet::slotStartReadHttpContent()
{
}

void QHttpGet::slotReadHttpContent()
{
    qDebug()<<__FUNCTION__<<QThread::currentThread();
    if(!mReply) return;
    emit signalSendHttpConent(mReply->readAll());
    mReply->deleteLater();
}

QByteArray QHttpGet::getContentOfUrl(const QString &firstUrl, const QString &contentUrl)
{
    QByteArray recv;
    QNetworkAccessManager mgr;
    NetworkCookie *new_cookies = new NetworkCookie;
    mgr.setCookieJar(new_cookies);

    QNetworkReply *reply = mgr.get(QNetworkRequest(firstUrl));
//    qDebug()<<reply;
    if(!reply) return recv;

    QEventLoop subloop;
    connect(reply, SIGNAL(finished()), &subloop, SLOT(quit()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &subloop, SLOT(quit()));
    QTimer timer;
    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), &subloop, SLOT(quit()));
    timer.start(10*1000);
    subloop.exec();
    if(timer.isActive()) timer.stop();
    if(reply->error() == QNetworkReply::NoError && reply->isFinished())
    {
        qDebug()<<"fetch cookie:"<<new_cookies->getCookies();
        QByteArray total = reply->readAll();
        qDebug()<<reply->rawHeaderList();
        reply->deleteLater();
        QNetworkRequest request(contentUrl);
        QList<QNetworkCookie> cookielist;
        cookielist.append(QNetworkCookie("yfx_c_g_u_id_10000042", "_ck20072016335716456557625602252"));
        cookielist.append(QNetworkCookie("yfx_mr_10000042", "%3A%3Amarket_type_free_search%3A%3A%3A%3Abaidu%3A%3A%3A%3A%3A%3A%3A%3Awww.baidu.com%3A%3A%3A%3Apmf_from_free_search"));
        cookielist.append(QNetworkCookie("yfx_key_10000042", ""));
        cookielist.append(QNetworkCookie("yfx_mr_f_10000042", "%3A%3Amarket_type_free_search%3A%3A%3A%3Abaidu%3A%3A%3A%3A%3A%3A%3A%3Awww.baidu.com%3A%3A%3A%3Apmf_from_free_search"));
        cookielist.append(QNetworkCookie("JSESSIONID", "4B07EA7EC9DCC5B547214581F3478763"));
        cookielist.append(QNetworkCookie("yfx_f_l_v_t_10000042", "f_t_1595234037624__r_t_1595302118901__v_t_1595302118901__r_c_1"));
        cookielist.append(QNetworkCookie("VISITED_COMPANY_CODE", "%5B%22510010%22%5D"));
        cookielist.append(QNetworkCookie("VISITED_FUND_CODE", "%5B%22510010%22%5D"));
        cookielist.append(QNetworkCookie("VISITED_MENU", "%5B%228547%22%2C%228550%22%2C%2210848%22%2C%228491%22%5D"));



//Host: query.sse.com.cn
//Connection: keep-alive
//User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/84.0.4147.89 Safari/537.36
//Accept: */*
//Referer: http://www.sse.com.cn/market/funddata/volumn/etfvolumn/
//Accept-Encoding: gzip, deflate
//Accept-Language: zh-CN,zh;q=0.9
//Cookie: yfx_c_g_u_id_10000042=_ck20072115125217721926968350309; yfx_f_l_v_t_10000042=f_t_1595315572764__r_t_1595579117057__v_t_1595579117057__r_c_1; VISITED_MENU=%5B%228864%22%2C%228528%22%2C%2210025%22%2C%228547%22%2C%228491%22%5D


//        QVariant val;
//        val.setValue(cookielist);
//        request.setHeader(QNetworkRequest::CookieHeader, val);
        request.setRawHeader("Connection", "keep-alive");
        request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/84.0.4147.89 Safari/537.36");

        qDebug()<<request.rawHeaderList()<<request.rawHeader("Cookie");

        reply = mgr.get(request);
//        qDebug()<<request.header(QNetworkRequest::CookieHeader);

        //    qDebug()<<reply;
        if(!reply) return recv;

        QEventLoop subloop;
        connect(reply, SIGNAL(finished()), &subloop, SLOT(quit()));
        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &subloop, SLOT(quit()));
        QTimer timer;
        timer.setSingleShot(true);
        connect(&timer, SIGNAL(timeout()), &subloop, SLOT(quit()));
        timer.start(10*1000);
        subloop.exec();
        if(timer.isActive()) timer.stop();
        recv = reply->readAll();
        qDebug()<<"sse:"<<recv;
    }

    reply->abort();
    reply->close();
    delete reply;
    reply = 0;
    return recv;
}

QByteArray QHttpGet::getContentOfURL(const QString &url, const QList<QNetworkCookie>& list)
{
    QByteArray recv;
    QNetworkAccessManager mgr;
    NetworkCookie cookie;
    if(list.size() > 0)
    {
        cookie.setCookies(list);
        mgr.setCookieJar(&cookie);
    }
    QNetworkReply *reply = mgr.get(QNetworkRequest(url));
//    qDebug()<<reply;
    if(!reply) return recv;

    QEventLoop subloop;
    connect(reply, SIGNAL(finished()), &subloop, SLOT(quit()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &subloop, SLOT(quit()));
    QTimer timer;
    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), &subloop, SLOT(quit()));
    timer.start(10*1000);
    subloop.exec();
    if(timer.isActive())
    {
//        qDebug()<<"reply finished";
        timer.stop();
    }

//    qDebug()<<reply->isFinished()<<reply->errorString();
    if(reply->error() == QNetworkReply::NoError && reply->isFinished())
    {
        recv = reply->readAll();
    }

    reply->abort();
    reply->close();
    delete reply;
    reply = 0;
    return recv;
}

QByteArray QHttpGet::getContentOfURLWithPost(const QString &url, const QByteArray& post, int out)
{
    QByteArray recv;
    QNetworkAccessManager mgr;
    QNetworkReply *reply = mgr.post(QNetworkRequest(url), post);
    if(!reply) return recv;

    QEventLoop subloop;
    connect(reply, SIGNAL(finished()), &subloop, SLOT(quit()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &subloop, SLOT(quit()));
    QTimer timer;
    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), &subloop, SLOT(quit()));
    timer.start(out*1000);
    subloop.exec();
    if(timer.isActive())
    {
        timer.stop();
    }
    if(reply->error() == QNetworkReply::NoError && reply->isFinished())
    {
        recv = reply->readAll();
    }
    reply->abort();
    reply->close();
    delete reply;
    reply = 0;
    return recv;
}


