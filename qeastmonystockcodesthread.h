#ifndef QEASTMONYSTOCKCODESTHREAD_H
#define QEASTMONYSTOCKCODESTHREAD_H

#include <QThread>

class QEastMonyStockCodesThread : public QObject
{
    Q_OBJECT
public:
    explicit QEastMonyStockCodesThread(QObject *parent = 0);
    ~QEastMonyStockCodesThread();

protected:
    bool getCodesFromFile(QStringList& codes);
    void getCodesFromURL(QStringList& list, const QString& URL);
    bool writeCodes(const QStringList& codes);


signals:
    void start();
    void signalSendCodesList(const QStringList& list);

public slots:
    void run();
    void slotRecvAllCodes(const QStringList& list);
    void slotDBInitFinished();
private:

private:
    QThread     mThread;
    QStringList mCodesList;
};

#endif // QEASTMONYSTOCKCODESTHREAD_H
