#ifndef QHQEASTMONEYREALINFOTHREAD_H
#define QHQEASTMONEYREALINFOTHREAD_H

#include <QThread>

class QHqEastMoneyRealInfoThread : public QThread
{
    Q_OBJECT
public:
    explicit QHqEastMoneyRealInfoThread(QObject *parent = 0);
protected:
    void run();
    void  parseHttpUrl(const QString& url);

signals:

public slots:
};

#endif // QHQEASTMONEYREALINFOTHREAD_H
