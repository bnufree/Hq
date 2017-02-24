#include "basehttpget.h"
#include <QDateTime>
BaseHttpGet::BaseHttpGet(const QString& pUrl, QObject *parent)
             : QObject(parent),
               mUrl(pUrl),
               mFreq(2000)

{
    mTimer = new QTimer;
    mTimer->setInterval(mFreq);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(slotTimeOut()));
    mReply = NULL;
    mManager = new QNetworkAccessManager;
    //connect(mManager, SIGNAL(finished(QNetworkReply*)),this, SLOT(slotFinishedReply(QNetworkReply*)));
    //connect(this, SIGNAL(signalGetFaild()), this, SLOT(slotReceiveGetFailed()));
   // mTimer->start();
    startRequest();
}

BaseHttpGet::~BaseHttpGet()
{
    mManager->deleteLater();
    mTimer->deleteLater();


}


void BaseHttpGet::slotReceiveGetFailed()
{
    return;
}

void BaseHttpGet::startRequest()
{
    //qDebug()<<mUrl;
    QNetworkRequest request;
    request.setUrl(mUrl);
    mManager->get(request);
    //connect(mReply, SIGNAL(finished()), this, SLOT()
    mTimer->start();
    connect(mManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotFinishedReply(QNetworkReply*)));


}

void BaseHttpGet::slotTimeOut()
{
//    if(mReply && mReply->isRunning())
//    {
//        qDebug()<<"stop reply";
//        mReply->abort();
//        mReply->deleteLater();
//        mReply = NULL;
//    }

}

void BaseHttpGet::SetUrl(const QString &pUrl)
{
    mUrl = QUrl(pUrl);

}

QString BaseHttpGet::GetUrl()
{
    return mUrl.toString();
}

void BaseHttpGet::SetFreq(int pFreq)
{
    mFreq = pFreq;
}

int BaseHttpGet::GetFreq()
{
    return mFreq;
}

void BaseHttpGet::slotFinishedReply(QNetworkReply *reply)
{
//    qDebug()<<"end time:"<<QDateTime::currentDateTime();
    //qDebug()<<"child ThreadID: "<<QThread::currentThreadId()<<endl;
//    reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
//    if(reply->error() != QNetworkReply::NoError)
//    {
//        emit signalGetFaild();
//        return;
//    }
    reply->deleteLater();
    mTimer->stop();
    startRequest();
}
