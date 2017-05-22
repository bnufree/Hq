#ifndef HQDATADEFINES
#define HQDATADEFINES

#include <QString>
#include <QStringList>
#include <QDate>
#include "stockdata.h"

struct BlockBaseData{
    int                 mCode;
    int                 mType;              //
    QString             mName;
    QStringList         mShareCodesList;
};

struct BlockRealInfo:public BlockBaseData{
    double              mCurPrice;
    double              mChange;
    double              mChangePercent;
    double              mZjlx;
    QDate               mDate;
};

class ChinaShareExchange:public StockData
{
public:
    QDate               mDate;
    double              mTop10Buy;
    double              mTop10Sell;
    double              mTop10Vol;
    double              mTop10TotalVol;
    double              mTop10TotalCap;
    double              mTop10AvgCost;
};

#endif // HQDATADEFINES

