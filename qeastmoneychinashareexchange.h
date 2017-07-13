#ifndef QEASTMONEYCHINASHAREEXCHANGE_H
#define QEASTMONEYCHINASHAREEXCHANGE_H

#include <QThread>
#include <QDate>
#include "hqdatadefines.h"

class QEastMoneyChinaShareExchange : public QThread
{
    Q_OBJECT
public:
    explicit QEastMoneyChinaShareExchange(const QDate& pDate, QObject *parent = 0);
    ~QEastMoneyChinaShareExchange();
protected:
    void run();

signals:
    void signalTop10Exchangers(const QList<ChinaShareExchange>& list);

public slots:
private:
    QDate   mWorkDate;

};

#endif // QEASTMONEYCHINASHAREEXCHANGE_H
