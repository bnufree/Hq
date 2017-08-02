#ifndef QSHAREHISTORYINFOMGR_H
#define QSHAREHISTORYINFOMGR_H

#include <QObject>
#include <QThread>

class QShareHistoryInfoMgr : public QObject
{
    Q_OBJECT
public:
    explicit QShareHistoryInfoMgr(QObject *parent = 0);

signals:
    void singalSetHistoryCodeList(const QStringList& list);
    void signalStartGetHistory();

public slots:
    void slotSetHistoryCodeList(const QStringList& list);
    void slotRecvCodeHistoryDate(const QString& code, const QDate& date);
    void slotSubThreadFinish();
    void slotStartGetHistory();
private:
    QThread     mWorkThread;
    QList<QThread*>     mSubThreadList;
    QStringList         mCodesList;
};

#endif // QSHAREHISTORYINFOMGR_H
