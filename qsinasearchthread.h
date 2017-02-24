#ifndef QSINASEARCHTHREAD_H
#define QSINASEARCHTHREAD_H

#include <QThread>

class QSinaSearchThread : public QThread
{
    Q_OBJECT
public:
    explicit QSinaSearchThread(QObject *parent = 0);
    ~QSinaSearchThread();
    void setSearchString(const QString& text);
protected:
    void run();

signals:
    void sendSearchResult(const QStringList& list);
public slots:

private:
    QString mSearchText;
};

#endif // QSINASEARCHTHREAD_H
