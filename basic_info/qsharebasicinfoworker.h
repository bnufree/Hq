#ifndef QSHAREBASICINFOWORKER_H
#define QSHAREBASICINFOWORKER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include "data_structure/sharedata.h"

class QShareBasicInfoWorker : public QObject
{
    Q_OBJECT
public:
    explicit QShareBasicInfoWorker(QObject *parent = 0);


signals:
    void signalGetBasicInfo();
    void signalBaseDataListFinished(const QStringList& codes, const ShareDataList& list);
    void signalUpdateFavCode(const QString& code);
public slots:
    void slotGetBasicInfo();
    void slotUpdateShareCodesList(const ShareDataList& list);
    void slotUpdateShareFHSPList(const ShareDataList &list);
    void slotUpdateShareHsgtTop10List(const ShareDataList &list);
    void slotUpdateShareFinancialList(const ShareDataList &list);
    void updateShareFavCode(const QString& code);
private:
    bool getInfosFromFile(ShareDataMap& map);
    bool getInfossFromWeb(ShareDataMap& map);
    bool writeInfos(const ShareDataList& list);

private:
    QThread             mWorkThread;
    ShareDataMap       mShareBaseDataMap;
    QMutex      mUpdateMutex;
    bool        mNeedUpdateCurrentTop10;

};

#endif // QSHAREBASICINFOWORKER_H
