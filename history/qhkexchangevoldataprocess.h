#ifndef QHKEXCHANGEVOLDATAPROCESS_H
#define QHKEXCHANGEVOLDATAPROCESS_H

#include <QRunnable>
#include <QDate>
#include <QObject>
#include "data_structure/sharedata.h"
#include <QMap>


class QHKExchangeVolDataProcess : public QObject, public QRunnable
{
    Q_OBJECT
public:

    enum FetchMode{
        Fetch_Only_File = 0x01,
        Fetch_Only_Web = 0x02,
        Fetch_Only_Local = Fetch_Only_File,
        Fetch_All = Fetch_Only_Local | Fetch_Only_Web,
    };

    explicit QHKExchangeVolDataProcess(const QDate& date, FetchMode mode = Fetch_All, QObject* parent = 0);
    ~QHKExchangeVolDataProcess() {}

public:
    void run();

signals:
    void signalSendDataList(const ShareForignVolFileDataList& list, const QDate& date);

private:
    void getVolofDate(ShareForignVolFileDataList& list, const QDate& date);
    void getVolInfoFromHKEX(ShareForignVolFileDataList& list,int& num, const QDate& date, int mkt = 0);
    bool getVolInfoFromFile(ShareForignVolFileDataList& list, const QDate& date);
    bool getVolInfoFromEastMoney(ShareForignVolFileDataList& list, const QDate& date);
    bool saveData(const QDate& date, const ShareForignVolFileDataList& list);
    QString getFileName(const QDate& date);
private:
    QDate       mDate;
    QObject*    mParent;
    FetchMode   mMode;
};

#endif // QHKEXCHANGEVOLDATAPROCESS_H
