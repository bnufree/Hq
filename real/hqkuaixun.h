#ifndef HQKUAIXUN_H
#define HQKUAIXUN_H

#include <QThread>
#include "data_structure/hqutils.h"

class QWidget;

class HqKuaixun : public QThread
{
    Q_OBJECT
public:
    explicit HqKuaixun(QObject *parent = 0);
    void run();
    void setDisplayWidget(QObject* w) {mDisplayWidget = w;}
private:
    void parseEastMoney(KuaiXunList& list);
    void parseThs(KuaiXunList& list);
    QList<LinkData> getLinkeDataList(const QString& digest);
signals:
    void signalSendKuaiXun(const KuaiXunList& list);
public slots:
private:
    QObject*    mDisplayWidget;
};

#endif // HQKUAIXUN_H
