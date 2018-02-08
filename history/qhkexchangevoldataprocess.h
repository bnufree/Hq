#ifndef QHKEXCHANGEVOLDATAPROCESS_H
#define QHKEXCHANGEVOLDATAPROCESS_H

#include <QRunnable>
#include <QDate>

struct HKVOLDATA{
    int mCode;
    qint64  mVol;
};
class QHKExchangeVolDataProcess : public QRunnable
{
public:
    explicit QHKExchangeVolDataProcess(const QDate& date);
    ~QHKExchangeVolDataProcess() {}

public:
    void run();
    void getMktVolInfo(QList<HKVOLDATA>& list, const QDate& date, int mkt);
private:
    QDate       mDate;
};

#endif // QHKEXCHANGEVOLDATAPROCESS_H
