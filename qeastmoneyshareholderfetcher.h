#ifndef QEASTMONEYSHAREHOLDERFETCHER_H
#define QEASTMONEYSHAREHOLDERFETCHER_H

#include <QObject>
struct ShareHolder
{
    QString mShareCode;              //股票名称
    QString mShareName;
    QString mJigouCode;              //机构编号
    QString mJigouName;              //名称
    quint64 mVol;                    //数量
    double  mMutalPercent;          //流通比例
    double  mNetPricePercent;       //净值比例
};

class QEastMoneyShareHolderFetcher : public QObject
{
    Q_OBJECT
public:
    explicit QEastMoneyShareHolderFetcher(QObject *parent = nullptr);
    QList<ShareHolder>  getJigouDataOfShare(const QString& shareCode, const QDate& date);
    QList<ShareHolder>  getShareDataOfJigou(const QString& jigouCode, const QDate& date);

signals:

public slots:
private:
};

#endif // QEASTMONEYSHAREHOLDERFETCHER_H
