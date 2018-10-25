#ifndef HQUTILS_H
#define HQUTILS_H

#include <QDate>
#include <QDebug>
#include <QDateTime>
#include <QStringList>


#define         ANDROID_FILE_PATH       "/mnt/sdcard/com.hq.info"

class ShareDate
{
public:
    ShareDate() {}
    ShareDate(const QDate& date):mDate(date){}
    ShareDate(uint time_t){mDate = QDateTime::fromTime_t(time_t).date();}
    ShareDate(const ShareDate& other) {mDate = other.mDate;}
    ShareDate& operator=(const ShareDate& other){
        if(this != &other) {
            this->mDate = other.mDate;
        }
        return *this;
    }

    uint toTime_t() const {return QDateTime(mDate).toTime_t();}
    static ShareDate fromTime_t(uint time_t)
    {
        return ShareDate(time_t);
    }
    QString toString(){
        if(toTime_t() == 0) return "-";
        return mDate.toString("yyyy-MM-dd");
    }
    static ShareDate fromString(const QString& str)
    {
        return ShareDate(QDate::fromString(str, "yyyy-MM-dd"));
    }

    static ShareDate currentDate(){
        return ShareDate(QDate::currentDate());
    }
    QDate   date() const {return mDate;}
    bool    isNull() const {return mDate.isNull();}

private:
    QDate       mDate;
};

class HqUtils
{

public:
    HqUtils();
    static QString prefixCode(const QString& code);
    static QString GetFirstLetter( const QString& unicode );
    static bool writeString2File(const QString& str, FILE *fp);
    static bool writeInt2File(int val, FILE* fp);
    static QString  double2Str(double val);
    static bool makePath(const QString& path);

public:
    //static QStringList        mDatesList;
};

#endif // HQUTILS_H
