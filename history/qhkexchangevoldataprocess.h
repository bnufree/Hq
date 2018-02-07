#ifndef QHKEXCHANGEVOLDATAPROCESS_H
#define QHKEXCHANGEVOLDATAPROCESS_H

#include <QObject>

//\d{2}/\d{2}/\d{4}
class QHKExchangeVolDataProcess : public QObject
{
    Q_OBJECT
public:
    explicit QHKExchangeVolDataProcess(QObject *parent = 0);

signals:

public slots:
private:
    int         mMktType;
    //QDate       mDate;
};

#endif // QHKEXCHANGEVOLDATAPROCESS_H
