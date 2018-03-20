#include "hqutils.h"
#include <QTextCodec>
#include <Windows.h>
#include "profiles.h"

#define         DATE_STR_FORMAT         "yyyy-MM-dd"

QStringList HqUtils::mDatesList = Profiles::instance()->value("DateManage", "Expired").toStringList();
HqUtils::HqUtils()
{
}

bool HqUtils::weekend(const QDate &date)
{
    return date.dayOfWeek() == 6 || date.dayOfWeek() == 7;
}

bool HqUtils::activeDay(const QDate &date)
{
    return (!weekend(date)) && (!HqUtils::mDatesList.contains(date2Str(date)));
}

bool HqUtils::isActiveTime(const QTime &time)
{
    int act_start1 = 9*60+15;
    int act_end1 = 11*60+30;
    int act_start2 = 13*60;
    int act_end2 = 15*60;
    int hour = time.hour();
    int minute = time.minute();
    int res = hour *60 + minute;
    if((res >= act_start1 && res <= act_end1) || (res >= act_start2 && res <= act_end2))
    {
            return true;
    }
    return false;
}

bool HqUtils::isCurrentActive()
{
    QDateTime cur = QDateTime::currentDateTime();
    if(!activeDay((cur.date()))) return false;
    return isActiveTime(cur.time());
}


