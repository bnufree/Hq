#ifndef QSINASEARCHTHREAD_H
#define QSINASEARCHTHREAD_H

#include <QThread>
#include "qhttpget.h"

class QSinaSearchThread : public QObject
{
    Q_OBJECT
public:
    explicit QSinaSearchThread(QObject *parent = 0);
    ~QSinaSearchThread();
private slots:
    void slotRecvSearchString(const QString& text);
    void slotRecvHttpContent(const QByteArray& bytes);

signals:
    void sendSearchResult(const QStringList& list);
    void signalSetSearchString(const QString& text);
public slots:

private:
    QThread mWorkThread;
    QHttpGet    *mHttp;
};

#endif // QSINASEARCHTHREAD_H
