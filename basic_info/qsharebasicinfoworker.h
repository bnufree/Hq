#ifndef QSHAREBASICINFOWORKER_H
#define QSHAREBASICINFOWORKER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include "utils/sharedata.h"

class QShareBasicInfoWorker : public QObject
{
    Q_OBJECT
public:
    explicit QShareBasicInfoWorker(QObject *parent = 0);


signals:
    void signalGetBasicInfo();
    void signalBaseDataListFinished(const QStringList& codes, const ShareBaseDataList& list);
    void signalUpdateFavCode(const QString& code);
public slots:
    void slotGetBasicInfo();
    void slotUpdateShareCodesList(const ShareBaseDataList& list);
    void slotUpdateShareFHSPList(const ShareBaseDataList &list);
    void slotUpdateShareHsgtTop10List(const ShareBaseDataList &list);
    void slotUpdateShareFinancialList(const ShareBaseDataList &list);
    void updateShareFavCode(const QString& code);
private:
    bool getInfosFromFile(QMap<QString, ShareBaseData>& map);
    bool getInfossFromWeb(QMap<QString, ShareBaseData>& map);
    bool writeInfos(const ShareBaseDataList& list);
private:
    QThread     mWorkThread;
    QMap<QString, ShareBaseData>        mShareBaseDataMap;
    QMutex      mUpdateMutex;
    bool        mNeedUpdateCurrentTop10;

};

#endif // QSHAREBASICINFOWORKER_H
