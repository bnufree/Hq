#ifndef QSHAREFINANCIALINFOWORK_H
#define QSHAREFINANCIALINFOWORK_H

#include <QObject>
#include <QThread>


class QShareFinancialInfoWork : public QThread
{
    Q_OBJECT
public:
    explicit QShareFinancialInfoWork(const QStringList& list, QObject *parent = 0);
    ~QShareFinancialInfoWork();
    void        setShareCodeList(const QStringList& list) {mShareCodeList = list;}
    void run();

private:
    QStringList     mShareCodeList;
};

#endif // QSHAREFINANCIALINFOWORK_H
