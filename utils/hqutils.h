#ifndef HQUTILS_H
#define HQUTILS_H

#include <QDate>
#include <QDebug>
#include <QDateTime>
#include <QStringList>


#define         ANDROID_FILE_PATH       "/mnt/sdcard/com.hq.info"
class ShareDate: public QDate
{

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
