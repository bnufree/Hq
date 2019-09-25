#ifndef HQUTILS_H
#define HQUTILS_H

#include <QDate>
#include <QDebug>
#include <QDateTime>
#include <QStringList>
#include "shareworkingdatetime.h"

#define         ANDROID_FILE_PATH       "/mnt/sdcard/com.hq.info"



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

public:
    //static QStringList        mDatesList;
};

#endif // HQUTILS_H
