#ifndef QEXCHANGEDATAMANAGE_H
#define QEXCHANGEDATAMANAGE_H

#include <QObject>
#include <QDate>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QTime>
class QExchangeDataManage : public QObject
{
    Q_OBJECT
public:
    ~QExchangeDataManage();
    static QExchangeDataManage *instance();
    void AddDay(const QDate& date);
    void RemoveDay(const QDate& date);
    bool IsActiveDay(const QDate& date);
    QDate GetLatestActiveDay(const QDate& date);
    QStringList GetUnExchangeDays();
    bool IsNeedUpdate(const QDate& last, const QDate& cur);
    bool IsNeedUpdate(const QDateTime& time);

private:
    explicit QExchangeDataManage(QObject *parent = 0);
private:
    static QExchangeDataManage     *minstance;
    class QExchangeDataDeleter
    {
    public:
        ~QExchangeDataDeleter()
        {
            if (QExchangeDataManage::minstance)
                delete QExchangeDataManage::minstance;
        }
    };
    static QExchangeDataDeleter delobj;

signals:

public slots:
private:
    QStringList         mUnexhangeDayList;
};

#endif // QEXCHANGEDATAMANAGE_H
