#ifndef BASEDATA_H
#define BASEDATA_H
#include <QString>
#include <QObject>

class BaseData
{
public:
    BaseData();
    virtual ~BaseData();
public:
    QString     name;
    QString     code;
    double      cur;
    double      chg;
    double      per;
    double      high;
    double      low;
    double      buy;
    double      sell;
public:
    bool operator <(const BaseData& other)
    {
        return this->per < other.per;
    }

    bool operator >(const BaseData& other)
    {
        return this->per > other.per;
    }
};

Q_DECLARE_METATYPE(BaseData)

#endif // BASEDATA_H
