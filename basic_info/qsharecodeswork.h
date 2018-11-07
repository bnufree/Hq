#ifndef QSHARECODESWORK_H
#define QSHARECODESWORK_H

#include <QRunnable>
#include <QObject>
#include <QThread>

#ifdef HQ_RUNNABLE
class QShareCodesWork : public QRunnable
{
public:
    explicit QShareCodesWork(QObject *parent = 0);
    ~QShareCodesWork();
    void run();
private:
    QObject* mParent;
};
#else
class QShareCodesWork : public QThread
{
    Q_OBJECT
public:
    explicit QShareCodesWork(QObject *parent = 0);
    ~QShareCodesWork();
    void run();
private:
    QObject* mParent;
};
#endif

#endif // QSHARECODESWORK_H
