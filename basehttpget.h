#ifndef BASEHTTPGET_H
#define BASEHTTPGET_H

#include <QObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QMutex>

class BaseHttpGet : public QObject
{
    Q_OBJECT
public:

    explicit BaseHttpGet(const QString& pUrl, QObject* parent = 0);
    virtual ~BaseHttpGet();

   //Ìí¼Ó·ÀÇøPost
   void SetUrl(const QString& pUrl);
   QString GetUrl();
   void SetFreq(int pFreq);
   int  GetFreq();
   void startRequest();
public slots:
   void slotTimeOut();
   virtual void slotFinishedReply(QNetworkReply* reply);
   virtual void slotReceiveGetFailed();
  // void slotGetReply();

signals:
   void signalGetFaild();
private:
   QUrl                     mUrl;  //httpµØÖ·
   int                      mFreq;
   QNetworkAccessManager*   mManager;
   QTimer*                  mTimer;
   QMutex                   mRepluMutex;
   QNetworkReply*           mReply;
};

#endif // BASEHTTPGET_H
