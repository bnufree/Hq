#ifndef QSHAREFINANCIALINFOWORK_H
#define QSHAREFINANCIALINFOWORK_H

#include <QObject>
#include <QRunnable>


class QShareFinancialInfoWork:public QRunnable
{
public:
    explicit QShareFinancialInfoWork(QObject *parent = 0);
    ~QShareFinancialInfoWork();
    void        setShareCodeList(const QStringList& list) {mShareCodeList = list;}
    void run();

private:
    QObject*        mParent;
    QStringList     mShareCodeList;
};

#endif // QSHAREFINANCIALINFOWORK_H
