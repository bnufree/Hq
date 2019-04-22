#ifndef QSHAREFHSPWORK_H
#define QSHAREFHSPWORK_H

#include <QObject>
#include <QThread>

class QShareFHSPWork : public QThread
{
    Q_OBJECT
public:
    explicit QShareFHSPWork(QObject *parent = 0);
    ~QShareFHSPWork();
    void run();

private:
};

#endif // QSHAREFHSPWORK_H
