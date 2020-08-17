#ifndef ANNUALCLOSEDDAYDISCLOSURE_H
#define ANNUALCLOSEDDAYDISCLOSURE_H

#include <QThread>

#include <QDate>

class AnnualClosedDayDisclosure : public QThread
{
    Q_OBJECT
public:
    explicit AnnualClosedDayDisclosure( bool async, QObject *parent = 0);
    QList<QDate>        getClosedDateList() const {return mDateList;}

    void run();

signals:
    void    signalSendClosedDateList(const QList<QDate>& list);

public slots:

private:
    QList<QDate>        mDateList;
    bool                mAsyncSend;
};

#endif // ANNUALCLOSEDDAYDISCLOSURE_H
