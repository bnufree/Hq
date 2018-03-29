#include "qeastmoneynorthboundthread.h"
#include "utils/qhttpget.h"
#include "QRegExp"
#include <QDebug>
#include "utils/sharedata.h"


QEastmoneyNorthBoundThread::QEastmoneyNorthBoundThread(QObject *parent):QObject(parent)
{
    QStringList urlList;
    urlList.append(tr("http://nufm.dfcfw.com/EM_Finance2014NumericApplication/JS.aspx?type=CT&cmd=P.(x),(x),(x)|0000011|3990012|3990012,BK07071,BK08041&sty=SHSTD|SZSTD|FCSHSTR&st=z&sr=&p=&ps=&cb=&js=(x)&token=1942f5da9b46b069953c873404aad4b5"));

    urlList.append(QString("http://sc.hkex.com.hk/TuniS/www.hkex.com.hk/chi/csm/script/data_NBSH_Turnover_chi.js?_=%1").arg(QDateTime::currentMSecsSinceEpoch()));
    urlList.append(QString("http://sc.hkex.com.hk/TuniS/www.hkex.com.hk/chi/csm/script/data_NBSZ_Turnover_chi.js?_=%2").arg(QDateTime::currentMSecsSinceEpoch()));
    int num = 0;
    foreach (QString url, urlList) {
        QHttpGet *get = new QHttpGet(url, true);
        get->setUpdateInterval(30);
        if(num == 0)
        {
            get->setObjectName("EAST");
        } else if(num == 1)
        {
            get->setObjectName("BK0707");

        }
        else if(num == 2)
        {
            get->setObjectName("BK0804");
        }
        num++;
        connect(get, SIGNAL(signalSendHttpConent(QByteArray)),\
                this, SLOT(slotRecvHttpContent(QByteArray)));
        mNorthBoudProxyList.append(get);
    }

    //
    connect(this, SIGNAL(start()), this, SLOT(run()));

    this->moveToThread(&mWorkThread);
    mWorkThread.start();
}

QEastmoneyNorthBoundThread::~QEastmoneyNorthBoundThread()
{
    //if(mHttp) mHttp->deleteLater();
    mWorkThread.quit();
    mWorkThread.wait();
}

double QEastmoneyNorthBoundThread::changeRMBString(const QString &src)
{
    QRegExp exp("([0-9\\.\\-]{1,})");
    int index = exp.indexIn(src, 0);
    if(index != -1)
    {
        double value = exp.cap().toDouble();
        if(src.contains(QStringLiteral("亿元")))
        {
            return value * 100000000;
        }
        return value * 10000;
    }

    return 0.0;
}

void QEastmoneyNorthBoundThread::run()
{
    if(mNorthBoudProxyList.size())
    {
        foreach (QHttpGet* get, mNorthBoudProxyList) {
            get->start();
        }
        //mHttp->start();
    }
}

void QEastmoneyNorthBoundThread::slotRecvHttpContent(const QByteArray &bytes)
{
    QHttpGet *get = qobject_cast<QHttpGet*>(sender());
    if(!get) return;
    QString name = get->objectName();
    QString result = QString::fromUtf8(bytes);
    //qDebug()<<"res:"<<result.replace(QRegExp("[\\r\\n\\t]"), "");
    if(name == "EAST")
    {
        //开始解析
        //"5.12亿元,99.88亿元,105亿元,0,0.00万元,2500亿元,12.93亿元,117.07亿元,130亿元,0,0.00万元,0万元","2.92亿元,102.08亿元,105亿元,0,0.00万元,0万元,9.19亿元,120.81亿元,130亿元,0,0.00万元,0万元","399001,2,深证成指,0.23%,310|50|140|37","BK0707,1,沪股通,0.57%,349|59|166|31","BK0804,1,深股通,0.60%,608|109|241|84"
        QStringList splitstr = result.split(QRegExp("\"|\","), QString::SkipEmptyParts);
        QString hgt_pure_in = "0.0";
        QString sgt_pure_in = "0.0";
        if(splitstr.length() != 5) return;
        QString sh_north = splitstr[0];
        hgt_pure_in = sh_north.split(",").at(6);
        QString sz_north = splitstr[1];
        sgt_pure_in = sz_north.split(",").at(6);
        //qDebug()<<"hgt:"<<hgt_pure_in<<" sgt:"<<sgt_pure_in;
        double pure_in[2];
        pure_in[0] = changeRMBString(hgt_pure_in) / 100000000;
        pure_in[1] = changeRMBString(sgt_pure_in) / 100000000;

        for(int i=3; i<splitstr.length(); i++)
        {
            QStringList hgt = splitstr[i].split(",");
            bool exist = false;
            QString code = hgt[0];
            for(int i=0; i<mBoundDataList.length(); i++)
            {
                NS_BOUND_DATA& data = mBoundDataList[i];
                if(data.mCode == code)
                {
                    data.mName = hgt[2];
                    data.mChange = hgt[3].left(hgt[3].length() -1).toDouble();
                    data.mPure = pure_in[i-3];
                    exist = true;
                    break;
                }
            }
            if(!exist)
            {
                NS_BOUND_DATA data;
                data.mCode = code;
                data.mName = hgt[2];
                data.mChange = hgt[3].left(hgt[3].length() -1).toDouble();
                data.mPure = pure_in[i-3];
                mBoundDataList.append(data);
            }
        }
    } else
    {
        QRegExp money("RMB([0-9, ]{1,})Mil");
        int index = 0;
        QList<int> list;
        while((index = money.indexIn(result, index)) >= 0)
        {
            list.append(money.cap(1).remove(QRegExp(tr("[, ]"))).toInt());
            index += money.matchedLength();
        }
        qDebug()<<list.length()<<list;
        if(list.length() >=3)
        {
            for(int i=0; i<mBoundDataList.length(); i++)
            {
                NS_BOUND_DATA& data = mBoundDataList[i];
                //qDebug()<<name<<data.mCode;
                if(name == data.mCode)
                {
                    data.mTotal = list[0] * 100.0;
                    data.mBuy = list[1]*100;
                    data.mSell = list[2]*100;
                    data.mPure = data.mBuy - data.mSell;
                    break;
                }

            }

        }
    }
    //qDebug()<<
    foreach (NS_BOUND_DATA data, mBoundDataList) {
        qDebug()<<data.mCode<<data.mName<<data.mBuy<<data.mSell<<data.mPure<<data.mTotal;
    }
    emit signalUpdateNorthBoundList(mBoundDataList);
}
