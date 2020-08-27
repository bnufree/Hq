#include "qhttpget.h"
#include <QDebug>
#include <QThread>
#include <QEventLoop>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QTextCodec>

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


void QHttpGet::setUpdateInterval(int secs)
{
    mInertVal = secs;
    if(mUpdateTimer)
    {
        mUpdateTimer->setInterval(mInertVal * 1000);
    }
}


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
//        active = QActiveDateTime::isCurDateTimeActive();
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

QByteArray QHttpGet::getContentOfURL(const QString &url, const QList<QNetworkCookie>& list)
{
    QByteArray recv;
    QNetworkAccessManager mgr;
    QNetworkRequest request(url);
    foreach (QNetworkCookie cookie, list) {
        request.setRawHeader(cookie.name(), cookie.value());
    }
    QNetworkReply *reply = mgr.get(request);
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
    if(reply->error() == QNetworkReply::NoError)
    {
        if(reply->isFinished())
        {
            QString content_type = reply->header(QNetworkRequest::ContentTypeHeader).toString();
            int index = content_type.indexOf("charset=");
            QTextCodec *htmlCodes = 0;
            if(index >= 0)
            {
                int last_index = content_type.indexOf(QRegExp("[;\"]"), index);
                QString name = content_type.mid(index+8);
                htmlCodes = QTextCodec::codecForName(name.toLatin1());
            }
            recv = reply->readAll();
            if(htmlCodes && htmlCodes->name() != QTextCodec::codecForLocale()->name())
            {
                recv = QTextCodec::codecForLocale()->fromUnicode(htmlCodes->toUnicode(recv));
            }
        }
        reply->abort();
        reply->close();
    }    
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


