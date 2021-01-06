#ifndef QSHAREFOREIGNVOLCHANGECOUNTERTHREAD_H
#define QSHAREFOREIGNVOLCHANGECOUNTERTHREAD_H

#include <QThread>
#include <QRunnable>
#include "data_structure/sharedata.h"

class QShareForeignVolChangeCounterThread : public QThread
{
    Q_OBJECT
public:
    explicit QShareForeignVolChangeCounterThread(QObject *parent = nullptr);

protected:
    void run();
    void read();
    void write();
private:
    bool        getData(const QString& type, const QDate& date);

signals:
    void        signalSendDataList(const QList<ShareForeignVolCounter>&, const QString& );
private:
    QMap<int, ShareForeignVolCounter>       mDataList;
    QString                             mFileName;

};

#endif // QSHAREFOREIGNVOLCHANGECOUNTERTHREAD_H
