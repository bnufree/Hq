#ifndef QSHAREHISTORYINFOMGR_H
#define QSHAREHISTORYINFOMGR_H

#include <QObject>

class QShareHistoryInfoMgr : public QObject
{
    Q_OBJECT
public:
    explicit QShareHistoryInfoMgr(QObject *parent = 0);

signals:

public slots:
    void startGetHistoryInfo(const QStringList& list);
};

#endif // QSHAREHISTORYINFOMGR_H
