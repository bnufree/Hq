#ifndef QSINANORTHREALINFOTHREAD_H
#define QSINANORTHREALINFOTHREAD_H

#include <QThread>
#include <QDateTime>

struct NorthBoundData{
    double      sh_flow;
    double      sz_flow;
    double      total_flow;
    QDateTime   time;
};

class QSinaNorthRealInfoThread : public QThread
{
    Q_OBJECT
public:
    explicit QSinaNorthRealInfoThread(QObject *parent = 0);

public:
    void run();

signals:
    void    signalUpdateNorthBoundList(const QList<NorthBoundData> &list, int max, int min, const QDate& date);
public slots:
};

#endif // QSINANORTHREALINFOTHREAD_H
