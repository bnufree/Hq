#ifndef QSHAREACTIVEDATEUPDATETHREAD_H
#define QSHAREACTIVEDATEUPDATETHREAD_H

#include <QThread>
class QShareActiveDateUpdateThread : public QThread
{
    Q_OBJECT
public:
    explicit QShareActiveDateUpdateThread(QObject* dest, QObject *parent = 0);
public:
    void run();

private:
    QList<QDate> getDateListFromNetease();
    QList<QDate> getDateListFromHexun();

signals:
    void signalNewWorkDateNow();
    void signalUpdateHistoryWorkDays();
    void signalSystemStatus(qint64 time, int sts);

private:
    QObject*        mDest;

public slots:
};

#endif // QSHAREACTIVEDATEUPDATETHREAD_H
