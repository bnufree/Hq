#ifndef HQBASEDATA_H
#define HQBASEDATA_H

#include <QString>

class HqBaseData
{
public:
    //定义基础的数据类型
    typedef enum data_type{
        UNDEFINED = 0x00,
        SHARE = 0x01,
        BLOCK = 0x02,
    }DATA_TYPE;

    HqBaseData();
    ~HqBaseData() {}
    HqBaseData(const HqBaseData& data);
    HqBaseData(const QString& code, const QString& name, const QString& py, int type, bool isFav);
    HqBaseData& operator =(const HqBaseData& other);
    void setFav(bool fav) { mIsFav = fav;}
    bool isFav() const {return mIsFav;}
    void setCode(const QString& code){ mCode = code;}
    QString code() const {return mCode;}
    void setName(const QString& name){ mName = name;}
    QString name() const { return mName;}
    void setPY(const QString& abbr){mPY = abbr;}
    QString& py() const {return mPY;}
private:
    bool        mIsFav;
    int         mType;
    QString     mCode;
    QString     mName;
    QString     mPY; //拼音简称
};

#endif // HQBASEDATA_H
