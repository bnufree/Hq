#include "qsharebasicinfoworker.h"
#include "qsharecodeswork.h"
#include "data_structure/hqutils.h"
#include "dbservices/hqdatadefines.h"
#include <QThreadPool>
#include "qsharecodeswork.h"
#include "qsharefhspwork.h"
#include "qsharehsgttop10work.h"
#include "qsharefinancialinfowork.h"
#include <QFile>
#include "dbservices/dbservices.h"
#include "utils/comdatadefines.h"
#include "dbservices/qactivedate.h"

#define         STOCK_CODE_FILE         QString("%1share.data").arg(HQ_CODE_DIR)
QShareBasicInfoWorker::QShareBasicInfoWorker(QObject *parent) : QObject(parent)
{
    connect(this, SIGNAL(signalGetBasicInfo()), this, SLOT(slotGetBasicInfo()));
    moveToThread(&mWorkThread);
    mWorkThread.start();
}

void QShareBasicInfoWorker::slotGetBasicInfo()
{
    QThreadPool pool;
    pool.setMaxThreadCount(8);
    pool.setExpiryTimeout(-1);
    //取得代码
    pool.start(new QShareCodesWork(this));
    //取得分红送配
    pool.start(new QShareFHSPWork(this));
#if 0
    //取得北向交易TOP10
    pool.start(new QShareHsgtTop10Work(this));
    pool.waitForDone();
    //取得财务信息
    QStringList allCodes = DATA_SERVICE->getAllShareCodes();
    int pos = 0;
    int section = 200;
    while(pos < allCodes.length())
    {
        QStringList sublist = allCodes.mid(pos, section);
        pos += section;
        pool.start(new QShareFinancialInfoWork(sublist, this));
    }
#endif
    pool.waitForDone();
}
