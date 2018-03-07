#ifndef QSHAREBASICINFOWORKER_H
#define QSHAREBASICINFOWORKER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include "dbservices/sharedata.h"

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
    void slotUpdateShareCodesList(const ShareBaseDataList& list);
    void slotUpdateShareFHSPList(const ShareBaseDataList &list);
    void slotUpdateShareHsgtTop10List(const ShareBaseDataList &list);
private:
    bool getInfosFromFile(QMap<QString, ShareBaseData>& map);
    bool getInfossFromWeb(QMap<QString, ShareBaseData>& map);
    bool writeInfos(const ShareBaseDataList& list);
private:
    QThread     mWorkThread;
    QMap<QString, ShareBaseData>        mShareBaseDataMap;
    QMutex      mUpdateMutex;

};

#endif // QSHAREBASICINFOWORKER_H
