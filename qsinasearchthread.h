#ifndef QSINASEARCHTHREAD_H
#define QSINASEARCHTHREAD_H

#include <QThread>

class QSinaSearchThread : public QObject
{
    Q_OBJECT
public:
    explicit QSinaSearchThread(QObject *parent = 0);
    ~QSinaSearchThread();
private slots:
    void slotRecvSearchString(const QString& text);

signals:
    void sendSearchResult(const QStringList& list);
    void signalSetSearchString(const QString& text);
public slots:

private:
    QThread mWorkThread;
};

#endif // QSINASEARCHTHREAD_H
