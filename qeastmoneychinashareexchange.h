#ifndef QEASTMONEYCHINASHAREEXCHANGE_H
#define QEASTMONEYCHINASHAREEXCHANGE_H

#include <QThread>

class QEastMoneyChinaShareExchange : public QThread
{
    Q_OBJECT
public:
    explicit QEastMoneyChinaShareExchange(QObject *parent = 0);
    ~QEastMoneyChinaShareExchange();
protected:
    void run();

signals:

public slots:
};

#endif // QEASTMONEYCHINASHAREEXCHANGE_H
