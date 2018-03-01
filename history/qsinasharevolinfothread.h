#ifndef QSINASHAREVOLINFOTHREAD_H
#define QSINASHAREVOLINFOTHREAD_H

#include <QThread>
#include <QMap>

struct FINANCE_DATA;
class QSinaShareVolInfoThread : public QThread
{
    Q_OBJECT
public:
    explicit QSinaShareVolInfoThread(const QStringList& codes, QObject *parent = 0);
protected:
    void run();
    void updateFHSPInfoWithDate(QMap<int, FINANCE_DATA>& map, const QString &date);
signals:

public slots:
private:
    QStringList mShareCodesList;
};

#endif // QSINASHAREVOLINFOTHREAD_H
