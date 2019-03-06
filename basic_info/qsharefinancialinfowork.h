#ifndef QSHAREFINANCIALINFOWORK_H
#define QSHAREFINANCIALINFOWORK_H

#include <QObject>
#include <QRunnable>


class QShareFinancialInfoWork:public QRunnable
{
public:
    explicit QShareFinancialInfoWork(QObject *parent = 0);
    ~QShareFinancialInfoWork();
    void run();

private:
    QObject*        mParent;
};

#endif // QSHAREFINANCIALINFOWORK_H
