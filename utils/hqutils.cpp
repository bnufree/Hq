#include "hqutils.h"

HqUtils::HqUtils()
{

}

bool HqUtils::isWeekend(const QDate &date)
{
    return date.dayOfWeek() == 6 || date.dayOfWeek() == 7;
}

SHARE_TYPE HqUtils::shareType(const QString &code)
{
    if( code.length() == 6)
    {
        if(code.left(1) == "6") return SHARE_SH;
        if(code.left(1) == "0" || code.left(1) == "3") return SHARE_SZ;
        if(code.left(1) == "5" || code.left(1) == "1") return SHARE_FOUND;
    } else if(code.length() == 5)
    {
        return SHARE_HK;
    }
    return SHARE_UNDEFINE;
}

QString HqUtils::prefixCode(const QString &code)
{
    SHARE_TYPE type = shareType(code);
    if(type == SHARE_SH) return "sh";
    if(type == SHARE_SZ) return "sz";
    if(type == SHARE_FOUND && code.left(1) == "5") return "sh";
    if(type == SHARE_FOUND && code.left(1) == "1") return "sz";
    if(type == SHARE_HK) return "hk";
    if(type == SHARE_US) return "us";
    return "undefined";
}
