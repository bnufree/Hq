#include "hqutils.h"
#include <QTextCodec>
//#include <Windows.h>
#include "utils/profiles.h"
#include <QDir>
#include <QFontMetrics>
#include <QApplication>
#include <QDesktopWidget>
#include <math.h>



//QStringList HqUtils::mDatesList = Profiles::instance()->value("DateManage", "Expired").toStringList();
HqUtils::HqUtils()
{
}







QString HqUtils::GetFirstLetter( const QString& unicode )
{
    QTextCodec *gbkCodec = QTextCodec::codecForName("GBK");
    QByteArray gbkArray = gbkCodec->fromUnicode(unicode);
    char *strChs = gbkArray.data();

//    QByteArray unicodeArray;
//    for(int i=0; i<unicode.length(); i++)
//    {
//        unicodeArray.append(QByteArray::number(unicode[i].unicode(), 16));
//    }
    //qDebug()<<"chars:"<<gbkArray<<" src:"<<unicode<<" unicode:"<<unicodeArray.toUpper();
 //   char *strChs = unicode.toLocal8Bit().data();
    static int li_SecPosValue[] = {
        1601, 1637, 1833, 2078, 2274, 2302, 2433, 2594, 2787, 3106, 3212,
        3472, 3635, 3722, 3730, 3858, 4027, 4086, 4390, 4558, 4684, 4925, 5249
    };
    static QString lc_FirstLetter[] = {
        "A", "B", "C", "D", "E", "F", "G", "H", "J", "K", "L", "M", "N", "O",
        "P", "Q", "R", "S", "T", "W", "X", "Y", "Z"
    };
    //char* lc_FirstLetter = QString("ABCDEFGHJKLMNOPQRSTWXYZ").toLocal8Bit().data();
    QString ls_SecondSecTable = "CJWGNSPGCGNE[Y[BTYYZDXYKYGT[JNNJQMBSGZSCYJSYY[PGKBZGY[YWJKGKLJYWKPJQHY[W[DZLSGMRYPYWWCCKZNKYYGTTNJJNYKKZYTCJNMCYLQLYPYQFQRPZSLWBTGKJFYXJWZLTBNCXJJJJTXDTTSQZYCDXXHGCK[PHFFSS[YBGXLPPBYLL[HLXS[ZM[JHSOJNGHDZQYKLGJHSGQZHXQGKEZZWYSCSCJXYEYXADZPMDSSMZJZQJYZC[J[WQJBYZPXGZNZCPWHKXHQKMWFBPBYDTJZZKQHYLYGXFPTYJYYZPSZLFCHMQSHGMXXSXJ[[DCSBBQBEFSJYHXWGZKPYLQBGLDLCCTNMAYDDKSSNGYCSGXLYZAYBNPTSDKDYLHGYMYLCXPY[JNDQJWXQXFYYFJLEJPZRXCCQWQQSBNKYMGPLBMJRQCFLNYMYQMSQYRBCJTHZTQFRXQHXMJJCJLXQGJMSHZKBSWYEMYLTXFSYDSWLYCJQXSJNQBSCTYHBFTDCYZDJWYGHQFRXWCKQKXEBPTLPXJZSRMEBWHJLBJSLYYSMDXLCLQKXLHXJRZJMFQHXHWYWSBHTRXXGLHQHFNM[YKLDYXZPYLGG[MTCFPAJJZYLJTYANJGBJPLQGDZYQYAXBKYSECJSZNSLYZHSXLZCGHPXZHZNYTDSBCJKDLZAYFMYDLEBBGQYZKXGLDNDNYSKJSHDLYXBCGHXYPKDJMMZNGMMCLGWZSZXZJFZNMLZZTHCSYDBDLLSCDDNLKJYKJSYCJLKWHQASDKNHCSGANHDAASHTCPLCPQYBSDMPJLPZJOQLCDHJJYSPRCHN[NNLHLYYQYHWZPTCZGWWMZFFJQQQQYXACLBHKDJXDGMMYDJXZLLSYGXGKJRYWZWYCLZMSSJZLDBYD[FCXYHLXCHYZJQ[[QAGMNYXPFRKSSBJLYXYSYGLNSCMHZWWMNZJJLXXHCHSY[[TTXRYCYXBYHCSMXJSZNPWGPXXTAYBGAJCXLY[DCCWZOCWKCCSBNHCPDYZNFCYYTYCKXKYBSQKKYTQQXFCWCHCYKELZQBSQYJQCCLMTHSYWHMKTLKJLYCXWHEQQHTQH[PQ[QSCFYMNDMGBWHWLGSLLYSDLMLXPTHMJHWLJZYHZJXHTXJLHXRSWLWZJCBXMHZQXSDZPMGFCSGLSXYMJSHXPJXWMYQKSMYPLRTHBXFTPMHYXLCHLHLZYLXGSSSSTCLSLDCLRPBHZHXYYFHB[GDMYCNQQWLQHJJ[YWJZYEJJDHPBLQXTQKWHLCHQXAGTLXLJXMSL[HTZKZJECXJCJNMFBY[SFYWYBJZGNYSDZSQYRSLJPCLPWXSDWEJBJCBCNAYTWGMPAPCLYQPCLZXSBNMSGGFNZJJBZSFZYNDXHPLQKZCZWALSBCCJX[YZGWKYPSGXFZFCDKHJGXDLQFSGDSLQWZKXTMHSBGZMJZRGLYJBPMLMSXLZJQQHZYJCZYDJWBMYKLDDPMJEGXYHYLXHLQYQHKYCWCJMYYXNATJHYCCXZPCQLBZWWYTWBQCMLPMYRJCCCXFPZNZZLJPLXXYZTZLGDLDCKLYRZZGQTGJHHGJLJAXFGFJZSLCFDQZLCLGJDJCSNZLLJPJQDCCLCJXMYZFTSXGCGSBRZXJQQCTZHGYQTJQQLZXJYLYLBCYAMCSTYLPDJBYREGKLZYZHLYSZQLZNWCZCLLWJQJJJKDGJZOLBBZPPGLGHTGZXYGHZMYCNQSYCYHBHGXKAMTXYXNBSKYZZGJZLQJDFCJXDYGJQJJPMGWGJJJPKQSBGBMMCJSSCLPQPDXCDYYKY[CJDDYYGYWRHJRTGZNYQLDKLJSZZGZQZJGDYKSHPZMTLCPWNJAFYZDJCNMWESCYGLBTZCGMSSLLYXQSXSBSJSBBSGGHFJLYPMZJNLYYWDQSHZXTYYWHMZYHYWDBXBTLMSYYYFSXJC[DXXLHJHF[SXZQHFZMZCZTQCXZXRTTDJHNNYZQQMNQDMMG[YDXMJGDHCDYZBFFALLZTDLTFXMXQZDNGWQDBDCZJDXBZGSQQDDJCMBKZFFXMKDMDSYYSZCMLJDSYNSBRSKMKMPCKLGDBQTFZSWTFGGLYPLLJZHGJ[GYPZLTCSMCNBTJBQFKTHBYZGKPBBYMTDSSXTBNPDKLEYCJNYDDYKZDDHQHSDZSCTARLLTKZLGECLLKJLQJAQNBDKKGHPJTZQKSECSHALQFMMGJNLYJBBTMLYZXDCJPLDLPCQDHZYCBZSCZBZMSLJFLKRZJSNFRGJHXPDHYJYBZGDLQCSEZGXLBLGYXTWMABCHECMWYJYZLLJJYHLG[DJLSLYGKDZPZXJYYZLWCXSZFGWYYDLYHCLJSCMBJHBLYZLYCBLYDPDQYSXQZBYTDKYXJY[CNRJMPDJGKLCLJBCTBJDDBBLBLCZQRPPXJCJLZCSHLTOLJNMDDDLNGKAQHQHJGYKHEZNMSHRP[QQJCHGMFPRXHJGDYCHGHLYRZQLCYQJNZSQTKQJYMSZSWLCFQQQXYFGGYPTQWLMCRNFKKFSYYLQBMQAMMMYXCTPSHCPTXXZZSMPHPSHMCLMLDQFYQXSZYYDYJZZHQPDSZGLSTJBCKBXYQZJSGPSXQZQZRQTBDKYXZKHHGFLBCSMDLDGDZDBLZYYCXNNCSYBZBFGLZZXSWMSCCMQNJQSBDQSJTXXMBLTXZCLZSHZCXRQJGJYLXZFJPHYMZQQYDFQJJLZZNZJCDGZYGCTXMZYSCTLKPHTXHTLBJXJLXSCDQXCBBTJFQZFSLTJBTKQBXXJJLJCHCZDBZJDCZJDCPRNPQCJPFCZLCLZXZDMXMPHJSGZGSZZQLYLWTJPFSYASMCJBTZKYCWMYTCSJJLJCQLWZMALBXYFBPNLSFHTGJWEJJXXGLLJSTGSHJQLZFKCGNNNSZFDEQFHBSAQTGYLBXMMYGSZLDYDQMJJRGBJTKGDHGKBLQKBDMBYLXWCXYTTYBKMRTJZXQJBHLMHMJJZMQASLDCYXYQDLQCAFYWYXQHZ";
    QString result;
    int H = 0;
    int L = 0;
    int W = 0;
    unsigned int stringlen = strlen(strChs);
    for (unsigned int i = 0; i < stringlen; i++) {
        H = (unsigned char) (strChs[i + 0]);
        L = (unsigned char) (strChs[i + 1]);
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

    //qDebug()<<"gbk:"<<gbkArray.toHex().toUpper()<<gbkArray.size()<<result.c_str();
    return result;
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


QString HqUtils::double2Str(double val)
{
    QString wkval = QString::number(val, 'f', 3);
    if(wkval.lastIndexOf("0") == wkval.length() -1)
    {
        return QString::number(val, 'f', 2);
    }

    return wkval;
}

bool HqUtils::makePath(const QString &path)
{
    QDir dir(path);
    if(!dir.exists())
    {
        dir.mkpath(path);
    }
    bool exist = dir.exists();
    qDebug()<<__func__<<__LINE__<<path<<exist;
    return exist;
}

int HqUtils::convertMM2Pixel(double mm)
{
    int total_height = QApplication::desktop()->geometry().height();
    int total_mm = QApplication::desktop()->heightMM();

    return int(ceil(total_height * 1.0 / total_mm * mm));
}

void HqUtils::setFontPixelSize(QFont *font, int size)
{
    if(size <= 0)
    {
        return;
    }
    font->setPixelSize(size);
}

int  HqUtils::calFontPixelSize(int mm)
{
    return convertMM2Pixel(mm);
}
