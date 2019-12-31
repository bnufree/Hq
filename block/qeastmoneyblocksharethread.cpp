#include "qeastmoneyblocksharethread.h"
#include <QRegExp>
#include "utils/qhttpget.h"
#include <QDebug>
#include "dbservices/dbservices.h"

QEastMoneyBlockShareThread::QEastMoneyBlockShareThread(const QString& pBlockCode, QObject *parent) :\
    mParent(parent),QRunnable()
{
    mBlockCode = pBlockCode;
}

QEastMoneyBlockShareThread::~QEastMoneyBlockShareThread()
{

}

void QEastMoneyBlockShareThread::run()
{
    QStringList     sharecodeslist;
    QString wkURL = QString("http://nufm.dfcfw.com/EM_Finance2014NumericApplication/JS.aspx?type=CT&cmd=C.BK0wkcode1&sty=FCOIATA&sortType=C&sortRule=-1&page=1&pageSize=1000&js=var%20quote_123%3d{rank:[(x)],pages:(pc)}&token=7bc05d0d4c3c22ef9fca8c2a912d779c&jsName=quote_123&_g=0.5186116359042079");
    wkURL.replace("wkcode", mBlockCode.right(3));
    //开始解析数据
    QByteArray bytes = QHttpGet::getContentOfURL(wkURL);
    QString result = QString::fromUtf8(bytes.data());


    int index = 0;
    QRegExp exp("[12]{1},(60[013][0-9]{3}|300[0-9]{3}|00[012][0-9]{3})");
    while((index = exp.indexIn(result, index)) >= 0)
    {
        QString code = exp.cap(1);
        index += exp.matchedLength();
        sharecodeslist.append(ShareData::fullCode(code));
        DATA_SERVICE->setShareBlock(code, mBlockCode);
    }

    if(sharecodeslist.length() > 0)
    {
        DATA_SERVICE->setBlockShareCodes(mBlockCode, sharecodeslist);
    }

}

