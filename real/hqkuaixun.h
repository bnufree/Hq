#ifndef HQKUAIXUN_H
#define HQKUAIXUN_H

#include <QThread>
#include "data_structure/hqutils.h"

class HqKuaixun : public QThread
{
    Q_OBJECT
public:
    explicit HqKuaixun(QObject *parent = 0);
    void run();
private:
    void parseEastMoney(KuaiXunList& list);
    void parseThs(KuaiXunList& list);
    QList<LinkData> getLinkeDataList(const QString& digest);
signals:
    void signalSendKuaiXun(const KuaiXunList& list);
public slots:
};

#endif // HQKUAIXUN_H
