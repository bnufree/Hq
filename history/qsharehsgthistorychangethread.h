#ifndef QSHAREHSGTHISTORYCHANGETHREAD_H
#define QSHAREHSGTHISTORYCHANGETHREAD_H

#include <QObject>
#include <QMap>
#include "data_structure/sharedata.h"

enum HSGT_CHG_MODE{
    HSGT_CHG_NONE = -1,
    HSGT_CHG_1DAY = 0,
    HSGT_CHG_3DAY,
    HSGT_CHG_5DAY,
    HSGT_CHG_10DAY,
    HSGT_CHG_MONTH,
    HSGT_CHG_SEASON,
    HSGT_CHG_YEAR,
};


class QShareHSGTHistoryChangeThread : public QObject
{
    Q_OBJECT
public:
    explicit QShareHSGTHistoryChangeThread(HSGT_CHG_MODE mode = HSGT_CHG_NONE, QObject *parent = 0);
    void run();

signals:

public slots:
private:
    HSGT_CHG_MODE       mMode;
};

#endif // QSHAREHSGTHISTORYCHANGETHREAD_H
