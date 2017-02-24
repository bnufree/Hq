#include "httppost.h"
#include<QDir>
#include<QFile>
#include<QJsonParseError>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>
#include<QDebug>

#include"Log.h"
#include<QCoreApplication>

static const char * MESSAGETYPESTR = "messagetype";
static const char * MESSAGEVALUESTR = "messagevalue";
static const char * MESSAGECHANSTR = "messagechan";
static const char * MESSAGEURLSTR = "messageurl";

static const char * MESSAGE_WARNLINE = "warnline";
static const char * MESSAGE_WARNZONE = "warnzone";
static const char * MESSAGE_WARNRESULT = "warnresult";


#define MAX_HTTP_POST_STR_LEN  2048

namespace Dataservices {


HttpPost::HttpPost(QObject *parent)
    :QObject(parent),
      m_bqurlState(false),
      m_freq(300),
      m_bpostEnable(false)
{
    m_bStart = false;
    m_pFun = 0;
    m_pUser = 0;
    m_targetParse = new AnalyticsDataParse;
    m_timer = new QTimer;
    m_timer->setInterval(m_freq);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slotTimeOut()));
    m_manager = new QNetworkAccessManager;

    connect(m_manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(slotFinishedReply(QNetworkReply*)));
    connect(this, SIGNAL(signalPostFaild()), this, SLOT(slotReceivePostFailed()));
}

HttpPost::~HttpPost()
{

    m_postthread.exit();
    m_timer->stop();
    m_bpostEnable = false;

}

void HttpPost::AddWarningRulePost()
{
    int  numb;

    for(int i =0; i < MAX_WAR_LINE; i++)
    {
//        QJsonObject ruleObject;
//        PGWARLINE lineValue = m_warinfo.aLine[i];    // TODO 我们取得是第0个
//        if(lineValue.bEnable == 0)
//        {
//            continue;
//        }
//        ruleObject.insert("Info", "WarningLine");
//        numb = m_warinfo.nScreenW;
//        ruleObject.insert("sceneWidth", numb);
//        numb = m_warinfo.nScreenH;
//        ruleObject.insert("sceneHeight", numb);
//        numb = i;
//        ruleObject.insert("warningLineID", numb);
//        numb = lineValue.startPoint.ptx;
//        ruleObject.insert("point1_x", numb);
//        numb = lineValue.startPoint.pty;
//        ruleObject.insert("point1_y", numb);
//        numb = lineValue.endPoint.ptx;
//        ruleObject.insert("point2_x", numb);
//        numb = lineValue.endPoint.pty;
//        ruleObject.insert("point2_y", numb);
//        QJsonDocument document;
//        document.setObject(ruleObject);
//        QByteArray byte_array = document.toJson(QJsonDocument::Compact);
//        QByteArray temp =  byte_array;

//        PostData( byte_array,WARNING_LINE );

////        QNetworkRequest request;
////        request.setUrl(m_qurl);
////        request.setHeader(QNetworkRequest::ContentTypeHeader, temp);
////        m_manager->post(request, temp);
////        LOG(LOG_RTM,"Channel[%x]HttpPost::AddWarningRulePost addline url=%s",this,m_qurl.toString().toStdString().c_str());
////        LOG(LOG_RTM,"Channel[%x]HttpPost::AddWarningRulePost addline url=%s)",this,temp.toStdString().c_str());
    }

    for(int i = 0; i < MAX_WAR_ZONE; i++)
    {
//        QJsonObject ruleObject;
//        PGWARZONE zoneValue = m_warinfo.aZone[i];
//        if(zoneValue.bEnable == 0)continue;
//        ruleObject.insert("Info", "WatchZoneInfo");
//        numb = m_warinfo.nScreenW;
//        ruleObject.insert("sceneWidth", numb);
//        numb = m_warinfo.nScreenH;
//        ruleObject.insert("sceneHeight", numb);
//        numb = i;
//        qDebug()<<"zoneID: "<<numb<<endl;
//        ruleObject.insert("WatchZoneID", numb);
//        QJsonArray pointListArray;
//        int pointcnt=0;
//        for (size_t j = 0; j < MAX_ZONE_POINT; j++)
//        {
//            if(zoneValue.aPoint[j].bEnable == 0) continue;
//            QJsonObject point;
//            point.insert("x", zoneValue.aPoint[j].ptx);
//            point.insert("y", zoneValue.aPoint[j].pty);
//            qDebug()<<"*****************************************************"<<endl;
//            qDebug()<<"x: "<<zoneValue.aPoint[j].ptx<<" y: "<<zoneValue.aPoint[j].pty<<endl;
//            pointListArray.insert(pointcnt, point);
//            pointcnt++;
//        }
//        ruleObject.insert("WatchZones", pointListArray);
//        QJsonDocument document;
//        document.setObject(ruleObject);
//        QByteArray byte_array = document.toJson(QJsonDocument::Compact);
//        QByteArray temp =  byte_array;

//        PostData( byte_array,WARNING_ZONE );

////        QNetworkRequest request;
////        request.setUrl(m_qurl);
////        request.setHeader(QNetworkRequest::ContentTypeHeader, temp);
////        m_manager->post(request, temp);
////        LOG(LOG_RTM,"Channel[%x]HttpPost::AddWarningRulePost addzone url=%s",this,m_qurl.toString().toStdString().c_str());
////        LOG(LOG_RTM,"Channel[%x]HttpPost::AddWarningRulePost addzone data=%s",this,temp.toStdString().c_str());
    }
}

