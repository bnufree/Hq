#include "qeastmoneynorthboundthread.h"
#include "qhttpget.h"
#include "QRegExp"

QEastmoneyNorthBoundThread::QEastmoneyNorthBoundThread(QObject *parent = 0):QThread(parent)
{

}

QEastmoneyNorthBoundThread::run()
{
    QString url = tr("http://nufm.dfcfw.com/EM_Finance2014NumericApplication/JS.aspx?type=CT"
                     "&cmd=P.(x),(x),(x)|0000011|3990012|3990012,BK07071,BK08041&sty=SHSTD|"
                     "SZSTD|FCSHSTR&st=z&sr=&p=&ps=&cb=&js=var%20lHoZiFt=({data:[(x)]})&"
                     "token=1942f5da9b46b069953c873404aad4b5");
    QString result = QString::fromUtf8(QHttpGet().getContent(url));
    //开始解析
    QRegExp reg("\"([0-9]{1,2})\", \"([1-9]{1}[0-9]{0,5})\", \"([^\x00-\xff]{1,})\", \"(([0-9]{1,},{0,}){1,})\", \"(([0-9]{1,},{0,}){1,})\", \"(([0-9]{1,},{0,}){1,})\"");

}
