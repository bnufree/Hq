#ifndef QETFSCALETHREAD_H
#define QETFSCALETHREAD_H

#include <QThread>

struct EtfScaleData{
    QString         mCode;
    qint64          mScale;
};

class QEtfScaleThread : public QThread
{
    Q_OBJECT
public:
    explicit QEtfScaleThread(QObject *parent = 0);

    void run();

signals:
    void    signalSendAllEtfScaleDataList(const QList<EtfScaleData>& list);
public slots:
};

#endif // QETFSCALETHREAD_H
