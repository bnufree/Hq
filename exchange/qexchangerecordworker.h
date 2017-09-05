#ifndef QEXCHANGERECORDWORKER_H
#define QEXCHANGERECORDWORKER_H

#include <QObject>

class QExchangeRecordWorker : public QObject
{
    Q_OBJECT
public:
    explicit QExchangeRecordWorker(QObject *parent = 0);

signals:

public slots:
};

#endif // QEXCHANGERECORDWORKER_H