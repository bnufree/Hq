#ifndef QEASTMONEYSHAREFHSPTHREAD_H
#define QEASTMONEYSHAREFHSPTHREAD_H

#include <QObject>

class QEastMoneyShareFHSPThread : public QObject
{
    Q_OBJECT
public:
    explicit QEastMoneyShareFHSPThread(QObject *parent = 0);
    ~QEastMoneyShareFHSPThread();

signals:

public slots:
    void updateFHSPFiles();
    void updateFHSPInfoWithDate(const QString& date);
};

#endif // QEASTMONEYSHAREFHSPTHREAD_H
