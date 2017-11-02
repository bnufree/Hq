#ifndef QHTTPGET_H
#define QHTTPGET_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>

class QHttpGet : public QObject
{
    Q_OBJECT
public:
    explicit QHttpGet(const QString& url = QString(), bool sequential = false, QObject *parent = 0);
    ~QHttpGet();
    void setUrl(const QString& url);
    void startGet();
    void setUpdateInterval(int secs);           //刷新间隔
signals:
    void signalSendHttpConent(const QByteArray& content);
    void signalErrorOccured(QNetworkReply::NetworkError);
public slots:
    void    slotReadHttpContent();
    void    slotUpdateHttp();
    void    slotStartReadHttpContent();
private:
    QString     mUrl;
    QNetworkAccessManager   *mMgr;
    QNetworkReply           *mReply;
    QTimer      *mUpdateTimer;                  //刷新时间
    bool        mIsSequential;                  //连续刷新
    int         mInertVal;

};

#endif // QHTTPGET_H
