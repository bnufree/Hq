#ifndef QHTTPGET_H
#define QHTTPGET_H

#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>

class QHttpGet : public QThread
{
    Q_OBJECT
public:
    explicit QHttpGet(const QString& url = QString(), bool sequential = false, QObject *parent = 0);
    ~QHttpGet();
    void setUrl(const QString& url);
    void setSequential(bool sts);
    //void startGet();
    void setUpdateInterval(int secs);           //刷新间隔
    QByteArray static getContentOfURL(const QString& url);
    QByteArray static getContentOfURLWithPost(const QString &url, const QByteArray& post);
signals:
    void signalSendHttpConent(const QByteArray& content);
    void signalErrorOccured(QNetworkReply::NetworkError);
protected:
    void run();
public slots:
    void    slotReadHttpContent();
    //void    slotUpdateHttp();
    void    slotStartReadHttpContent();
private:
    QString     mUrl;
    QNetworkAccessManager   *mMgr;
    QNetworkReply           *mReply;
    QTimer      *mUpdateTimer;                  //刷新时间
    bool        mIsSequential;                  //连续刷新
    int         mInertVal;
    QByteArray      mHttpContent;

};

#endif // QHTTPGET_H
