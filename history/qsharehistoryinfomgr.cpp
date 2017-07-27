#include "qsharehistoryinfomgr.h"
#include "qeastmoneystockhistoryinfothread.h"

QShareHistoryInfoMgr::QShareHistoryInfoMgr(QObject *parent) : QObject(parent)
{

}


void QShareHistoryInfoMgr::startGetHistoryInfo(const QStringList &list)
{
    foreach (QString code, list) {
        QEastmoneyStockHistoryInfoThread* thread = new QEastmoneyStockHistoryInfoThread(code);
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
        thread->start();
    }
}
