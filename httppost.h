#ifndef HTTPPOST_H
#define HTTPPOST_H

#include<QObject>
#include"dataservices_global.h"
#include<QNetworkRequest>
#include <QNetworkReply>
#include<QUrl>
#include<QNetworkAccessManager>
#include<QTimer>
#include"analyticsdataparse.h"
#include"AnalyticsDataStruct.h"
#include<QThread>
#include<Pgsinterface.h>
#include<fstream>

#define HTTPPOSTPTR Dataservices::HttpPost::instance()

typedef void(*HTTPPOSTCBKFUN)( AnalyticsDataStruct &s,void *puser );

namespace Dataservices {
typedef enum WARN_POST_REQUEST_TYPE
{
    WARNING_LINE,
    WARNING_ZONE,
    WARNING_RESULT
};
class DATASERVICES_EXPORT HttpPost : public QObject
{
    Q_OBJECT
public:
    ~HttpPost();
     explicit HttpPost(QObject* parent = 0);

    //添加防区Post
    void AddWarningRulePost();
    void SetCbkFun( HTTPPOSTCBKFUN pfun,void *pUser );
    void StartPost( /*CAMWARNCFGINFO &info*/ );
    void PostData( QByteArray & strVal ,WARN_POST_REQUEST_TYPE t );
public slots:

    void slotStopPost();
    void slotReceivePostFailed();
    void slotTimeOut();
    void slotFinishedReply(QNetworkReply* reply);

signals:

    void signalPostFaild();

private:

    bool ReadHttpQurl();

private:
    HTTPPOSTCBKFUN m_pFun;
    void           *m_pUser;
    QByteArray     m_targetData;
    QUrl           m_qurl;  //http地址
    bool           m_bqurlState;
    unsigned int   m_freq;
    //CAMWARNCFGINFO  m_warinfo;
    QNetworkAccessManager*  m_manager;
    QTimer*                 m_timer;
    AnalyticsDataParse*     m_targetParse;
    AnalyticsDataStruct     m_currentAnalyticsResult;
    QThread                 m_postthread;
    bool                    m_bpostEnable;
    bool                    m_bStart;
};

}
#endif // HTTPPOST_H
