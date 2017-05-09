#ifndef HQDATADEFINES
#define HQDATADEFINES

#include <QString>
#include <QStringList>
#include <QDate>

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

#endif // HQDATADEFINES

