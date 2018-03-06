#ifndef QSHAREBASICINFOWORKER_H
#define QSHAREBASICINFOWORKER_H

#include <QObject>
#include <QThread>

class ShareDataList;
class QShareBasicInfoWorker : public QObject
{
    Q_OBJECT
public:
    explicit QShareBasicInfoWorker(QObject *parent = 0);

signals:
    void signalGetBasicInfo();
public slots:
    void slotGetBasicInfo();
    void slotUpdateShareCodesList(const BaseDataList& list);
private:
    bool getInfosFromFile(BaseDataList& list);
    bool getInfossFromWeb(BaseDataList& list);
    bool writeInfos(const BaseDataList& list);
private:
    QThread     mWorkThread;
    BaseDataList       mShareDataList;
};

#endif // QSHAREBASICINFOWORKER_H
