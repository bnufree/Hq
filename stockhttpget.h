#ifndef STOCKHTTPGET_H
#define STOCKHTTPGET_H

#include <QList>
#include "stockdata.h"
#include "basehttpget.h"

typedef     QList<StockData>        StockDataList;

typedef enum tagMarketStyle
{
    MARKET_HS = 0,
    MARKET_SH,
    MARKET_SZ,
    MARKET_ZXB,
    MARKET_CYB,
}MarketStyle;

typedef enum tagSortType
{
    SORT_PRICE = 0,
    SORT_ZHANGDIEFU,
    SORT_ZHANGDIE,
    SORT_VOL,
    SORT_MONEY,
    SORT_ZHENFU
}SortType;

typedef enum tagHqMode
{
    HQ_ZXG = 0,
    HQ_STOCKCENTER,
}HqMode;



typedef void(*HTTPGETCBKFUN)( StockDataList &pDataList,void *pUser );

class StockHttpGet:public BaseHttpGet
{
    Q_OBJECT
public:
    ~StockHttpGet();
     explicit StockHttpGet(const QString& pUrl, QObject* parent = 0);
    void SetCbkFun( HTTPGETCBKFUN pFun,void *pUser );
    void SetSortStyle(QString style);
    void SetSortType(int type);
    void SetSortRule(int rule);
    void SetHqMode(int mode);
    int  GetSortType();
    int  GetSortRule();
    QString  GetSortStyle();
    int  GetHqMode();
    void ModifyUrl(int type, int rule);

public slots:
    void slotFinishedReply(QNetworkReply* reply);
private:
    HTTPGETCBKFUN       mFunc;
    void                *mUser;
    QString             mSortStyle;
    int                 mSortType;
    int                 mSortRule; //-1降序；1升序
    int                 mHqMode; //0板块；1股票
};
#endif // STOCKHTTPGET_H
