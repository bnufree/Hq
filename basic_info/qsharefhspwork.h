#ifndef QSHAREFHSPWORK_H
#define QSHAREFHSPWORK_H

#include <QObject>
#include <QRunnable>

class QShareFHSPWork : public QRunnable
{
public:
    explicit QShareFHSPWork(const QString& date, QObject *parent = 0);
    ~QShareFHSPWork();
    void run();

private:
    QObject*        mParent;
    QString         mDate;
};

#endif // QSHAREFHSPWORK_H
