#ifndef QSINASHAREVOLINFOTHREAD_H
#define QSINASHAREVOLINFOTHREAD_H

#include <QThread>
#include <QMap>

class StockData;
class QSinaShareVolInfoThread : public QThread
{
    Q_OBJECT
public:
    explicit QSinaShareVolInfoThread(const QStringList& codes, QObject *parent = 0);
protected:
    void run2();
    void run();
    void updateFHSPInfoWithDate(QMap<QString, StockData>& map, const QString &date);
signals:

public slots:
private:
    QStringList mShareCodesList;
};

#endif // QSINASHAREVOLINFOTHREAD_H