void HttpPost::SetCbkFun(HTTPPOSTCBKFUN pFun,void *pUser )
{
    m_pFun = pFun;
    m_pUser = pUser;
}

void HttpPost::StartPost(/* CAMWARNCFGINFO &info*/)
{
//     memcpy(&m_warinfo,&info,sizeof(m_warinfo));
//     m_freq = m_warinfo.nObjRequestFreq;

//     m_qurl = QString::fromLocal8Bit(m_warinfo.szObjRequestUrl);

//     LOG(LOG_RTM,"Channel[%x]HttpPost::StartPost rtspurl=%s",this,m_warinfo.szRtspUrl);
//     LOG(LOG_RTM,"Channel[%x]HttpPost::StartPost szObjRequestUrl=%s",this,m_warinfo.szObjRequestUrl);
//     LOG(LOG_RTM,"Channel[%x]HttpPost::StartPost szObjRequestType=%s",this,m_warinfo.szObjRequestType);
//     LOG(LOG_RTM,"Channel[%x]HttpPost::StartPost szTargetResult=%s",this,m_warinfo.szTargetResult);


//    m_bqurlState = true;
//    AddWarningRulePost(  );

//    LOG(LOG_ERROR,"HttpPost::slotStartPost");
//    m_bpostEnable = true;
//    if(!m_bStart)
//    {
//        m_timer->start();
////        this->moveToThread(&m_postthread);
//        m_postthread.start();
//        m_bStart = true;
//    }

}

void HttpPost::slotStopPost()
{
    if(m_bqurlState)
    {
        m_bpostEnable = false;
        LOG(LOG_ERROR,"停止Post");
    }
}

void HttpPost::slotReceivePostFailed()
{
    if(m_bqurlState)
    {
        //LOG(LOG_ERROR,"重新启动Post");
    }
}

void HttpPost::slotTimeOut()
{
    QByteArray data;
    if(m_bpostEnable)
    {
        PostData( data,WARNING_RESULT );

//        QNetworkRequest request;
//        request.setUrl(m_qurl);
//        request.setHeader(QNetworkRequest::ContentTypeHeader, m_targetData);
//        m_manager->post(request, m_targetData);
    }
}

