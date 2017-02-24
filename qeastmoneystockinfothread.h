#ifndef QEASTMONEYSTOCKINFOTHREAD_H
#define QEASTMONEYSTOCKINFOTHREAD_H

#include <QThread>
#include <QStringList>
class QDate;

class qeastmoneystockinfothread : public QThread
{
    Q_OBJECT
public:
    explicit qeastmoneystockinfothread(QThread *parent = 0);
    ~qeastmoneystockinfothread();
    void    setStockCodeList(const QStringList& codes);
    bool    isActiveDay(QDate date);
    QDate   lastActiveDay();
protected:
    void run();

signals:
    void    sendMktCapInfo(const QString& code, qint64 totala, qint64 lta);
    void    sendUpdateProgress(int cur, int total);
public slots:
private:
    QStringList         mStockcodeList;
};

#endif // QEASTMONEYSTOCKINFOTHREAD_H


//http://nufm.dfcfw.com/EM_Finance2014NumericApplication/JS.aspx?type=CT&cmd=P.[(x)]|3000592&sty=MPICT&st=z&sr=&p=&ps=&cb=callback&js=&token=aaf32a88de888ea7b0ea63e017ecb049&_=1481245370248
