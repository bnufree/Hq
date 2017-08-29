#include "qeastmoneyzjlxthread.h"
#include <QDebug>
#include "qhttpget.h"

QEastMoneyZjlxThread::QEastMoneyZjlxThread(QObject *parent) : QThread(parent)
{
    qRegisterMetaType<QList<zjlxData>>("const QList<zjlxData>&");
}

QEastMoneyZjlxThread::~QEastMoneyZjlxThread()
{

}

void QEastMoneyZjlxThread::run()
{
    QString wkURL = QString("http://nufm.dfcfw.com/EM_Finance2014NumericApplication/JS.aspx/JS.aspx?type=ct&st=(BalFlowMain)&sr=-1&p=1&ps=5000&js=var%20jRmrwiia={pages:(pc),data:[(x)]}&token=894050c76af8597a853f5b408b759f5d&cmd=C._AB&sty=DCFFITA&rt=49490052");

    qDebug()<<"zjlx url:"<<wkURL;
    while(true)
    {
        //开始解析数据
        int index = -1;
        QByteArray bytes = QHttpGet::getContentOfURL(wkURL);
        //qDebug()<<"zjlx bytes"<<bytes;
        QString result = QString::fromLocal8Bit(bytes.data());

        //qDebug()<<"zjlx res"<<result;
        int startindex = -1, endindex = -1;
        startindex = result.indexOf("[") + 1;
        endindex = result.indexOf("]");
        if(startindex < 0 || endindex < 0) continue;
        QString hqStr = result.mid(startindex, endindex - startindex);
        //qDebug()<<"zjlx start:"<<startindex<<" end:"<<endindex;
        if( !hqStr.isEmpty() )
        {
            QList<zjlxData> list;
            QStringList blockInfoList = hqStr.split(QRegExp("\""), QString::SkipEmptyParts);
            foreach (QString info, blockInfoList) {
                QStringList InfoList = info.split(QRegExp(","), QString::SkipEmptyParts);
                if(InfoList.size() < 6) continue;
                zjlxData data;
                data.code = InfoList.at(1);
                data.zjlx = InfoList.at(5).toDouble();
                //qDebug()<<"list "<<list.length() +1<<" code:"<<data.code<<" zjlx:"<<data.zjlx<<endl;
                list.append(data);
            }

            emit sendZjlxDataList(list);

        }
        sleep(10);
    }
}