QString HqUtils::GetFirstLetter( const QString& unicode )
{
//    QTextCodec *gbkCodec = QTextCodec::codecForName("GBK");
//    char *strChs = gbkCodec->fromUnicode(unicode).toUpper().data();
    char *strChs = unicode.toLocal8Bit().data();
    qDebug()<<"chars:"<<QString::fromStdString(strChs)<<" src:"<<unicode;
 //   char *strChs = unicode.toLocal8Bit().data();
    static int li_SecPosValue[] = {
        1601, 1637, 1833, 2078, 2274, 2302, 2433, 2594, 2787, 3106, 3212,
        3472, 3635, 3722, 3730, 3858, 4027, 4086, 4390, 4558, 4684, 4925, 5249
    };
//    static char* lc_FirstLetter[] = {
//        "A", "B", "C", "D", "E", "F", "G", "H", "J", "K", "L", "M", "N", "O",
//        "P", "Q", "R", "S", "T", "W", "X", "Y", "Z"
//    };
    char* lc_FirstLetter = QString("ABCDEFGHJKLMNOPQRSTWXYZ").toLatin1().data();
    char* ls_SecondSecTable = QString("CJWGNSPGCGNE[Y[BTYYZDXYKYGT[JNNJQMBSGZSCYJSYY[PGKBZGY[YWJKGKLJYWKPJQHY[W[DZLSGMRYPYWWCCKZNKYYGTTNJJNYKKZYTCJNMCYLQLYPYQFQRPZSLWBTGKJFYXJWZLTBNCXJJJJTXDTTSQZYCDXXHGCK[PHFFSS[YBGXLPPBYLL[HLXS[ZM[JHSOJNGHDZQYKLGJHSGQZHXQGKEZZWYSCSCJXYEYXADZPMDSSMZJZQJYZC[J[WQJBYZPXGZNZCPWHKXHQKMWFBPBYDTJZZKQHYLYGXFPTYJYYZPSZLFCHMQSHGMXXSXJ[[DCSBBQBEFSJYHXWGZKPYLQBGLDLCCTNMAYDDKSSNGYCSGXLYZAYBNPTSDKDYLHGYMYLCXPY[JNDQJWXQXFYYFJLEJPZRXCCQWQQSBNKYMGPLBMJRQCFLNYMYQMSQYRBCJTHZTQFRXQHXMJJCJLXQGJMSHZKBSWYEMYLTXFSYDSWLYCJQXSJNQBSCTYHBFTDCYZDJWYGHQFRXWCKQKXEBPTLPXJZSRMEBWHJLBJSLYYSMDXLCLQKXLHXJRZJMFQHXHWYWSBHTRXXGLHQHFNM[YKLDYXZPYLGG[MTCFPAJJZYLJTYANJGBJPLQGDZYQYAXBKYSECJSZNSLYZHSXLZCGHPXZHZNYTDSBCJKDLZAYFMYDLEBBGQYZKXGLDNDNYSKJSHDLYXBCGHXYPKDJMMZNGMMCLGWZSZXZJFZNMLZZTHCSYDBDLLSCDDNLKJYKJSYCJLKWHQASDKNHCSGANHDAASHTCPLCPQYBSDMPJLPZJOQLCDHJJYSPRCHN[NNLHLYYQYHWZPTCZGWWMZFFJQQQQYXACLBHKDJXDGMMYDJXZLLSYGXGKJRYWZWYCLZMSSJZLDBYD[FCXYHLXCHYZJQ[[QAGMNYXPFRKSSBJLYXYSYGLNSCMHZWWMNZJJLXXHCHSY[[TTXRYCYXBYHCSMXJSZNPWGPXXTAYBGAJCXLY[DCCWZOCWKCCSBNHCPDYZNFCYYTYCKXKYBSQKKYTQQXFCWCHCYKELZQBSQYJQCCLMTHSYWHMKTLKJLYCXWHEQQHTQH[PQ[QSCFYMNDMGBWHWLGSLLYSDLMLXPTHMJHWLJZYHZJXHTXJLHXRSWLWZJCBXMHZQXSDZPMGFCSGLSXYMJSHXPJXWMYQKSMYPLRTHBXFTPMHYXLCHLHLZYLXGSSSSTCLSLDCLRPBHZHXYYFHB[GDMYCNQQWLQHJJ[YWJZYEJJDHPBLQXTQKWHLCHQXAGTLXLJXMSL[HTZKZJECXJCJNMFBY[SFYWYBJZGNYSDZSQYRSLJPCLPWXSDWEJBJCBCNAYTWGMPAPCLYQPCLZXSBNMSGGFNZJJBZSFZYNDXHPLQKZCZWALSBCCJX[YZGWKYPSGXFZFCDKHJGXDLQFSGDSLQWZKXTMHSBGZMJZRGLYJBPMLMSXLZJQQHZYJCZYDJWBMYKLDDPMJEGXYHYLXHLQYQHKYCWCJMYYXNATJHYCCXZPCQLBZWWYTWBQCMLPMYRJCCCXFPZNZZLJPLXXYZTZLGDLDCKLYRZZGQTGJHHGJLJAXFGFJZSLCFDQZLCLGJDJCSNZLLJPJQDCCLCJXMYZFTSXGCGSBRZXJQQCTZHGYQTJQQLZXJYLYLBCYAMCSTYLPDJBYREGKLZYZHLYSZQLZNWCZCLLWJQJJJKDGJZOLBBZPPGLGHTGZXYGHZMYCNQSYCYHBHGXKAMTXYXNBSKYZZGJZLQJDFCJXDYGJQJJPMGWGJJJPKQSBGBMMCJSSCLPQPDXCDYYKY[CJDDYYGYWRHJRTGZNYQLDKLJSZZGZQZJGDYKSHPZMTLCPWNJAFYZDJCNMWESCYGLBTZCGMSSLLYXQSXSBSJSBBSGGHFJLYPMZJNLYYWDQSHZXTYYWHMZYHYWDBXBTLMSYYYFSXJC[DXXLHJHF[SXZQHFZMZCZTQCXZXRTTDJHNNYZQQMNQDMMG[YDXMJGDHCDYZBFFALLZTDLTFXMXQZDNGWQDBDCZJDXBZGSQQDDJCMBKZFFXMKDMDSYYSZCMLJDSYNSBRSKMKMPCKLGDBQTFZSWTFGGLYPLLJZHGJ[GYPZLTCSMCNBTJBQFKTHBYZGKPBBYMTDSSXTBNPDKLEYCJNYDDYKZDDHQHSDZSCTARLLTKZLGECLLKJLQJAQNBDKKGHPJTZQKSECSHALQFMMGJNLYJBBTMLYZXDCJPLDLPCQDHZYCBZSCZBZMSLJFLKRZJSNFRGJHXPDHYJYBZGDLQCSEZGXLBLGYXTWMABCHECMWYJYZLLJJYHLG[DJLSLYGKDZPZXJYYZLWCXSZFGWYYDLYHCLJSCMBJHBLYZLYCBLYDPDQYSXQZBYTDKYXJY[CNRJMPDJGKLCLJBCTBJDDBBLBLCZQRPPXJCJLZCSHLTOLJNMDDDLNGKAQHQHJGYKHEZNMSHRP[QQJCHGMFPRXHJGDYCHGHLYRZQLCYQJNZSQTKQJYMSZSWLCFQQQXYFGGYPTQWLMCRNFKKFSYYLQBMQAMMMYXCTPSHCPTXXZZSMPHPSHMCLMLDQFYQXSZYYDYJZZHQPDSZGLSTJBCKBXYQZJSGPSXQZQZRQTBDKYXZKHHGFLBCSMDLDGDZDBLZYYCXNNCSYBZBFGLZZXSWMSCCMQNJQSBDQSJTXXMBLTXZCLZSHZCXRQJGJYLXZFJPHYMZQQYDFQJJLZZNZJCDGZYGCTXMZYSCTLKPHTXHTLBJXJLXSCDQXCBBTJFQZFSLTJBTKQBXXJJLJCHCZDBZJDCZJDCPRNPQCJPFCZLCLZXZDMXMPHJSGZGSZZQLYLWTJPFSYASMCJBTZKYCWMYTCSJJLJCQLWZMALBXYFBPNLSFHTGJWEJJXXGLLJSTGSHJQLZFKCGNNNSZFDEQFHBSAQTGYLBXMMYGSZLDYDQMJJRGBJTKGDHGKBLQKBDMBYLXWCXYTTYBKMRTJZXQJBHLMHMJJZMQASLDCYXYQDLQCAFYWYXQHZ").toLatin1().data();
    std::string result;
    int H = 0;
    int L = 0;
    int W = 0;
    UINT stringlen = strlen(strChs);
    for (UINT i = 0; i < stringlen; i++) {
        H = (UCHAR) (strChs[i + 0]);
        L = (UCHAR) (strChs[i + 1]);
        if (H < 0xA1 || L < 0xA1) {
            if(H != 42)
            {
                result += strChs[i];
            }
            continue;
        } else {
            W = (H - 160) * 100 + L - 160;
        }
        if (W > 1600 && W < 5590) {
            for (int j = 22; j >= 0; j--) {
                if (W >= li_SecPosValue[j]) {
                    result += lc_FirstLetter[j];
                    i ++;
                    break;
                }
            }
            continue;
        } else {
            i++;
            W = (H - 160 - 56) * 94 + L - 161;
            if (W >= 0 && W <= 3007)
                result += ls_SecondSecTable[W];
            else {
                result += (char) H;
                result += (char) L;
            }
        }
    }
    return QString::fromStdString(result);
}

