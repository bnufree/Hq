#include "qeastmoneynorthboundthread.h"
#include "utils/qhttpget.h"
#include "QRegExp"
#include <QDebug>
#include "data_structure/sharedata.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonValue>
#include <QJsonObject>


QEastmoneyNorthBoundThread::QEastmoneyNorthBoundThread(QObject *parent):QObject(parent)
{
    QString url = "http://push2.eastmoney.com/api/qt/kamt/get?fields1=f1,f2,f3,f4&fields2=f51,f52,f53,f54&ut=b2884a393a59ad64002292a3e90d46a5&_=1563542656718";
    QHttpGet *get = new QHttpGet(url, true);
    get->setUpdateInterval(30);
    connect(get, SIGNAL(signalSendHttpConent(QByteArray)),\
            this, SLOT(slotRecvHttpContent(QByteArray)));
    mNorthBoudProxyList.append(get);
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
    double res = 0.0;
    if(index != -1)
    {
        double value = exp.cap().toDouble();
        if(src.contains(QStringLiteral("亿元")))
        {
            res = value * 100000000.0;
        } else
        {
            res = value * 10000.0;
        }
    }

    //qDebug()<<src<<res;

    return res;
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
    QString result = QString::fromUtf8(bytes);
    //{"rc":0,"rt":13,"svr":183017931,"lt":1,"full":0,"data":{"hk2sh":{"status":3,"dayNetAmtIn":449536.28,"dayAmtRemain":4750463.72,"dayAmtThreshold":5200000.0},"sh2hk":{"status":3,"dayNetAmtIn":37379.99,"dayAmtRemain":4162620.01,"dayAmtThreshold":4200000.0},"hk2sz":{"status":3,"dayNetAmtIn":203609.78,"dayAmtRemain":4996390.22,"dayAmtThreshold":5200000.0},"sz2hk":{"status":3,"dayNetAmtIn":48018.17,"dayAmtRemain":4151981.83,"dayAmtThreshold":4200000.0}}}
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(bytes, &err);
    if(err.error != QJsonParseError::NoError) return;
    if(!doc.isObject()) return;
    QJsonObject obj = doc.object();
    if(!obj.contains("data")) return;
    QJsonValue dataVal = obj.value("data");
    if(!dataVal.isObject()) return;
    obj = dataVal.toObject();
    QJsonValue hk2sh = obj.value("hk2sh");
    QJsonValue hk2sz = obj.value("hk2sz");
    if(!hk2sh.isObject() || !hk2sz.isObject()) return;
    QJsonObject hk2sh_obj = hk2sh.toObject();
    ShareHsgt sh;
    sh.mCode = "hk2sh";
    sh.mName = tr("沪股通");
    sh.mPure = hk2sh_obj.value("dayNetAmtIn").toDouble();
    QJsonObject hk2sz_obj = hk2sz.toObject();
    ShareHsgt sz;
    sz.mCode = "hk2sz";
    sz.mName = tr("深股通");
    sz.mPure = hk2sz_obj.value("dayNetAmtIn").toDouble();
    mBoundDataList[sh.mCode] = sh;
    mBoundDataList[sz.mCode] = sz;
    emit signalUpdateNorthBoundList(mBoundDataList.values());
}
