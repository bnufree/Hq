#ifndef QEASTMONEYCHINASHAREEXCHANGE_H
#define QEASTMONEYCHINASHAREEXCHANGE_H

#include <QThread>
#include <QDate>
#include "dbservices/dbservices.h"

class QEastMoneyChinaShareExchange : public QThread
{
    Q_OBJECT
public:
    explicit QEastMoneyChinaShareExchange(const QDate& pDate, QObject *parent = 0);
    ~QEastMoneyChinaShareExchange();
protected:
    void run();
    void getHGTTop10Share(QList<ChinaShareExchange>& list, const QDate& date);

signals:
    void signalTop10Exchangers(const QList<ChinaShareExchange>& list);
    void signalHSGTofTodayTop10Updated();

public slots:
private:
    QDate   mWorkDate;

};

#endif // QEASTMONEYCHINASHAREEXCHANGE_H
