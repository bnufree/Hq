#ifndef QEASTMONEYHSGTSHAREAMOUNT_H
#define QEASTMONEYHSGTSHAREAMOUNT_H

#include <QThread>

class QEastMoneyHSGTShareAmount : public QThread
{
    Q_OBJECT
public:
    explicit QEastMoneyHSGTShareAmount(QObject *parent = 0);

signals:
    void     signalAmountFinshedAtDate(const QString& date);

public slots:
protected:
    void run();
};

#endif // QEASTMONEYHSGTSHAREAMOUNT_H
