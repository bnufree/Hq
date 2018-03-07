#ifndef QSHARECODESWORK_H
#define QSHARECODESWORK_H

#include <QRunnable>
#include <QObject>

class QShareCodesWork : public QRunnable
{
public:
    explicit QShareCodesWork(QObject *parent = 0);
    ~QShareCodesWork();
    void run();
private:
    QObject* mParent;
};

#endif // QSHARECODESWORK_H
