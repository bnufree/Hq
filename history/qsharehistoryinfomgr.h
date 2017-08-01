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

public slots:
    void slotSetHistoryCodeList(const QStringList& list);
    void slotRecvCodeHistoryDate(const QString& code, const QDate& date);
private:
    QThread     mWorkThread;
};

#endif // QSHAREHISTORYINFOMGR_H
