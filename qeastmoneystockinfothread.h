#ifndef QEASTMONEYSTOCKINFOTHREAD_H
#define QEASTMONEYSTOCKINFOTHREAD_H

#include <QThread>
#include <QStringList>
class QDate;

class qeastmoneystockinfothread : public QThread
{
    Q_OBJECT
public:
    explicit qeastmoneystockinfothread(const QString& code, QObject *parent = 0);
    ~qeastmoneystockinfothread();
protected:
    void run();

signals:
    void    sendMktCapInfo(const QString& code, qint64 totala, qint64 lta);
    void    sendUpdateProgress(int cur, int total);
public slots:
    void    updateHistoryStkInfo();
private:
    QString         mCode;
};

#endif // QEASTMONEYSTOCKINFOTHREAD_H


//http://nufm.dfcfw.com/EM_Finance2014NumericApplication/JS.aspx?type=CT&cmd=P.[(x)]|3000592&sty=MPICT&st=z&sr=&p=&ps=&cb=callback&js=&token=aaf32a88de888ea7b0ea63e017ecb049&_=1481245370248