void HttpPost::slotFinishedReply(QNetworkReply *reply)
{
    //qDebug()<<"child ThreadID: "<<QThread::currentThreadId()<<endl;
    reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if(reply->error() == QNetworkReply::NoError)
    {

        QByteArray bytes = reply->readAll();
        if( !bytes.isEmpty() )
        {
            LOG(LOG_RTM,"Channel[0x%x]HttpPost::slotFinishedReply retcode=%d,receive=%s",
               this,reply->error(),bytes.toStdString().c_str());
            m_targetParse->Parse(bytes, m_currentAnalyticsResult);
            if( m_pFun )
            {
//                m_currentAnalyticsResult.nUserGroup = m_warinfo.nUserGroup;
//                m_currentAnalyticsResult.rtspSrc = m_warinfo.szRtspUrl;
//                LOG(LOG_RTM,"Channel[0x%x]HttpPost will callback url=%s,usergroup=%d,objsize=%d,screenw=%d,screenh=%d",
//                    this,m_currentAnalyticsResult.rtspSrc.toStdString().c_str(),
//                    m_currentAnalyticsResult.nUserGroup,
//                    m_currentAnalyticsResult.targetList.size(),
//                    m_currentAnalyticsResult.sceneWidth,
//                    m_currentAnalyticsResult.sceneHeight);

//                m_pFun( m_currentAnalyticsResult,m_pUser );
            }

        }

    }
    else
    {
        emit signalPostFaild();
        //LOG(LOG_ERROR,"post reply error ");
    }

    reply->deleteLater();
}

bool HttpPost::ReadHttpQurl()
{

//    m_qurl = QString::fromLocal8Bit(m_warinfo.szObjRequestUrl);
//    char szMsg[ MAX_HTTP_POST_STR_LEN ] = { 0 };
//    sprintf_s(szMsg,"%s=%s&%s=%s&%s=%s",MESSAGETYPESTR,MESSAGEVALUESTR,MESSAGEURLSTR,
//              m_warinfo.szObjRequestType,m_warinfo.szTargetResult);
//    m_targetData.append(QString::fromLocal8Bit(szMsg));

//    m_freq = m_warinfo.nObjRequestFreq;

//    LOG(LOG_RTM,"Channel[%x]HttpPost::ReadHttpQurl url=%s!",this,m_qurl.toString().toStdString().c_str());
//    LOG(LOG_RTM,"Channel[%x]HttpPost::ReadHttpQurl requesturl(db)=%s!",this,m_warinfo.szObjRequestUrl);
//    LOG(LOG_RTM,"Channel[%x]HttpPost::ReadHttpQurl targitdata=%s!",this,m_targetData.constData());

    return true;
  }
void HttpPost::PostData( QByteArray & strVal ,WARN_POST_REQUEST_TYPE t )
{

    QByteArray data;
    char szChannelID[MAX_HTTP_POST_STR_LEN] = { 0 };
//    sprintf_s(szChannelID,MAX_HTTP_POST_STR_LEN,"%d",m_warinfo.nDBID);

    data.append(QString::fromLocal8Bit(MESSAGETYPESTR));
    data.append(QString::fromLocal8Bit("="));
    if( WARNING_LINE == t )
    {
        data.append(QString::fromLocal8Bit(MESSAGE_WARNLINE));
    }
    else if ( WARNING_ZONE == t )
    {
       data.append(QString::fromLocal8Bit(MESSAGE_WARNZONE));
    }
    else
    {
       data.append(QString::fromLocal8Bit(MESSAGE_WARNRESULT));
    }
    data.append(QString::fromLocal8Bit("&"));
    data.append(QString::fromLocal8Bit(MESSAGEVALUESTR));
    data.append(QString::fromLocal8Bit("="));
    data.append(strVal);

    data.append(QString::fromLocal8Bit("&"));
    data.append(QString::fromLocal8Bit(MESSAGECHANSTR));
    data.append(QString::fromLocal8Bit("="));
    data.append(szChannelID);

    data.append(QString::fromLocal8Bit("&"));
    data.append(QString::fromLocal8Bit(MESSAGEURLSTR));
    data.append(QString::fromLocal8Bit("="));
    //data.append(QString::fromLocal8Bit(m_warinfo.szRtspUrl));

    QNetworkRequest request;
 //   request.setUrl(QString::fromLocal8Bit(m_warinfo.szObjRequestUrl));
    request.setHeader(QNetworkRequest::ContentTypeHeader, data);
    m_manager->post(request, data);
    LOG(LOG_RTM,"Channel[0x%x]HttpPost::PostData RequestUrl=[%s],RequestData=[%s]",
        this,m_qurl.toString().toStdString().c_str(),data.constData());

}
}