bool HqUtils::writeString2File(const QString &str, FILE *fp)
{
    if(!fp) return false;
    QByteArray bytes = str.toUtf8();
    int size = bytes.size();
    fwrite(&size, sizeof(size), 1, fp);
    fwrite(bytes.data(), size, 1, fp);
    return true;
}

bool HqUtils::writeInt2File(int val, FILE *fp)
{
    if(!fp) return false;
    //qDebug()<<"val:"<<val;
    int wkval = val;
    fwrite(&wkval, sizeof(int), 1, fp);
    return true;
}
QString  HqUtils::date2Str(const QDate& date)
{
    return date.toString(DATE_STR_FORMAT);
}

QDate  HqUtils::dateFromStr(const QString& str)
{
    return QDate::fromString(str, DATE_STR_FORMAT);
}

QString HqUtils::double2Str(double val)
{
    QString wkval = QString::number(val, 'f', 3);
    if(wkval.lastIndexOf("0") == wkval.length() -1)
    {
        return QString::number(val, 'f', 2);
    }

    return wkval;
}

QDate   HqUtils::latestActiveDay()
{
    QDate date = QDate::currentDate();
    while(!activeDay(date))
    {
        date = date.addDays(-1);
    }

    return date;
}

QDate   HqUtils::lastActiveDay()
{
    QDate date =latestActiveDay().addDays(-1);
    while(!activeDay(date))
    {
        date = date.addDays(-1);
    }

    return date;
}

QDate HqUtils::getLgtStartDate()
{
    return QDate(2017,3,17);
}

QDate HqUtils::getActiveDayBefore1HYear()
{
    QDate date = QDate::currentDate();
    date = date.addDays(-182);
    while (!activeDay(date)) {
        date = date.addDays(1);
    }

    return date;
}

int   HqUtils::activeDaysNum(const QDate &start)
{
    int num = 0;
    QDate wkdate = start;
    while (wkdate < latestActiveDay()) {
        if(activeDay(wkdate))
        {
            num++;
        }
        wkdate = wkdate.addDays(1);
    }

    return num;
}


