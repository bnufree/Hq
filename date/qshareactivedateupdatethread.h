#ifndef QSHAREACTIVEDATEUPDATETHREAD_H
#define QSHAREACTIVEDATEUPDATETHREAD_H

#include <QThread>
class QShareActiveDateUpdateThread : public QThread
{
    Q_OBJECT
public:
    explicit QShareActiveDateUpdateThread( QObject *parent = 0);
public:
    void run();

private:
    QList<QDate> getDateListFromNetease();
    QList<QDate> getDateListFromHexun();
    QList<QDate> getCurrentYearClosedDateList();

signals:
    void signalNewWorkDateNow();
    void signalUpdateHistoryWorkDays();
    void signalSystemStatus(qint64 time, int sts);
    void signalUpdateHsgtTop10();
    void signalSaveTodayShareHistory();
    void signalRestartMe();

private:

public slots:
};

#endif // QSHAREACTIVEDATEUPDATETHREAD_H
