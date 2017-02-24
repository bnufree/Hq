#ifndef DATAEXCHANGE_H
#define DATAEXCHANGE_H

#include <QObject>

class DataExchange : public QObject
{
    Q_OBJECT
public:
    explicit DataExchange(QObject *parent = 0);
    ~DataExchange();

    static DataExchange* getInstance();

signals:

public slots:

private:
    static DataExchange* mInstance;
};

#endif // DATAEXCHANGE_H
