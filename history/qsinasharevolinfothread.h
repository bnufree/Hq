#ifndef QSINASHAREVOLINFOTHREAD_H
#define QSINASHAREVOLINFOTHREAD_H

#include <QThread>

class QSinaShareVolInfoThread : public QThread
{
    Q_OBJECT
public:
    explicit QSinaShareVolInfoThread(const QStringList& codes, QObject *parent = 0);
protected:
    void run();

signals:

public slots:
private:
    QStringList mShareCodesList;
};

#endif // QSINASHAREVOLINFOTHREAD_H
