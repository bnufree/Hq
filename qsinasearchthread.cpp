#include "qsinasearchthread.h"
#include <QDateTime>
#include <QDebug>
#include "qhttpget.h"

QSinaSearchThread::QSinaSearchThread(QObject *parent) : QObject(parent)
{
    connect(this ,SIGNAL(signalSetSearchString(QString)), this, SLOT(slotRecvSearchString(QString)));
    moveToThread(&mWorkThread);
    mWorkThread.start();
}

QSinaSearchThread::~QSinaSearchThread()
{
    qDebug()<<__FUNCTION__<<__LINE__;

}

void QSinaSearchThread::slotRecvSearchString(const QString& text)
{
    QString wkURL = QString("http://suggest3.sinajs.cn/suggest/type=11&key=%1&name=suggestdata_%2")
            .arg(text)
            .arg(QDateTime::currentDateTime().toMSecsSinceEpoch());

    QByteArray bytes = QHttpGet().getContent(wkURL);
    QString result = QString::fromLocal8Bit(bytes.data());
    //qDebug()<<"result:"<<result;
    int start = 0;
    QStringList resultlist;
    QRegExp worker("[shz]{2}[0-9]{6}");
    while((start = worker.indexIn(result, start)) != -1)
    {
        //qDebug()<<"start:"<<start<<"   "<<worker.cap();
        resultlist.append(worker.cap());
        start += worker.cap().length();
    }
    //qDebug()<<"search result:"<<resultlist;
    emit sendSearchResult(resultlist);
}
