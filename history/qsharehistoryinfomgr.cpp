#include "qsharehistoryinfomgr.h"
#include "qeastmoneystockhistoryinfothread.h"
#include "dbservices.h"

QShareHistoryInfoMgr::QShareHistoryInfoMgr(QObject *parent) : QObject(parent)
{
    connect(this, SIGNAL(singalSetHistoryCodeList(QStringList)), this, SLOT(slotSetHistoryCodeList(QStringList)));
    connect(DATA_SERVICE, SIGNAL(signalSendShareHistoryLastDate(QString,QDate)), this, SLOT(slotRecvCodeHistoryDate(QString,QDate)));
    this->moveToThread(&mWorkThread);
    mWorkThread.start();
}


void QShareHistoryInfoMgr::slotSetHistoryCodeList(const QStringList &list)
{
    foreach (QString code, list) {
        DATA_SERVICE->signalQueryShareHistoryLastDate(code);
    }
}

void QShareHistoryInfoMgr::slotRecvCodeHistoryDate(const QString& code, const QDate& date)
{
    QEastmoneyStockHistoryInfoThread* thread = new QEastmoneyStockHistoryInfoThread(code, date);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}
