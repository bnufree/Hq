#include "qeastmoneysharefhspthread.h"
#include <QEventLoop>

QEastMoneyShareFHSPThread::QEastMoneyShareFHSPThread(QObject *parent) : QObject(parent)
{

}

QEastMoneyShareFHSPThread::~QEastMoneyShareFHSPThread()
{

}

void QEastMoneyShareFHSPThread::updateFHSPFiles()
{
//    //更新所有股票的板块信息
//    slotUpdateFHSPInfo();
//    int index = 0;
//    int total = mShareBlockList.keys().length();
//    foreach (QString code, mShareBlockList.keys()) {
//        emit signalUpdateMsg(QString("%1:%2/%3").arg(tr("正在更新板块信息")).arg(++index).arg(total));
//        StkInfoFileManage *filemgr = new StkInfoFileManage(code.right(6));
//        filemgr->setValue("Block", "names", mShareBlockList[code]);
//        filemgr->setValue("FHSP", "Content", mShareFHSPList[code]);
//        filemgr->deleteLater();
//    }
}

void QEastMoneyShareFHSPThread::updateFHSPInfoWithDate(const QString &date)
{
//    //wkdate = 2016-12-31
//    QNetworkAccessManager *mgr = new QNetworkAccessManager;
//    QString url("http://data.eastmoney.com/DataCenter_V3/yjfp/getlist.ashx?js=var vWLdLOFe&pagesize=5000&page=1&sr=-1&sortType=SZZBL&mtk=%C8%AB%B2%BF%B9%C9%C6%B1&filter=(ReportingPeriod=^%1^)&rt=49499306");
//    QString wkURL = url.arg(date);
//    QNetworkReply *reply  = mgr->get(QNetworkRequest(wkURL));
//    if(!reply)
//    {
//        mgr->deleteLater();
//        return;
//    }
//    QEventLoop loop; // 使用事件循环使得网络通讯同步进行
//    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
//    loop.exec(); // 进入事件循环， 直到reply的finished()信号发出， 这个语句才能退出
//    if(reply->error())
//    {
//        qDebug()<<"err occured:"<<reply->errorString();
//        reply->deleteLater();
//        mgr->deleteLater();
//        return;
//    }


//    //结果分析
//    QByteArray bytes = reply->readAll();
//    QString result = QString::fromUtf8(bytes.data());
//    int startindex = -1, endindex = -1;
//    startindex = result.indexOf("[{");
//    endindex = result.indexOf("}]");
//    if(startindex < 0 || endindex < 0)
//    {
//        reply->deleteLater();
//        mgr->deleteLater();
//        return;
//    }
//    endindex += 2;
//    QString hqStr = result.mid(startindex, endindex - startindex);

//    QJsonParseError err;
//    QJsonDocument doc = QJsonDocument::fromJson(hqStr.toUtf8().data(), &err);
//    if(err.error != QJsonParseError::NoError)
//    {
//        qDebug()<<"parse json error occured.";
//        reply->deleteLater();
//        mgr->deleteLater();
//        return;
//    }

//    if(!doc.isArray())
//    {
//        qDebug()<<"json with wrong format.";
//        reply->deleteLater();
//        mgr->deleteLater();
//        return;
//    }
//    QJsonArray array = doc.array();
//    for(int i=0; i<array.count(); i++)
//    {
//        QJsonValue value = array.at(i);
//        if(!value.isObject()) continue;
//        QJsonObject subobj = value.toObject();
//        //开始解析角色信息数据
//        QString code = subobj.value("Code").toString();
//        QStringList list;
//        list.append(subobj.value("SZZBL").toString());
//        list.append(subobj.value("XJFH").toString());
//        list.append(subobj.value("GXL").toString());
//        list.append(subobj.value("GQDJR").toString());
//        list.append(subobj.value("YAGGR").toString().left(10));
//        mShareFHSPList[code] = list;
//    }

//    reply->deleteLater();
//    mgr->deleteLater();
}

