#include "qhttpget.h"
#include <QDebug>
#include <QThread>
#include <QEventLoop>

QHttpGet::QHttpGet(const QString& url, bool sequential, QObject *parent) :
    QThread(parent),mMgr(0), mUrl(url), mReply(0), mIsSequential(sequential), mUpdateTimer(0)
{
    mMgr = new QNetworkAccessManager;
    mInertVal = 3;
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
    do{
        if(mReply)
        {
            mReply->abort();
            mReply->close();
            delete mReply;
        }
        mReply = mgr.get(QNetworkRequest(mUrl));
        if(mReply)
        {
            QEventLoop subloop;
            connect(mReply, SIGNAL(finished()), &subloop, SLOT(quit()));
            connect(mReply, SIGNAL(error(QNetworkReply::NetworkError)), &subloop, SLOT(quit()));
            subloop.exec();
            if(mReply->error() == QNetworkReply::NoError)
            {
                emit signalSendHttpConent(mReply->readAll());
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
        QThread::sleep(mInertVal);
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

QByteArray QHttpGet::getContentOfURL(const QString &url)
{
    QByteArray recv;
    QNetworkAccessManager mgr;
    QNetworkReply *reply = mgr.get(QNetworkRequest(url));
    if(!reply) return recv;

    QEventLoop subloop;
    connect(reply, SIGNAL(finished()), &subloop, SLOT(quit()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &subloop, SLOT(quit()));
    subloop.exec();
    if(reply->error() == QNetworkReply::NoError)
    {
        recv = reply->readAll();
    }
    reply->abort();
    reply->close();
    delete reply;
    reply = 0;
    return recv;
}


