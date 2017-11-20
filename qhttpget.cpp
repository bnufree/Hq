#include "qhttpget.h"
#include <QDebug>

QHttpGet::QHttpGet(const QString& url, bool sequential, QObject *parent) :
    QObject(parent),mMgr(0), mUrl(url), mReply(0), mIsSequential(sequential), mUpdateTimer(0)
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

void QHttpGet::setUpdateInterval(int secs)
{
    mInertVal = secs;
    if(mUpdateTimer)
    {
        mUpdateTimer->setInterval(mInertVal * 1000);
    }
}

void QHttpGet::slotUpdateHttp()
{
    if(mUrl.length() == 0) return;
    mReply = mMgr->get(QNetworkRequest(mUrl));
    connect(mReply, SIGNAL(finished()), this, SLOT(slotReadHttpContent()));
//    connect(mReply, SIGNAL(readyRead()), this, SLOT(slotStartReadHttpContent()));
    connect(mReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SIGNAL(signalErrorOccured(QNetworkReply::NetworkError)));
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
    if(!mReply) return;
    emit signalSendHttpConent(mReply->readAll());
    mReply->deleteLater();
}


