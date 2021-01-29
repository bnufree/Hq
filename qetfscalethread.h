#ifndef QETFSCALETHREAD_H
#define QETFSCALETHREAD_H

#include <QThread>
#include <QMap>
#include <QDate>

struct EtfScaleData{
    QString         mCode;
    double          mScale;
    QString         mName;
    QDate           mDate;
};

class QEtfScaleThread : public QThread
{
    Q_OBJECT
public:
    explicit QEtfScaleThread(QObject *parent = 0);
    ~QEtfScaleThread();

    void run();
    QList<EtfScaleData> getAllEtfScales() const {return mEtfVolMap.values();}
private:
    void parseShEtf(const QDate& date);
    void parseSzEtf(const QDate& date);
    void parseShEtfScaleChange(const QString& code);
    QMap<QString, EtfScaleData>       mEtfVolMap;

signals:
    void    signalSendAllEtfScaleDataList(const QList<EtfScaleData>& list);
public slots:
};

#endif // QETFSCALETHREAD_H
