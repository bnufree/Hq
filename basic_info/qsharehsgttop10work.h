#ifndef QSHAREHSGTTOP10WORK_H
#define QSHAREHSGTTOP10WORK_H

#include <QObject>
#include <QRunnable>

class QShareHsgtTop10Work : public QRunnable
{
public:
    explicit QShareHsgtTop10Work(const QString& date, QObject *parent = 0);
    ~QShareHsgtTop10Work();
    void run();

private:
    QObject*        mParent;
    QString         mDate;
};

#endif // QSHAREHSGTTOP10WORK_H
