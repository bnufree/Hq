#ifndef QSHAREHSGTTOP10WORK_H
#define QSHAREHSGTTOP10WORK_H

#include <QObject>
#include <QRunnable>
#include "data_structure/sharedata.h"

class QShareHsgtTop10Work :  public QObject,public QRunnable
{
    Q_OBJECT
public:
    explicit QShareHsgtTop10Work(const QString& date, QObject *parent = 0);
    ~QShareHsgtTop10Work();
    void run();
signals:
    void signalChinaAShareTop10Updated(const ShareDataList& list, const QString& date);

private:
    QObject*        mParent;
    QString         mDate;
    QString         mFileName;
};

#endif // QSHAREHSGTTOP10WORK_H
