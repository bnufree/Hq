#ifndef BLOCKHTTPGET_H
#define BLOCKHTTPGET_H

#include <QList>
#include "blockdata.h"
#include "basehttpget.h"

typedef     QList<BlockData>        BlockDataList;

typedef enum tagBlockType
{
    BLOCK_HY = 0,
    BLOCK_GN,
    BLOCK_DQ
}BlockType;


typedef void(*BLOCKGETCBKFUN)( BlockDataList &pDataList,void *pUser );

class BlockHttpGet:public BaseHttpGet
{
    Q_OBJECT
public:
    ~BlockHttpGet();
     explicit BlockHttpGet(const QString& pUrl, QObject* parent = 0);
    void SetCbkFun( BLOCKGETCBKFUN pFun,void *pUser );
    void SetSortType(int type);
    void SetSortRule(int rule);
    int  GetSortType();
    int  GetSortRule();
    void ModifyUrl(int type);

public slots:
    void slotFinishedReply(QNetworkReply* reply);
private:
    BLOCKGETCBKFUN       mFunc;
    void                *mUser;
    int                 mSortType;
    int                 mSortRule; //-1降序；1升序
};

#endif // BLOCKHTTPGET_H
