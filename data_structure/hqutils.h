#ifndef HQUTILS_H
#define HQUTILS_H

#include <QDate>
#include <QDebug>
#include <QDateTime>
#include <QStringList>
#include "date/shareworkingdatetime.h"

#define         ANDROID_FILE_PATH       "/mnt/sdcard/com.hq.info"



struct LinkData{
    QString mCode;
    QString mName;
};

struct KuaixunData{
    enum Source{
        Source_EastMoney = 0,
        Source_Ths,
    };

    QString strid;
    QString src_time;
    QString local_time;
    QString title;
    QString url;
    QString digest;
    int     source;
    QList<LinkData> mLinkDataList;
    bool operator <(const KuaixunData &other) const
    {
        if(local_time < other.local_time) return true;
        if(local_time == other.local_time) return src_time < other.src_time;
        return false;
    }

    bool operator >(const KuaixunData &other) const
    {
        if(local_time > other.local_time) return true;
        if(local_time == other.local_time) return src_time > other.src_time;
        return false;
    }
    QString sourceString() const
    {
        if(source == 0) return QStringLiteral("东方财富");
        return QStringLiteral("同花顺");
    }
};

typedef QList<KuaixunData> KuaiXunList;

Q_DECLARE_METATYPE(KuaixunData)
Q_DECLARE_METATYPE(KuaiXunList)

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
    //字体中的pixsize就是pix的长度  对应的就是字体的高度值,这里设定需要设定的高度毫米,返回字体的pixsize
    static int      calFontPixelSize(int target_mm);
    static int     convertMM2Pixel(double mm);
    static void    setFontPixelSize(QFont* font, int size);

public:
    //static QStringList        mDatesList;
};

#endif // HQUTILS_H
